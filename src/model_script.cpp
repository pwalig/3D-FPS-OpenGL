#include "model_script.h"
#include "lambert_model.h"

game::model_script::model_script(const std::string& mesh, const glm::mat4& initial_matrix) : m(new renderer::lambert_model(mesh, initial_matrix)) {}

game::model_script::~model_script()
{
	delete m;
}
