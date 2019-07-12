#include "pcv_window.h"

pcv_window::pcv_window(){
}


pcv_window::~pcv_window(){
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

		draw_gui();		

		// do draw calls here
		if (_cur_scene)
			_cur_scene->draw();

		// update global varialbes
		gv->delta_time = glfwGetTime() - gv->last_time;
		gv->last_time = glfwGetTime();

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
		if (gv->_cur_scene && gv->_cur_scene->_pcs.size() > 0)
		{
			gv->_cur_scene->_pcs[0]->rescale(gv->scale_factor);
		}
	}
	ImGui::SliderInt("Int Slider", &gv->int_slider, 0, 10);
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
