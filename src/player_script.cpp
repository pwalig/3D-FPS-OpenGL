#include "player_script.h"
#include <renderer.h>
#include <time_system.h>
#include <projectile.h>
#include "gameplay_manager.h"
#include "player_ui.h"
#include <damage_number.h>
#include "game_over_menu.h"
#include "geo_utils.h"
#include "crosshair_indicator.h"

std::vector<game::player*> game::player::players;

game::player::player(const glm::vec3& initial_position, const float& y_rotation) :
	rb(), col(&rb, this), dir(glm::vec3(0.0f, 0.0f, 1.0f)), floor_normal(VEC3_UP),
	gun_cooldown(std::bind(&game::player::auto_shoot, this)),
	l(glm::vec3(initial_position), glm::vec3(20.0f)),
	ui_dash_cooldown("../assets/textures/White_Square.png", glm::vec3(0.5f, 0.05f, 0.2f))
{
	// set up rigidbody
	rb.mass = 80.0f;
	rb.force = physics::gravity * rb.mass;
	rb.moment_of_inertia = 999999.9f; // make player almost unrotatable
	rb.restitution = 0.0f; // make player not bouncy
	rb.position = initial_position;
	rb.rotation = glm::rotate(glm::quat(glm::vec3(0.0f)), y_rotation, glm::vec3(glm::vec3(0.0f, 1.0f, 0.0f)));
	col.layer = COLLISION_LAYERS_PLAYER;

	// subscribe for collision event
	col.on_collision_stay.subscribe(std::bind(&game::player::land, this, std::placeholders::_1));
	col.on_collision_exit.subscribe([this](physics::collider* other) {
		this->floor_normal = VEC3_UP;
		});

	// dash function
	dash_timer.function = [this]() {
		this->max_speed = 7.0f;
		};
	dash_cooldown.function = [this]() {
		ready_to_dash = true;
		};

	// prepare gun and cubes
	// jump increasing cube
	hand_cubes.push_back(new power_cube(this, &game::cube_presets::jumping));
	hand_cubes.back()->set_ui_position(glm::vec3(0.05f, 0.05f, 0.5f));

	// speed increasing cube
	hand_cubes.push_back(new power_cube(this, &game::cube_presets::speed));
	hand_cubes.back()->set_ui_position(glm::vec3(0.1f, 0.05f, 0.5f));

	gun_cubes.push_back(new power_cube(this, &game::cube_presets::dash));
	gun_cubes.back()->set_ui_position(glm::vec3(0.90f, 0.05f, 0.5f));
	gun_cubes.push_back(new power_cube(this, &game::cube_presets::missle));
	gun_cubes.back()->set_ui_position(glm::vec3(0.95f, 0.05f, 0.5f));
	recoil_rb.movement_drag = 100.0f;
	scope_rb.movement_drag = 200.0f;
	scope_rb.position.x = 1.0f;

	// add yourself to players list
	players.push_back(this);

	// ui
	ui_dash_cooldown.color.a = 0.5f;
}

void game::player::start()
{
	game::gameplay_manager::player_position = &(this->rb.position);
	update_active_cube();
	update_active_gun();
}

