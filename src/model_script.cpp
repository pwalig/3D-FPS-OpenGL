#include "model_script.h"
#include "lambert_textured_model.h"

game::model_script::model_script(const std::string& mesh, const std::string& texture, const std::string& texture1, const std::string& texture2, const glm::mat4& initial_matrix)
	: m(new renderer::pbr_model(mesh, texture, texture1, texture2, initial_matrix))
	//: m(new renderer::lambert_textured_model(mesh, texture1, initial_matrix))
{}

game::model_script::~model_script()
{
	delete m;
}
