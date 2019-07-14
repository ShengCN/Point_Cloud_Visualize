#include "PPC.h"
#include "Global_Variables.h"
#include <glm/gtx/transform.hpp>
#include <glm/detail/type_mat.hpp>
#include "Helplers.h"
using namespace glm;


float PPC::GetFocal()
{
	auto gv = Global_Variables::Instance();
	return static_cast<float>(gv->width / 2) / tan(fov / 2.0f);
}


PPC::PPC(float _fov, float _aspect) :fov(_fov), aspect(_aspect),
 _position(0.0f, 0.35f, 1.3f), _worldUp(0.0f, 1.0f, 0.0f)
{
}

PPC::~PPC()
{
}

vec3 PPC::GetRight()
{
	vec3 view = GetViewVec();
	
	return cross(view, _worldUp);
}

vec3 PPC::GetUp()
{
	return cross(GetViewVec(), GetRight());
}

void PPC::PositionAndOrient(vec3 p, vec3 lookatP, vec3 up)
{
	_position = p;
	_front = glm::normalize(lookatP - p);
	_worldUp = up;
}

glm::mat4 PPC::GetP()
{
	return glm::perspective(Degree2Radian(fov), aspect, 0.01f, 10000.0f);
}

glm::mat4 PPC::GetV()
{
	return glm::lookAt(_position, _position + _front, _worldUp);
	// return glm::lookAt(_position, target, _worldUp);
}

void PPC::Rotate_Axis(glm::vec3 O, glm::vec3 axis, float angled)
{
	glm::vec3 CO = _position - O;
	auto rot_mat = glm::rotate(Degree2Radian(angled), normalize(axis));
	CO = vec3(rot_mat * vec4(CO, 0.0f));
	_position = CO + O;

}

void PPC::Zoom(float delta)
{
	fov += delta;
	fov = clamp(fov, 10.0f, 150.0f);
}

void PPC::Keyboard(CameraMovement cm, float deltaTime)
{
	auto gv = Global_Variables::Instance();
	float moving_speed = gv->is_speed_up ? 20.0f : 1.0f;
	
	switch (cm)
	{
	case CameraMovement::forward:
		_position += moving_speed * deltaTime * _front;
		break;
	case CameraMovement::backward:

		_position -= moving_speed * deltaTime * _front;
		break;
	case CameraMovement::left:

		_position -= moving_speed * deltaTime * GetRight();
		break;
	case CameraMovement::right:

		_position += moving_speed * deltaTime * GetRight();
		break;
	case CameraMovement::up:

		_position += moving_speed * deltaTime * GetUp();
		break;
	case CameraMovement::down:

		_position -= moving_speed * deltaTime * GetUp();
		break;
	default:
		break;
	}
}

void PPC::pan(double deg)
{
	auto gv = Global_Variables::Instance();
	deg = deg / gv->width * 10.0f;
	//glm::mat4 rot_y = glm::rotate(Degree2Radian(deg), glm::vec3(0.0f, 1.0f, 0.0f));
	//_front = glm::vec3(rot_y * glm::vec4(_front, 0.0f));
	vec3 right = GetRight();
	_front += right * float(deg);
	_front = glm::normalize(_front);
}

void PPC::tilt(double deg)
{
	// #TODO
}

void PPC::pitch(double deg)
{
	auto gv = Global_Variables::Instance();
	deg = deg / gv->height * 10.0f;
	//glm::mat4 rot_x = glm::rotate(Degree2Radian(deg), glm::vec3(1.0f, 0.0f, 0.0f));
	//_front = glm::vec3(rot_x * glm::vec4(_front, 0.0f));
	vec3 up = GetUp();
	_front += up * float(deg);
	_front = glm::normalize(_front);
}

std::ostream& operator<<(std::ostream& out, PPC ppc)
{
	out << "Camera Position: ";
	PrintGlmVec3(out, ppc._position);
	out << "Camera Front: ";
	
	PrintGlmVec3(out, ppc._front);
	return out;
}
