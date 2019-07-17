#include <fstream>
#include <iostream>
#include <math.h>
#include <algorithm>

#include "Global_Variables.h"
#include "pc.h"
#include "InitShader.h"
#include "Helplers.h"
#include "LoadTexture.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i)) 

pc::pc(const std::string vs, const std::string fs) :_vs(vs), _fs(fs), _M(glm::translate(vec3(0.0f)))
{
	init_shader();
}

pc::~pc()
{
}

void pc::read_from_file(std::string fname)
{
}

void pc::draw(int iteration, glm::mat4 P, glm::mat4 V){
	glUseProgram(_shader_program);
	
	auto gv = Global_Variables::Instance();

	// uniform variables
	glm::mat4 PVM = P * V * _M;
	auto pvm_loc = glGetUniformLocation(_shader_program, "PVM");
	glUniformMatrix4fv(pvm_loc, 1, false, glm::value_ptr(PVM));
	
	auto iter_loc = glGetUniformLocation(_shader_program, "iterations");
	glUniform1i(iter_loc, iteration);

	auto islider_loc = glGetUniformLocation(_shader_program, "int_slider");
	glUniform1i(islider_loc, gv->int_slider);

	islider_loc = glGetUniformLocation(_shader_program, "w");
	glUniform1i(islider_loc, gv->width);

	islider_loc = glGetUniformLocation(_shader_program, "h");
	glUniform1i(islider_loc, gv->height);

	// vertex buffer
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _verts.size() * sizeof(glm::vec3), &_verts[0], GL_DYNAMIC_DRAW);
	glDrawArrays(GL_POINTS, 0, GLsizei(_verts.size()));
	glBindVertexArray(0);
}

bool pc::init_shader()
{
	if (_shader_program != -1)
		glDeleteShader(_shader_program);

	_shader_program = InitShader(_vs.c_str(), _fs.c_str());
	return _shader_program != -1;
}

void pc::reload_shader()
{
	init_shader();
}

void pc::init_buffers()
{
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _verts.size() * sizeof(glm::vec3), &_verts[0], GL_DYNAMIC_DRAW);

	auto pos_loc = glGetAttribLocation(_shader_program, "pos_attrib");
	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(pos_loc);
}

void pc::load_pc(const std::string file, float fract /*= 1.0*/)
{
	_verts.clear();
	auto gv = Global_Variables::Instance();

	// #delete
	if (!gv->is_dbg)
	{
		std::ifstream input(file);
		if (input.is_open())
		{
			std::string line;
			while (std::getline(input, line))
			{
				size_t first_commas = line.find(",");
				size_t second_commas = first_commas + 1 + line.substr(first_commas + 1).find(",");
				float x = std::stof(line.substr(0, first_commas));
				float y = std::stof(line.substr(first_commas + 1, second_commas));
				float z = std::stof(line.substr(second_commas + 1));

				if (random_float() < fract)
					_verts.push_back(glm::vec3{ x, -y, z });
			}
		}
		else
			std::cerr << "cannot open file: " << file;
	}
	else {
		for (int i = 0; i < 10; ++i)
		{
			_verts.push_back(glm::vec3(random_float(), random_float(), random_float()));
		}
	}

	std::cerr << "There are " << _verts.size() << " points \n";

	init_buffers();
}

void pc::load_depth_img(const std::string fname)
{
	// x,y did not change according to h_fov or v_fov yet
	std::shared_ptr<image> depth_img;
	load_image(fname, depth_img);
	
	if (depth_img) {
		_verts.clear();
		_verts.resize(depth_img->_h * depth_img->_w);

		std::cerr << "Point cloud loading begin \n";
		std::cerr << "Width: " << depth_img->_w << " Height: " << depth_img->_h << std::endl;

		float min_z = 10.0f;
		float max_z = -1.0f;
		for (int i = 0; i < depth_img->_h; ++i) {
			for (int j = 0; j < depth_img->_w; ++j) {
				glm::vec3 point;

				// convert disparity value here 
				point.z = (float)1.0 / depth_img->get_pixel(j, i).x;
				point.y = (float)((float)i / depth_img->_h * 2.0 - 1.0) * point.z;
				point.x = (float)((float)j / depth_img->_w * 2.0 - 1.0) * point.z;

				_verts[(depth_img->_h - 1 - i) * depth_img->_w + j] = point;

				min_z = std::min(min_z, point.z);
				max_z = std::max(max_z, point.z);
			}
		}
		std::cerr << "min z" << min_z << " " << "max z: " << max_z << std::endl;
		std::cerr << "Point cloud loading finish \n";
	}
	else {
		std::cerr << "Depth image has problem. " << std::endl;
	}

	init_buffers();
}

void pc::load_pixel(const std::string px_fname)
{
	// #TODO
}

glm::vec3 pc::get_center()
{
	glm::vec3 center(0.0f);
	int num_vertice = int(_verts.size());

	for (auto &v : _verts)
	{
		if (std::isinf(v.x) || std::isinf(v.y) || std::isinf(v.z))
			continue;

		center += v / float(num_vertice);
	}

	return center;
}

void pc::rescale(float fract){
	
	// Animations
	vec3 center = get_center();
	for (auto &v : _verts)
	{
		vec3 offset = v - center;
		offset = offset * fract;
		v = center + offset;
	}
}
