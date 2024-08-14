#pragma once
#include "ui_image.h"
#include <timer.h>

namespace ui_system {
	class ui_animation : public ui_image {
	public:
		int rows;
		int columns;
		bool loop = true;
		bool stop_at_row_end = false;
		float interval = (1.0f / 24.0f);
		ui_animation(
			const std::string& image,
			const int& rows_, const int& columns,
			const glm::vec3& anchor_point_ = glm::vec3(0.0f),
			const glm::mat4& model_matrix_ = glm::mat4(1.0f),
			const glm::vec3& pivot_point_ = glm::vec3(0.0f)
		);
		void draw() override;
		void play(const float& interval_ = 0.0277777778f);
		void pause();
		void unpause();
		void stop();
	private:
		glm::vec2 extent;
		time_system::function_timer t;
		void loop_play();
	};
}