void game::player::update()
{
	// rotation
	rot += glm::vec2(input_system::mouse_delta.y * rot_speed, input_system::mouse_delta.x * rot_speed) * (float)time_system::time_scale;
	rot.x -= recoil_rb.velocity.x * (float)time_system::delta_time;
	if (rot.x > max_rot) rot.x = max_rot;
	if (rot.x < -max_rot) rot.x = -max_rot;

	rb.rotation = glm::rotate(glm::quat(glm::vec3(0.0f)), rot.y, glm::vec3(0.0f, 1.0f, 0.0f)); // rotate around y axis only to preserve movement on xz plane

	// movement
	glm::vec3 move_dir = rotatation_between(VEC3_UP, floor_normal) * (rb.rotation * glm::vec3(move_in.normalized().x, 0.0f, move_in.normalized().y));
	float y_vel = glm::dot(rb.velocity, floor_normal); // velocity along the normal
	rb.velocity -= floor_normal * y_vel; // set velocity along the normal to 0
	if (glm::length(move_in.normalized()) != 0.0f) {
		rb.velocity += responsiveness * (float)time_system::delta_time * move_dir;
	}
	else if (glm::length(rb.velocity) > 0.0f) {
		if (responsiveness * (float)time_system::delta_time <= glm::length(rb.velocity)) rb.velocity -= responsiveness * (float)time_system::delta_time * glm::normalize(rb.velocity);
		else rb.velocity = glm::vec3(0.0f);
	}
	if (glm::length(rb.velocity) > max_speed) rb.velocity = glm::normalize(rb.velocity) * max_speed;
	rb.velocity += floor_normal * y_vel;  // set velocity along the normal back to y_vel

	// looking direction
	dir = glm::rotate(rb.rotation, rot.x, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec3(0, 0, 1); // rotate on x axis (up down) and calculate look direction

	glm::vec3 posi = this->rb.position + (glm::vec3(0.0f, 1.0f, 0.0f) * (this->col.spread / 2.0f)); // player head position
	renderer::active_camera.set_V(posi, posi + dir);

	recoil_rb.temp_force -= recoil_rb.position * 100.0f;

	// scope
	if (input_system::key_held[GLFW_MOUSE_BUTTON_2]) {
		scope_rb.temp_force.x += ((gun->scope) - scope_rb.position.x) * 1000.0f;
	}
	else {
		scope_rb.temp_force.x += (1.0f - scope_rb.position.x) * 1000.0f;
	}
	renderer::active_camera.set_fov(scope_rb.position.x * renderer::global_fov);
	input_system::mouse_sensitivity_multiplier = scope_rb.position.x;

	// light
	this->l.position = this->rb.position;

	//ui
	ui_dash_cooldown.model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f * dash_cooldown.time, 0.005f, 0.1f));
}

void game::player::damage(int damage, glm::vec3 damage_source_position)
{
	this->entity::damage(game::gameplay_manager::multiply_by_difficulty(damage, 0.6f), damage_source_position);

	crosshair_indicator* hiti = new crosshair_indicator("../assets/UI/hit-indicator.png");
	
	glm::vec2 v1 = glm::vec2(this->dir.x, this->dir.z);
	glm::vec2 v2 = glm::normalize(glm::vec2(damage_source_position.x - this->rb.position.x, damage_source_position.z - this->rb.position.z));

	float angle = glm::acos(glm::dot(v1, v2));
	printf("%f\n", angle * 180.0f / PI);
	if (glm::cross(glm::vec3(v2, 0.0f), glm::vec3(v1, 0.0f)).z < 0.0f) angle = -angle;

	hiti->uii.model_matrix = glm::scale(
		glm::translate(
			glm::rotate(
				glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)
			), glm::vec3(0.0f, 0.08f, 0.0f)
		), glm::vec3(0.07f, 0.034f, 1.0f)
	);

	// update healt bar
	game::player_ui* ui = scripts_system::find_script_of_type<game::player_ui>("hud");
	if (ui != nullptr) {
		ui->hp_bar.model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(this->hp / 1000.0f, 0.01f, 1.0f));
	}
}

void game::player::heal(int healing)
{
	this->entity::heal(game::gameplay_manager::multiply_by_difficulty(healing, 0.6f, true));

	// update healt bar
	game::player_ui* ui = scripts_system::find_script_of_type<game::player_ui>("hud");
	if (ui != nullptr) {
		ui->hp_bar.model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(this->hp / 1000.0f, 0.01f, 1.0f));
	}
}

void game::player::die()
{
	printf("you died\n");
	this->hp = 0;
	new game::game_over_menu();
}

