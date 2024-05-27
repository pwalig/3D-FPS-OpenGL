#include "model_script.h"
#include "lambert_textured_model.h"

game::model_script::model_script(const std::string& mesh, const GLuint* texture,const glm::mat4& initial_matrix) : m(new renderer::lambert_textured_model(mesh,texture, initial_matrix)) {}

game::model_script::~model_script()
{
	delete m;
}
