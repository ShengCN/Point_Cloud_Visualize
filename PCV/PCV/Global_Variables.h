#pragma once
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "pcv_scene.h"
#include "pc.h"
#include "ppc.h"

using std::shared_ptr;
using std::vector;
using std::unordered_map;

class Global_Variables
{
public:
	static Global_Variables* Instance();

	// IMGUI
	float rotation = 0.0f;
	float scale_factor = 1.0f;
	int int_slider = 1;

	std::string title = "Adobe_PCV";
	int width = 1080;
	int height = 720;;
	int iteration = 0;
	bool is_update = true;

	// camera
	float fov = 50.0f;
	double last_time, delta_time;
	std::shared_ptr<PPC> cur_ppc;

	const std::string pc_file = "test.txt";
	const std::string vertex_shader = "point_vs.glsl";
	const std::string fragment_shader = "point_fs.glsl";
	glm::vec4 error_color = { 1.0f,0.0f,0.0f,1.0f };
	glm::vec4 default_color = { 0.8f,0.8f, 0.8f, 1.0f };

	// scene management
	std::shared_ptr<pcv_scene> _cur_scene;

private:
	static Global_Variables* m_instance;

	Global_Variables();
	~Global_Variables();
	Global_Variables& operator=(Global_Variables const&) const { return *m_instance; }
};