void game::player::use_weapon(game::weapon* weapon)
{
	if (gun_cooldown.time > 0.0f || game::gameplay_manager::game_paused) return;
	glm::vec3 pos = this->rb.position + (glm::vec3(0.0f, 1.0f, 0.0f) * (this->col.spread / 2.0f)); // player head position
	weapon->shoot(pos, this->dir, COLLISION_LAYERS_PLAYER_PROJECTILES);
	gun_cooldown.start(weapon->cooldown);

	//recoil
	recoil_rb.velocity = glm::vec3(weapon->recoil, 0, 0);
	recoil_rb.position = glm::vec3(0.0f);

	//ui cooldown
	game::player_ui* ui = scripts_system::find_script_of_type<game::player_ui>("hud");
	if (ui != nullptr) {
		ui->gun_cooldown.play(weapon->cooldown / 36.0f);
	}	
}

void game::player::use_dash(const float& speed, const float& duration, const float& cooldown)
{
	if (ready_to_dash) {
		glm::vec3 move_dir = rotatation_between(VEC3_UP, floor_normal) * (rb.rotation * glm::vec3(move_in.normalized().x, 0.0f, move_in.normalized().y));
		if (glm::length(move_dir) > 0.0f) {
			max_speed = game::gameplay_manager::multiply_by_difficulty(speed, 0.1f, true);
			float y_vel = glm::dot(rb.velocity, floor_normal); // velocity along the normal
			rb.velocity -= floor_normal * y_vel; // set velocity along the normal to 0
			rb.velocity = move_dir * max_speed;
			rb.velocity += floor_normal * y_vel;  // set velocity along the normal back to y_vel
			ready_to_dash = false;
			dash_timer.start(duration);
			dash_cooldown.start(game::gameplay_manager::multiply_by_difficulty(cooldown, 0.2f));
		}
		else {
			glm::vec3 move_dir = rb.velocity;
			float y_vel = glm::dot(rb.velocity, floor_normal); // velocity along the normal
			move_dir -= floor_normal * y_vel; // set velocity along the normal to 0
			if (glm::length(move_dir) > 0.0f) {
				max_speed = game::gameplay_manager::multiply_by_difficulty(speed, 0.1f, true);
				rb.velocity = glm::normalize(move_dir) * max_speed;
				ready_to_dash = false;
				dash_timer.start(duration);
				dash_cooldown.start(game::gameplay_manager::multiply_by_difficulty(cooldown, 0.2f));
			}
		}
	}
}

void game::player::jump()
{
	if (ready_to_jump) {
		ready_to_jump = false;
		responsiveness = air_responsiveness;
		rb.velocity.y = jump_force;
	}
}

void game::player::land(physics::collision_info ci)
{
	if ((ci.other->layer != COLLISION_LAYERS_ENVIRIONMENT) && (ci.other->layer != COLLISION_LAYERS_DEFAULT)) return;
	if (ci.normal.y > 0.5f) {
		floor_normal = ci.normal;
		if (!ready_to_jump) {
			ready_to_jump = true;
			responsiveness = ground_responsiveness;
		}
		if (input_system::key_held[GLFW_KEY_SPACE]) jump();
	}
 }

void game::player::dash()
{
	this->use_dash(24.0f, 0.1f, 1.4f);
}


// GUNS

void game::player::shoot()
{
	this->use_weapon(this->gun);
}

void game::player::auto_shoot()
{
	if (gun->auto_repeat && input_system::key_held[GLFW_MOUSE_BUTTON_1]) this->shoot();
}

void game::player::update_active_gun()
{
	std::string cube_arrangement = "";
	for (power_cube* pc : gun_cubes) cube_arrangement += pc->preset->type;
	this->gun = weapon::weapon_map[cube_arrangement];
	printf("gun: %s\n", cube_arrangement.c_str());

	for (game::power_cube* pc : this->gun_cubes) {
		pc->visual.color.a = 1.0f;
	}

	//ui gun
	game::player_ui* ui = scripts_system::find_script_of_type<game::player_ui>("hud");
	if (ui != nullptr) {
		ui->active_gun.image = renderer::texture_ptr(this->gun->icon);
	}
}


