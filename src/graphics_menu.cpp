#include <renderer.h>
#include <engine.h>
#include "graphics_menu.h"
#include <glm/ext/matrix_transform.hpp>
#include <scripts_system.h>
#include "gameplay_manager.h"

game::graphics_menu* game::graphics_menu::instance = nullptr;
bool game::graphics_menu::vsynch_enabled = true;
unsigned int game::graphics_menu::framerate_cap = 250;
bool game::graphics_menu::framerate_cap_enable = false;

game::graphics_menu::graphics_menu(const std::function<void()>& on_close) :
	title("GRAPHICS", "../assets/fonts/bitmap/handwiriting-readable.png",
		glm::vec3(0.5f, 1.0f, 0.02f), glm::scale(glm::mat4(1.0f), glm::vec3(0.03f, 0.05f, 1.0f))),
	fov_text("FIELD OF VIEW : " + std::to_string(renderer::global_fov), "../assets/fonts/bitmap/handwiriting-readable.png",
		glm::vec3(0.5f, 0.95f, 0.02f), glm::scale(glm::mat4(1.0f), glm::vec3(0.017f, 0.03f, 1.0f))),
	framerate_text("FRAMERATE CAP : " + std::to_string(framerate_cap), "../assets/fonts/bitmap/handwiriting-readable.png",
		glm::vec3(0.5f, 0.85f, 0.02f), glm::scale(glm::mat4(1.0f), glm::vec3(0.017f, 0.03f, 1.0f))),
	vsynch_text("VSYNCH", "../assets/fonts/bitmap/handwiriting-readable.png",
		glm::vec3(0.5f, 0.75f, 0.02f), glm::scale(glm::mat4(1.0f), glm::vec3(0.017f, 0.03f, 1.0f))),
	fullscreen_text("FULLSCREEN", "../assets/fonts/bitmap/handwiriting-readable.png",
		glm::vec3(0.5f, 0.7f, 0.02f), glm::scale(glm::mat4(1.0f), glm::vec3(0.017f, 0.03f, 1.0f))),
	minimization_text("MINIMIZATION", "../assets/fonts/bitmap/handwiriting-readable.png",
		glm::vec3(0.5f, 0.65f, 0.02f), glm::scale(glm::mat4(1.0f), glm::vec3(0.017f, 0.03f, 1.0f))),
	magnification_text("MAGNIFICATION", "../assets/fonts/bitmap/handwiriting-readable.png",
		glm::vec3(0.5f, 0.5f, 0.02f), glm::scale(glm::mat4(1.0f), glm::vec3(0.017f, 0.03f, 1.0f))),
	anisotropy_text("ANISOTROPY : ", "../assets/fonts/bitmap/handwiriting-readable.png",
		glm::vec3(0.5f, 0.35f, 0.02f), glm::scale(glm::mat4(1.0f), glm::vec3(0.017f, 0.03f, 1.0f))),
	fov(glm::vec3(0.5f, 0.9f, 0.02f), glm::vec2(0.12f, 0.02f)),
	framerate(glm::vec3(0.5f, 0.8f, 0.02f), glm::vec2(0.12f, 0.02f)),
	framerate_enable(glm::vec3(0.7f, 0.8f, 0.02f), glm::vec2(0.015f, 0.015f)),
	vsynch(glm::vec3(0.7f, 0.75f, 0.02f), glm::vec2(0.015f, 0.015f)),
	fullscreen(glm::vec3(0.7f, 0.7f, 0.02f), glm::vec2(0.015f, 0.015f)),
	minimization(glm::vec3(0.5f, 0.6f, 0.018f), glm::vec2(0.12f, 0.02f)),
	magnification(glm::vec3(0.5f, 0.45f, 0.016f), glm::vec2(0.12f, 0.02f)),
	anisotropy(glm::vec3(0.5f, 0.3f, 0.02f), glm::vec2(0.12f, 0.02f)),
	back(glm::vec3(0.5f, 0.1f, 0.02f), glm::vec2(0.07f, 0.03f), "../assets/textures/White_Square.png", "BACK", "../assets/fonts/bitmap/handwiriting-readable.png")
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
	framerate.value = std::sqrt((framerate_cap - 30.0f) / 470.0f);
	framerate.update_visual();
	framerate_enable.update_check(framerate_cap_enable);
	vsynch.update_check(vsynch_enabled);
	fullscreen.update_check(engine::is_fullscreen);
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
	framerate.on_value_changed = [this](float new_framerate) {
		framerate_cap = (new_framerate * new_framerate * 470.0f) + 30.0f;
		this->framerate_text.text = "FRAMERATE CAP: " + std::to_string(framerate_cap);
		};
	framerate_enable.on_click.subscribe([this]() {
		framerate_cap_enable = this->framerate_enable.get_value();
		});
	vsynch.on_click.subscribe([this]() {
		vsynch_enabled = this->vsynch.get_value();
		glfwSwapInterval(vsynch_enabled ? 1 : 0);
		});
	fullscreen.on_click.subscribe([this]() {
		game::gameplay_manager::full_screen();
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
