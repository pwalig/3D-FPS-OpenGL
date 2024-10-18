#pragma once
#include <functional>
#include <ui_model.h>
#include "timer.h"
#include "light.h"
#include "pbr_model.h"

namespace game {
	class player;
}
#include "player_script.h"
#include <ui_animation.h>

namespace game {
	class cube_preset {
	public:
		float cooldown = 5.0f;
		int healing = 25;
		std::function<void(game::player*)> on_use = [](game::player* owner) {
			printf("used a cube\n");
			};
		glm::vec4 color = glm::vec4(1.0f);

		static std::vector<cube_preset*> all;
		static cube_preset* get_random();
	};

	class power_cube : public scripts_system::script {
	public:
		game::player* owner;
		cube_preset* preset;
		time_system::function_timer t;
		ui_system::ui_model visual;
		void use();
		void heal();

		power_cube(game::player* owner_, game::cube_preset* preset_);
		void update() override;

		glm::vec3 target_ui_pos = glm::vec3(0.0f);
		void set_ui_position(const glm::vec3& new_position);
		friend player;

	private:
		physics::rigidbody visual_rb;
		ui_system::ui_animation ui_cooldown;
	};

	class collectable_cube : public scripts_system::script {
	public:
		collectable_cube(const glm::vec3& position_, const float& radius);
		renderer::pbr_model visual;
		physics::colliders::sphere col;
		glm::vec3 position;
		renderer::light l;
	};

	namespace cube_presets {
		extern cube_preset jumping;
		extern cube_preset speed;
		extern cube_preset dash;
		extern cube_preset missle;
	}
}
