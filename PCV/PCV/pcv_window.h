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
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);

		auto gv = Global_Variables::Instance();
		if (key == GLFW_KEY_W){
			// move back
			std::cerr << "Pressed w \n";
			gv->cur_ppc->Keyboard(CameraMovement::forward, gv->delta_time);
		}
		if (key == GLFW_KEY_A) {
			// move back
			std::cerr << "Pressed a \n";
			gv->cur_ppc->Keyboard(CameraMovement::left, gv->delta_time);
		}
		if (key == GLFW_KEY_S) {
			// move back
			std::cerr << "Pressed s \n";
			gv->cur_ppc->Keyboard(CameraMovement::backward, gv->delta_time);
		}
		if (key == GLFW_KEY_D) {
			// move back
			std::cerr << "Pressed d \n";
			gv->cur_ppc->Keyboard(CameraMovement::right, gv->delta_time);
		}
	}

	static void error_callback(int error, const char* description){
		std::cerr << "Error: %s\n" << description << std::endl;
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
};

