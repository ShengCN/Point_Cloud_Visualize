#include "Helplers.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

std::ostream& PrintGlmVec3(std::ostream& out, glm::vec3 v)
{
	return out << "(" << v.x << "," << v.y << "," << v.z << ")" << std::endl;
}