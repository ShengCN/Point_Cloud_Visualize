#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <imgui/imgui.h>
#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>
#include <vector>
#include "Global_Variables.h"
#include "pcv_scene.h"

class pcv_window
{
public:
	pcv_window();
	~pcv_window();

// public variables
public:
	GLFWwindow* _window;

// callback functions
public:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	//static void mouse_callback()
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
		if (ImGui::IsMouseHoveringAnyWindow())
			return;

		auto gv = Global_Variables::Instance();
		gv->cur_ppc->scroll(yoffset);
	}

	static void error_callback(int error, const char* description){
		std::cerr << "Error: %s\n" << description << std::endl;
	}

	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
		if (ImGui::IsMouseHoveringAnyWindow())
			return;

		auto gv = Global_Variables::Instance();

		if (glfwGetMouseButton(window, 0) == GLFW_PRESS){
			gv->cur_ppc->pan(xpos - gv->mouse_last_x);
			gv->cur_ppc->pitch(ypos - gv->mouse_last_y);
		}

		gv->mouse_last_x = xpos;
		gv->mouse_last_y = ypos;
	}

// public functions
public:
	int create_window(int w, int h, const std::string title);
	void show();	// one thread one window
	void draw_scene(std::shared_ptr<pcv_scene> scene) { _cur_scene = scene; };

private:
	void init_gui();
	void draw_gui();
	std::shared_ptr<pcv_scene> _cur_scene;

// GUI functions
private:
};

