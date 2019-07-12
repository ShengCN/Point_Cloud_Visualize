#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <imgui/imgui.h>
#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>
#include "Global_Variables.h"

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
	}

	static void error_callback(int error, const char* description){
		std::cerr << "Error: %s\n" << description << std::endl;
	}

// public functions
public:
	int create_window(int w, int h, const std::string title);
	void show();	// one thread one window

private:
	void draw_gui() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// ------------------------ Window ------------------------ //
		ImGui::Begin("PC control");
		auto gv = Global_Variables::Instance();
		ImGui::SliderFloat("View angle", &gv->rotation, -180.0f, +180.0f);
		ImGui::SliderFloat("Size", &gv->scale_factor, 0.0f, 1.0f);
		ImGui::End();

		// static bool show_test = false;
		// ImGui::ShowTestWindow(&show_test);
		ImGui::Render();

		int display_w, display_h;
		glfwGetFramebufferSize(_window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(gv->default_color.x, gv->default_color.y, gv->default_color.z, gv->default_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
};

