#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include "pc.h"

class pcv_scene
{
public:
	pcv_scene() = default;
	~pcv_scene();

	void add_pc(std::shared_ptr<pc> pc) { if (pc) _pcs.push_back(pc); };
	void load_scene(const std::string scene_fname);
	
	
	void setup_scene(); // set up camera
	void draw();		// called by window

// scene resources
private:
	std::vector<std::shared_ptr<pc>> _pcs;
	int _iteration = 0;
};

