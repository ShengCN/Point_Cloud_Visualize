#ifndef __LOADTEXTURE_H__
#define __LOADTEXTURE_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <memory>
#include <glm/gtc/type_ptr.hpp>

struct image
{
	int _w, _h, _c;
	std::vector<unsigned int> _data;
	
	image(int w, int h, int c) :_w(w), _h(h), _c(c) { _data = std::vector<unsigned int>(_w * _h * _c); }

	glm::vec4 get_pixel(int wi, int hi) {
		unsigned int color = _data[(_h - 1 - hi) * _w + wi];
		glm::vec4 c;
		c.x = (float)(((char*)&color)[0] / 255.0);
		c.y = (float)(((char*)&color)[1] / 255.0);
		c.z = (float)(((char*)&color)[2] / 255.0);
		c.a = (float)(((char*)&color)[3] / 255.0);

		return c;
	}
};

GLuint LoadTexture(const std::string& fname);
void load_image(const std::string fname, std::shared_ptr<image>& img_ptr);

#endif