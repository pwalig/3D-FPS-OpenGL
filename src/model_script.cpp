#include "model_script.h"
#include "pbr_model.h"

game::model_script::model_script(const std::string& mesh, const std::string& texture, const std::string& texture1, const std::string& texture2, const glm::mat4& initial_matrix)
	: m(new renderer::pbr_model(mesh, texture, texture1, texture2, initial_matrix)) {}

game::model_script::~model_script()
{
	delete m;
}
