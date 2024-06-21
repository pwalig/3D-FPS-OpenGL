#include <renderer.h>
#include "graphics_menu.h"
#include <glm/ext/matrix_transform.hpp>
#include <scripts_system.h>

game::graphics_menu* game::graphics_menu::instance = nullptr;
bool game::graphics_menu::vsynch_enabled = true;

game::graphics_menu::graphics_menu(const std::function<void()>& on_close) :
	title("GRAPHICS", "../assets/fonts/bitmap/handwiriting-readable.png",
	glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(800.0f, 1080.0f, -10.0f)), glm::vec3(30.0f, 50.0f, 1.0f))),
	fov_text("FIELD OF VIEW : " + std::to_string(renderer::global_fov), "../assets/fonts/bitmap/handwiriting-readable.png",
	glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(800.0f, 1000.0f, -10.0f)), glm::vec3(17.0f, 30.0f, 1.0f))),
	vsynch_text("VSYNCH", "../assets/fonts/bitmap/handwiriting-readable.png",
	glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(800.0f, 850.0f, -10.0f)), glm::vec3(17.0f, 30.0f, 1.0f))),
	minimization_text("MINIMIZATION", "../assets/fonts/bitmap/handwiriting-readable.png",
	glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(800.0f, 775.0f, -10.0f)), glm::vec3(17.0f, 30.0f, 1.0f))),
	magnification_text("MAGNIFICATION", "../assets/fonts/bitmap/handwiriting-readable.png",
	glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(800.0f, 625.0f, -10.0f)), glm::vec3(17.0f, 30.0f, 1.0f))),
	anisotropy_text("ANISOTROPY : ", "../assets/fonts/bitmap/handwiriting-readable.png",
	glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(800.0f, 475.0f, -10.0f)), glm::vec3(17.0f, 30.0f, 1.0f))),
	fov(glm::vec3(960.0f, 925.0f, -10.0f), glm::vec2(120.0f, 20.0f)),
	vsynch(glm::vec3(1050.0f, 850.0f, -10.0f), glm::vec2(15.0f, 15.0f)),
	minimization(glm::vec3(960.0f, 700.0f, -8.0f), glm::vec2(120.0f, 32.0f)),
	magnification(glm::vec3(960.0f, 550.0f, -9.0f), glm::vec2(120.0f, 32.0f)),
	anisotropy(glm::vec3(960.0f, 400.0f, -10.0f), glm::vec2(120.0f, 32.0f)),
	back(glm::vec3(960.0f, 100.0f, -10.0f), glm::vec2(75.0f, 32.0f), "../assets/textures/White_Square.png", "BACK", "../assets/fonts/bitmap/handwiriting-readable.png")
{
	// singleton stuff
	if (game::graphics_menu::instance) {
		printf("soft error: attempt to create second graphics menu\n");
		delete this;
		return;
	}
	game::graphics_menu::instance = this;

	// SET VALUES
	fov.value = (renderer::global_fov - 1.0f) / 168.0f;
	fov.update_visual();
	vsynch.update_check(vsynch_enabled);
	float maxAnisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
	anisotropy_text.text += std::to_string(maxAnisotropy);
	anisotropy.value = (maxAnisotropy - 1.0f) / 15.0f;
	anisotropy.update_visual();

	// FUNCTION
	fov.on_value_changed = [this](float new_fov) {
		renderer::global_fov = (new_fov * 168.0f) + 1.0f;
		this->fov_text.text = "FIELD OF VIEW: " + std::to_string(renderer::global_fov);
		};
	vsynch.on_click.subscribe([this]() {
		vsynch_enabled = this->vsynch.get_value();
		glfwSwapInterval(vsynch_enabled ? 1 : 0);
		});
	minimization.values.push_back("NEAREST");
	minimization.values.push_back("LINEAR");
	minimization.values.push_back("NEAREST MIPMAP NEAREST");
	minimization.values.push_back("NEAREST MIPMAP LINEAR");
	minimization.values.push_back("LINEAR MIPMAP NEAREST");
	minimization.values.push_back("LINEAR MIPMAP LINEAR");
	magnification.values.push_back("NEAREST");
	magnification.values.push_back("LINEAR");
	minimization.on_value_changed = [](std::string filter) {
		if (filter == "LINEAR") glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		else if (filter == "NEAREST") glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		else if (filter == "NEAREST NEAREST") glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		else if (filter == "NEAREST LINEAR") glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		else if (filter == "LINEAR NEAREST") glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		else if (filter == "LINEAR LINEAR") glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		printf("min %s set\n", filter.c_str());
		};
	magnification.on_value_changed = [](std::string filter) {
		if (filter == "LINEAR") glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		else if (filter == "NEAREST") glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		printf("mag %s set\n", filter.c_str());
		};
	anisotropy.on_value_changed = [this](float new_anisotropy) {
		new_anisotropy *= 15.0f;
		new_anisotropy += 1.0f;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, new_anisotropy);
		anisotropy_text.text = "ANISOTROPY : " + std::to_string(new_anisotropy);
		};
	this->back.on_click.subscribe([]() { scripts_system::safe_destroy(game::graphics_menu::instance); });
	this->back.on_click.subscribe(on_close);
}

game::graphics_menu::~graphics_menu()
{
	if (instance == this) instance = nullptr;
}
