#pragma once
#include "ui_image.h"
#include <timer.h>

namespace ui_system {
	class ui_animation : public ui_image {
	public:
		int rows;
		int columns;
		bool loop = true;
		float interval = 0.0416666667f;
		bool reverse = false;
		ui_animation(
			const std::string& image_,
			const int& rows_, const int& columns_,
			const glm::vec3& anchor_point_ = glm::vec3(0.0f),
			const glm::mat4& model_matrix_ = glm::mat4(1.0f),
			const glm::vec3& pivot_point_ = glm::vec3(0.0f)
		);
		void draw() override;
		void play(const float& interval_ = 0.0416666667f);
		void pause();
		void unpause();
		void stop();
		void swap_image(
			const std::string& image_,
			const int& rows_, const int& columns_
		);
	private:
		glm::vec2 extent;
		time_system::function_timer t;
		void loop_play();
	};
}
