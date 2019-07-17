#pragma once
#include<vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

// point cloud class
class pc
{
public:
	pc(const std::string vs, const std::string fs);
	~pc();

	void draw(int iteration, glm::mat4 P, glm::mat4 V);
	bool init_shader();
	void reload_shader();

	// transform
	glm::vec3 get_center();
	void rescale(float fract);

	// void init_vertices();
	void read_from_file(std::string fname);
	void load_pc(const std::string file, float fract = 1.0);
	void load_depth_img(const std::string fname);
	void load_pixel(const std::string px_fname);
public:
	std::vector<glm::vec3> _verts;
	std::vector<glm::vec3> _colors;

	// opengl drawing 
	glm::mat4 _M;
	GLuint _shader_program;
	GLuint _vao, _vbo;
	std::string _vs, _fs;

// private functions
private:
	void init_buffers();
};

