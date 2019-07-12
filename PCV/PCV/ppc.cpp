#include "PPC.h"
#include "Global_Variables.h"
#include <glm/gtx/transform.hpp>
#include <glm/detail/type_mat.hpp>
#include "Helplers.h"
using namespace glm;

void PPC::UpdatePPC()
{
	// default camera parameters.
	front = glm::normalize(target - position);
}

float PPC::GetFocal()
{
	auto gv = Global_Variables::Instance();
	return static_cast<float>(gv->width / 2) / tan(fov / 2.0f);
}


PPC::PPC(float _fov, float _aspect) :fov(_fov), aspect(_aspect),
target(0.0f, 0.0f, 0.0f), position(0.0f, 0.35f, 1.3f), worldUp(0.0f, 1.0f, 0.0f)
{
	UpdatePPC();
}

PPC::~PPC()
{
}

vec3 PPC::GetRight()
{
	vec3 view = GetViewVec();
	return cross(view, worldUp);
}

vec3 PPC::GetUp()
{
	return cross(GetViewVec(), GetRight());
}

void PPC::PositionAndOrient(vec3 p, vec3 lookatP, vec3 up)
{
	position = p;
	target = lookatP;
	worldUp = up;
	UpdatePPC();
}

glm::mat4 PPC::GetP()
{
	return glm::perspective(Degree2Radian(fov), aspect, 0.1f, 10000.0f);
}

glm::mat4 PPC::GetV()
{
	// return glm::lookAt(position, position + front, worldUp);
	return glm::lookAt(position, target, worldUp);
}

void PPC::Rotate_Axis(glm::vec3 O, glm::vec3 axis, float angled)
{
	glm::vec3 CO = position - O;
	auto rot_mat = glm::rotate(Degree2Radian(angled), normalize(axis));
	CO = vec3(rot_mat * vec4(CO, 0.0f));
	position = CO + O;
	target = O;

	UpdatePPC();
}

void PPC::Zoom(float delta)
{
	fov += delta;
	fov = clamp(fov, 10.0f, 150.0f);
}

void PPC::Keyboard(CameraMovement cm, float deltaTime)
{
	auto gv = Global_Variables::Instance();

	switch (cm)
	{
	case CameraMovement::forward:
		position += movingSpeed * deltaTime * front;
		break;
	case CameraMovement::backward:

		position -= movingSpeed * deltaTime * front;
		break;
	case CameraMovement::left:

		position -= movingSpeed * deltaTime * GetRight();
		break;
	case CameraMovement::right:

		position += movingSpeed * deltaTime * GetRight();
		break;
	case CameraMovement::up:

		position += movingSpeed * deltaTime * GetUp();
		break;
	case CameraMovement::down:

		position -= movingSpeed * deltaTime * GetUp();
		break;
	default:
		break;
	}
	UpdatePPC();
}

std::ostream& operator<<(std::ostream& out, PPC ppc)
{
	out << "Camera Position: ";
	PrintGlmVec3(out, ppc.position);
	out << "Camera Front: ";
	
	PrintGlmVec3(out, ppc.front);
	return out;
}
