#include "pcv_window.h"
#include "Helplers.h"
#include "LoadTexture.h"

pcv_window::pcv_window(){
}


pcv_window::~pcv_window(){
}

void pcv_window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	auto gv = Global_Variables::Instance();
	if (key == GLFW_KEY_W) {
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
	if (key == GLFW_KEY_Q) {
		gv->cur_ppc->elevate(gv->delta_time);
	}
	if (key == GLFW_KEY_E) {
		gv->cur_ppc->elevate(-gv->delta_time);
	}
	if (key == GLFW_KEY_P) {
		if (gv->cur_ppc->save(gv->default_ppc_file))
			std::cerr << "PPC file saved. \n";
	}
	if (key == GLFW_KEY_LEFT_SHIFT)
	{
		if (action == GLFW_PRESS)
		{
			gv->is_speed_up = true;
			std::cerr << "Speed up \n";
		}
		
		if (action == GLFW_RELEASE)
		{
			gv->is_speed_up = false;
			std::cerr << "Speed down \n";
		}
	}
}

int pcv_window::create_window(int w, int h, const std::string title)
{
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	/* Create a windowed mode window and its OpenGL context */
	_window = glfwCreateWindow(w, h, title.c_str(), NULL, NULL);
	if (!_window)
	{
		glfwTerminate();
		return -1;
	}
	
	// callbacks
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(_window, key_callback);
	glfwSetScrollCallback(_window, scroll_callback);
	glfwSetCursorPosCallback(_window, cursor_position_callback);

	// set up environment
	glfwMakeContextCurrent(_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);
	if (GLVersion.major < 2) {
		printf("Your system doesn't support OpenGL >= 2!\n");
		return -1;
	}

	glfwSwapInterval(1);

	init_gui();

	return 0;
}

void pcv_window::show()
{
	auto gv = Global_Variables::Instance();
	glfwMakeContextCurrent(_window);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(_window))
	{
		glfwPollEvents();
		
		int display_w, display_h;
		glfwGetFramebufferSize(_window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(gv->default_color.x, gv->default_color.y, gv->default_color.z, gv->default_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		// do draw calls here
		if (_cur_scene)
			_cur_scene->draw();
		
		draw_gui();

		// update global variables
		gv->delta_time = float(glfwGetTime() - gv->last_time);
		gv->last_time = float(glfwGetTime());

		glfwSwapBuffers(_window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}


void pcv_window::init_gui()
{
	// imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(_window, true);
	const char* glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version);
}

// #draw_gui
void pcv_window::draw_gui()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// ------------------------ Window ------------------------ //
	ImGui::Begin("PC control");
	auto gv = Global_Variables::Instance();
	ImGui::SliderFloat("View angle", &gv->rotation, -180.0f, +180.0f);
	ImGui::SliderFloat("Size", &gv->scale_factor, 0.0f, 2.0f);
	ImGui::SameLine();
	if (ImGui::Button("Rescale")){
		if (gv->cur_scene && gv->cur_scene->_pcs.size() > 0)
		{
			gv->cur_scene->_pcs[0]->rescale(gv->scale_factor);
		}
	}
	ImGui::SliderInt("Int Slider", &gv->int_slider, 0, 10);
	
	if (ImGui::Button("Reload")) {
		gv->cur_scene->reload_shaders();
	}

	// select files
	auto files = get_files(gv->depth_image_folder);
	static int selected = -1;
	for (int n = 0; n < files.size(); n++)
	{
		if (ImGui::Selectable(files[n].c_str(), selected == n))
			selected = n;
	}
	if (ImGui::Button("Show Depth")) {
		if (selected != -1) {
			std::cerr << "Visualize " << files[selected] << std::endl;
			
			gv->cur_scene->add_pc(files[selected]);
			gv->cur_scene->setup_scene();
		}
		else {
			std::cerr << "Please select a file \n";
		}
	}

	if (selected != -1) {
		static GLuint tex_id = -1;
		if (tex_id == -1) {
			tex_id = LoadTexture(files[selected]);
		}
		ImGui::Image((ImTextureID)tex_id, ImVec2(200, 150), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
	}
	ImGui::End();

	// ImGui::ShowTestWindow();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
