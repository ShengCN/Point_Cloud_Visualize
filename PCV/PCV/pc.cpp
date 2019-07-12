#include <fstream>
#include <iostream>

#include "Global_Variables.h"
#include "pc.h"
#include "InitShader.h"
#include "Helplers.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i)) 

pc::pc(const std::string vs, const std::string fs):_vs(vs), _fs(fs), _M(glm::translate(vec3(0.0f)))
{
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

	// vertex buffer
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _verts.size() * sizeof(glm::vec3), &_verts[0], GL_DYNAMIC_DRAW);
	glDrawArrays(GL_POINTS, 0, _verts.size());
	glBindVertexArray(0);
}

bool pc::init_shader()
{
	_shader_program = InitShader(_vs.c_str(), _fs.c_str());
	return _shader_program != -1;
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

void pc::load_pc(std::string file, float fract)
{
	_verts.clear();

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
				_verts.push_back(glm::vec3{ x, y, z });		
		}
	}
	else
		std::cerr << "cannot open file: " << file;

	//for (int i = 0; i < 10; ++i)
	//{
	//	_verts.push_back(glm::vec3(random_float(), random_float(), random_float()));
	//}

	std::cerr << "There are " << _verts.size() << " points \n";

	init_buffers();
}

glm::vec3 pc::get_center()
{
	glm::vec3 center(0.0f);
	int num_vertice = _verts.size();

	for (auto &v : _verts)
	{
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
