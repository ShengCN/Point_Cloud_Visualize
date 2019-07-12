#include <fstream>
#include <iostream>


#include "pc.h"
#include "InitShader.h"
#include "Helplers.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i)) 

pc::pc(const std::string vs, const std::string fs):_vs(vs), _fs(fs)
{
}

pc::~pc()
{
}

void pc::read_from_file(std::string fname)
{
}

void pc::draw(int iteration, glm::mat4 P, glm::mat4 V)
{
	glUseProgram(_shader_program);
	glm::mat4 PVM = P * V * _M;
	auto pvm_loc = glGetUniformLocation(_shader_program, "PVM");
	glUniformMatrix4fv(pvm_loc, 1, false, glm::value_ptr(PVM));
	
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
	glBindVertexArray(_vao);
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _verts.size() * sizeof(glm::vec3), &_verts[0], GL_DYNAMIC_DRAW);

	auto pos_loc = glGetAttribLocation(_shader_program, "pos_attrib");
	glEnableVertexAttribArray(pos_loc);
	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), BUFFER_OFFSET(0));
}

void pc::load_pc(std::string file, float fract)
{
	_verts.clear();

	//std::ifstream input(file);
	//if (input.is_open())
	//{
	//	std::string line;
	//	while (std::getline(input, line))
	//	{
	//		size_t first_commas = line.find(",");
	//		size_t second_commas = first_commas + 1 + line.substr(first_commas + 1).find(",");
	//		float x = std::stof(line.substr(0, first_commas));
	//		float y = std::stof(line.substr(first_commas + 1, second_commas));
	//		float z = std::stof(line.substr(second_commas + 1));

	//		if(random_float() < fract)
	//			_verts.push_back(glm::vec3(x, y, z));
	//	}
	//}
	//else
	//	std::cerr << "Cannot open file: " << file;

	_verts.push_back(glm::vec3(0.0f,0.0f,0.0f));

	init_buffers();
}

glm::vec3 pc::center()
{
	glm::vec3 center(0.0f);
	int num_vertice = _verts.size();

	for (auto &v : _verts)
	{
		center += v / float(num_vertice);
	}

	return center;
}
