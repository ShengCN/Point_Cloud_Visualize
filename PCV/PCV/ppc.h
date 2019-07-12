#pragma once
#include <iostream>

#include <glm/gtc/type_ptr.hpp>
/*
 *
 *	Planer Pinhole camera.
 *	Provide basic controls.
 *
 */
using glm::vec3;

enum class CameraMovement
{
	forward,
	backward,
	left,
	right,
	up,
	down
};

class PPC
{
public:
	glm::vec3 position, target;
	glm::vec3 front;
	glm::vec3 worldUp;	// default is (0.0f,1.0f,0.0f)
	float movingSpeed = 10.0;
	float fov, aspect;

	PPC(float fov, float aspect);
	~PPC();

	vec3 GetRight();
	vec3 GetUp();
	vec3 GetViewVec() { return target - position; }
	void PositionAndOrient(vec3 p, vec3 lookatP, vec3 up);
	void UpdatePPC();
	glm::mat4 GetP();
	glm::mat4 GetV();
	void Rotate_Axis(glm::vec3 O, glm::vec3 axis, float angled);
	void Zoom(float delta);
	void Keyboard(CameraMovement cm, float deltaTime); // keyboard reactions
	friend std::ostream& operator<<(std::ostream& out, PPC ppc);

private:
	float GetFocal();
};

