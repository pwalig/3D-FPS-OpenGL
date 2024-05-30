#include "model_script.h"

game::model_script::model_script(renderer::model * model) : m(model) {}

game::model_script::~model_script()
{
	delete m;
}
