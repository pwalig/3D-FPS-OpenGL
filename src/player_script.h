#pragma once
#include <physics.h>
#include <scripts_system.h>
#include <input_system.h>
#include <axis.h>
#include <constants.h>
#include <key_bind.h>
#include "entity.h"
#include <deque>
#include <weapon.h>
#include "light.h"

namespace game {
	class power_cube;
}
#include <power_cube.h>

namespace game {
	class player : public game::entity {
	public:
		player(const glm::vec3& initial_position, const float& y_rotation);
		void start() override;
		void update() override;
		void damage(int damage) override;
		void heal(int healing) override;
		void die() override;

		void update_active_cube();

		~player();

		// static access
		static std::vector<game::player*> players;
		static game::player* get_closest_player(const glm::vec3& position);
		static glm::vec3 get_closest_player_position(const glm::vec3& position);
		static game::player* get_player_by_name(const std::string& name);
	private:
		void jump();
		void land(physics::collision_info ci);
		void dash();

		void shoot();
		void auto_shoot();
		void update_active_gun();

		physics::rigidbody rb;
		physics::colliders::capsule col;

		game::weapon* gun = nullptr;
		time_system::function_timer gun_cooldown;
		physics::rigidbody recoil_rb;
		physics::rigidbody scope_rb;

		// power cubes
		game::power_cube* active_cube = nullptr;
		void use_cube();
		std::deque<game::power_cube*> hand_cubes;
		std::deque<game::power_cube*> gun_cubes;
		void cycle_cubes(const bool& reverse = false);
		void cube_heal();

		glm::vec3 dir; // looking direction
		glm::vec3 floor_normal; // how to rotate coorditante system when applying movement

		// light
		renderer::light l;

	public:
		float max_speed = 7.0f; // [m/s]
		float responsiveness = 1.0f; // [m/s^2]
		float ground_responsiveness = 100.0f; // [m/s^2]
		float air_responsiveness = 20.0f; // [m/s^2]

		float jump_force = 5.5f; // actually velocity [m/s]
		float dash_force = 14.0f; // actually velocity [m/s]
		bool ready_to_jump = false;

		float rot_speed = PI * 0.5f;
		float max_rot = PI * 0.49f;
		glm::vec2 rot = glm::vec2(0.0f);

	private:
		input_system::double_axis move_in = input_system::double_axis(GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_W, GLFW_KEY_S);
		input_system::key_bind shoot_key_bind = input_system::key_bind(std::bind(&game::player::shoot, this), GLFW_MOUSE_BUTTON_1, GLFW_PRESS);
		input_system::key_bind dash_key_bind = input_system::key_bind(std::bind(&game::player::dash, this), GLFW_KEY_LEFT_SHIFT, GLFW_PRESS);
		input_system::key_bind cube_key_bind = input_system::key_bind(std::bind(&game::player::use_cube, this), GLFW_KEY_F, GLFW_PRESS);
		input_system::key_bind cube_heal_key_bind = input_system::key_bind(std::bind(&game::player::cube_heal, this), GLFW_KEY_E, GLFW_PRESS);
		input_system::key_bind cube_cycle_key_bind_up = input_system::key_bind(std::bind(&game::player::cycle_cubes, this, false), GLFW_MOUSE_BUTTON_LAST + 2, GLFW_PRESS);
		input_system::key_bind cube_cycle_key_bind_down = input_system::key_bind(std::bind(&game::player::cycle_cubes, this, true), GLFW_MOUSE_BUTTON_LAST + 2, GLFW_RELEASE);
	};
}