// POWER CUBES

void game::player::use_cube()
{
	if (this->active_cube == nullptr || gameplay_manager::game_paused) return; // cannot use the cube
	this->active_cube->use();
	this->update_active_cube();
}


void game::player::cycle_cubes(const bool& reverse)
{
	if (reverse) {
		this->gun_cubes.push_front(this->hand_cubes.back());
		this->hand_cubes.push_front(this->gun_cubes.back());
		gun_cubes.pop_back();
		hand_cubes.pop_back();

		// visuals
		gun_cubes.front()->target_ui_pos.x = 0.95f - ((float)(gun_cubes.size()) * 0.05f);
		hand_cubes.front()->target_ui_pos.x = 0.0f;

		for (game::power_cube* pc : this->gun_cubes)
			pc->target_ui_pos.x += 0.05f;

		for (game::power_cube* pc : this->hand_cubes)
			pc->target_ui_pos.x += 0.05f;
	}
	else {
		this->gun_cubes.push_back(this->hand_cubes.front());
		this->hand_cubes.push_back(this->gun_cubes.front());
		gun_cubes.pop_front();
		hand_cubes.pop_front();

		// visuals
		gun_cubes.back()->target_ui_pos.x = 1.0f;
		hand_cubes.back()->target_ui_pos.x = ((float)(hand_cubes.size()) * 0.05f) + 0.05f;

		for (game::power_cube* pc : this->gun_cubes) 
			pc->target_ui_pos.x -= 0.05f;

		for (game::power_cube* pc : this->hand_cubes) 
			pc->target_ui_pos.x -= 0.05f;
	}
	this->update_active_gun();
	this->update_active_cube();
}

void game::player::update_active_cube()
{
	for (game::power_cube* pc : this->hand_cubes) {
		if (pc->t.time <= 0.0f) {
			this->active_cube = pc;
			printf("cube: %c\n", this->active_cube->preset->type);
			update_cubes_ui();
			return;
		}
	}
	this->active_cube = nullptr;
	printf("cube: none\n");
	update_cubes_ui();
}

void game::player::update_cubes_ui() {
	for (game::power_cube* pc : this->hand_cubes) {
		if (pc == active_cube) pc->visual.color.a = 0.85f;
		else if (pc->t.time <= 0.0f) pc->visual.color.a = 0.5f;
		else pc->visual.color.a = 0.2f;
	}
}

void game::player::cube_heal() {
	if (this->active_cube == nullptr || gameplay_manager::game_paused || hp == max_hp) return; // cannot use the cube
	this->active_cube->heal(); // if active_cube not null then it's timer must be 0.0f
	update_active_cube();
}

game::player::~player()
{
	for (game::power_cube* pc : hand_cubes) delete pc;
	for (game::power_cube* pc : gun_cubes) delete pc;
	std::vector<game::player*>::iterator id = std::find(game::player::players.begin(), game::player::players.end(), this);
	if (id != game::player::players.end()) game::player::players.erase(id);
}

game::player* game::player::get_closest_player(const glm::vec3& position)
{
	float min_dist = std::numeric_limits<float>::max();
	game::player* out = nullptr;
	for (game::player* pl : game::player::players) {
		float dist = glm::length(pl->rb.position - position);
		if (min_dist > dist) {
			min_dist = dist;
			out = pl;
		}
	}
	return out;
}

glm::vec3 game::player::get_closest_player_position(const glm::vec3& position)
{
	game::player* pl = game::player::get_closest_player(position);
	if (pl) return pl->rb.position;
	else return position;
}

game::player* game::player::get_player_by_name(const std::string& name)
{
	for (game::player* pl : game::player::players) {
		if (pl->name == name) return pl;
	}
}
