#pragma once
#include "model.h"
#include "game_script.h"
#include <rigidbody.h>
#include <physics.h>

namespace game {
	template<typename T>
	class physics_object : public scripts_system::script {
	public:
		physics::rigidbody rb;
		T col;
		renderer::model model;

		physics_object(glm::vec3 scale = glm::vec3(1.0f));
		void update() override;

	private:
		glm::vec3 _scale;
	};
}

template<typename T>
inline game::physics_object<T>::physics_object(glm::vec3 scale) : rb(), col(&rb, this), model(), _scale(scale) { }

template<typename T>
inline void game::physics_object<T>::update() {
	this->model.model_matrix = glm::scale(this->rb.model_matrix(), this->_scale);
}