#include "player_script.h"
#include <renderer.h>
#include <time_system.h>
#include <projectile.h>
#include "gameplay_manager.h"
#include "player_ui.h"
#include <damage_number.h>
#include "game_over_menu.h"

std::vector<game::player*> game::player::players;

game::player::player(const glm::vec3& initial_position, const float& y_rotation) :
	rb(), col(&rb, this), dir(glm::vec3(0.0f, 0.0f, 1.0f)),
	gun_cooldown(std::bind(&game::player::auto_shoot, this)),
	l(glm::vec3(initial_position), glm::vec3(25.0f)) {
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

	// prepare gun and cubes
	// jump increasing cube
	hand_cubes.push_back(new power_cube(this));
	hand_cubes.back()->type = 'c';
	hand_cubes.back()->on_use = [this]() {
		printf("jump_cube\n");
		float jmp = this->jump_force; // store previous jump force
		this->jump_force = 20.0f; // increase jump force
		time_system::call_in([this, jmp]() { this->jump_force = jmp; }, 3.0f); // set jump force back to normal after 3 seconds
		};

	// speed increasing cube
	hand_cubes.push_back(new power_cube(this));
	hand_cubes.back()->type = 'd';
	hand_cubes.back()->on_use = [this]() {
		printf("speed_cube\n");
		float spd = this->max_speed; // store previous speed
		this->max_speed = 13.0f; // increase speed
		time_system::call_in([this, spd]() { this->max_speed = spd; }, 5.0f); // set speed back to normal after 3 seconds
		};

	gun_cubes.push_back(new power_cube(this));
	gun_cubes.back()->on_use = []() { printf("xd\n"); };
	gun_cubes.push_back(new power_cube(this));
	gun_cubes.back()->type = 'b';
	update_active_cube();
	update_active_gun();
	recoil_rb.movement_drag = 100.0f;
	scope_rb.movement_drag = 200.0f;
	scope_rb.position.x = renderer::global_fov;

	// add yourself to players list
	players.push_back(this);
}

void game::player::start()
{
	game::gameplay_manager::player_position = &(this->rb.position);
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
	glm::vec3 move_dir = rb.rotation * glm::vec3(move_in.normalized().x, 0.0f, move_in.normalized().y);
	float y_vel = rb.velocity.y;
	rb.velocity.y = 0.0f;
	if (glm::length(move_in.normalized()) != 0.0f) {
		rb.velocity += responsiveness * (float)time_system::delta_time * move_dir;
	}
	else if (glm::length(rb.velocity) > 0.0f) {
		if (responsiveness * (float)time_system::delta_time <= glm::length(rb.velocity)) rb.velocity -= responsiveness * (float)time_system::delta_time * glm::normalize(rb.velocity);
		else rb.velocity = glm::vec3(0.0f);
	}
	if (glm::length(rb.velocity) > max_speed) rb.velocity = glm::normalize(rb.velocity) * max_speed;
	rb.velocity.y = y_vel;

	// looking direction
	dir = glm::rotate(rb.rotation, rot.x, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec3(0, 0, 1); // rotate on x axis (up down) and calculate look direction

	glm::vec3 posi = this->rb.position + (glm::vec3(0.0f, 1.0f, 0.0f) * (this->col.spread / 2.0f)); // player head position
	renderer::active_camera.set_V(posi, posi + dir);

	recoil_rb.temp_force -= recoil_rb.position * 100.0f;

	// scope
	if (input_system::key_held[GLFW_MOUSE_BUTTON_2]) {
		scope_rb.temp_force.x += ((gun->scope * renderer::global_fov) - scope_rb.position.x) * 1000.0f;
	}
	else {
		scope_rb.temp_force.x += (renderer::global_fov - scope_rb.position.x) * 1000.0f;
	}
	renderer::active_camera.set_fov(scope_rb.position.x);
	input_system::mouse_sensitivity_multiplier = scope_rb.position.x / 100.0f;

	// light
	this->l.position = this->rb.position;
}

void game::player::damage(int damage)
{
	this->entity::damage(damage);

	// update healt bar
	game::player_ui* ui = scripts_system::find_script_of_type<game::player_ui>("hud");
	if (ui != nullptr) {
		ui->hp_bar.model_matrix = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(960.0f, 20.0f, -10.0f)), glm::vec3(this->hp * 4.0f, 25.0f, 1.0f));
	}
}

void game::player::heal(int healing)
{
	this->entity::heal(healing);

	// update healt bar
	game::player_ui* ui = scripts_system::find_script_of_type<game::player_ui>("hud");
	if (ui != nullptr) {
		ui->hp_bar.model_matrix = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(960.0f, 20.0f, -10.0f)), glm::vec3(this->hp * 4.0f, 25.0f, 1.0f));
	}
}

void game::player::die()
{
	printf("you died\n");
	this->hp = 0;
	new game::game_over_menu();
}

void game::player::jump()
{
	if (ready_to_jump) {
		ready_to_jump = false;
		responsiveness = air_responsiveness;
		rb.velocity.y = jump_force;
	}
}

void game::player::land(physics::collision_info ci) {
	if (!ready_to_jump && ci.normal.y > 0.1f) {
		ready_to_jump = true;
		responsiveness = ground_responsiveness;
	}
	if (input_system::key_held[GLFW_KEY_SPACE]) jump();
 }


// GUNS

void game::player::shoot()
{
	if (gun_cooldown.time > 0.0f || game::gameplay_manager::game_paused) return;
	glm::vec3 pos = this->rb.position + (glm::vec3(0.0f, 1.0f, 0.0f) * (this->col.spread / 2.0f)); // player head position
	gun->shoot(pos, this->dir, COLLISION_LAYERS_PLAYER_PROJECTILES);
	gun_cooldown.start(gun->cooldown);

	//recoil
	recoil_rb.velocity = glm::vec3(gun->recoil, 0, 0);
	recoil_rb.position = glm::vec3(0.0f);
}

void game::player::auto_shoot()
{
	if (gun->auto_repeat && input_system::key_held[GLFW_MOUSE_BUTTON_1]) this->shoot();
}

void game::player::update_active_gun()
{
	std::string cube_arrangement = "";
	for (power_cube* pc : gun_cubes) cube_arrangement += pc->type;
	this->gun = weapon::weapon_map[cube_arrangement];
	printf("gun: %s\n", cube_arrangement.c_str());
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
	}
	else {
		this->gun_cubes.push_back(this->hand_cubes.front());
		this->hand_cubes.push_back(this->gun_cubes.front());
		gun_cubes.pop_front();
		hand_cubes.pop_front();
	}
	this->update_active_gun();
	this->update_active_cube();
}

void game::player::update_active_cube()
{
	for (game::power_cube* pc : this->hand_cubes) {
		if (pc->t.time <= 0.0f) {
			this->active_cube = pc;
			printf("cube: %c\n", this->active_cube->type);
			return;
		}
	}
	this->active_cube = nullptr;
	printf("cube: none\n");
}

void game::player::cube_heal() {
	if (this->active_cube == nullptr || gameplay_manager::game_paused) return; // cannot use the cube
	this->heal(active_cube->healing); // if active_cube not null then it's timer must be 0.0f
	this->active_cube->t.start(2.0f);
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
