#pragma once
#include <iostream>
#include <fstream>
#include <string>
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
	glm::vec3 _position;
	glm::vec3 _front;
	glm::vec3 _worldUp;	// default is (0.0f,1.0f,0.0f)
	float fov, aspect;

	PPC(float fov, float aspect);
	~PPC();

	bool save(const std::string file) {
		std::ofstream output(file,std::ofstream::binary);
		if (output.is_open()){
			output.write((char*)&_position[0], sizeof(glm::vec3));
			output.write((char*)&_front[0], sizeof(glm::vec3));
			output.write((char*)&_worldUp[0], sizeof(glm::vec3));
			output.write((char*)&fov, sizeof(float));
			output.write((char*)&aspect, sizeof(float));
			output.close();
			return true;
		}
		else {
			std::cout << "File " << file << " cannot save.\n";
			return false;
		}
	}

	bool load(const std::string file) {
		std::ifstream input(file, std::ifstream::binary);
		if (input.is_open()) {
			input.read((char*)&_position[0], sizeof(glm::vec3));
			input.read((char*)&_front[0], sizeof(glm::vec3));
			input.read((char*)&_worldUp[0], sizeof(glm::vec3));
			input.read((char*)&fov, sizeof(float));
			input.read((char*)&aspect, sizeof(float));
			input.close();
			return true;
		}
		else {
			std::cout << "File " << file << " cannot save.\n";
			return false;
		}
	}

	vec3 GetRight();
	vec3 GetUp();
	vec3 GetViewVec() { return _front; }
	void PositionAndOrient(vec3 p, vec3 lookatP, vec3 up);
	glm::mat4 GetP();
	glm::mat4 GetV();
	void Rotate_Axis(glm::vec3 O, glm::vec3 axis, float angled);	// only rotate ppc position
	void Zoom(float delta);
	void Keyboard(CameraMovement cm, float deltaTime); // keyboard reactions
	void scroll(double delta) {
		fov += float(delta);
	}

	friend std::ostream& operator<<(std::ostream& out, PPC ppc);

	void pan(double deg);
	void tilt(double deg);
	void pitch(double deg);

	void elevate(double deg) {
		vec3 cur_up = GetUp();
		_position += cur_up * float(deg);
	}

private:
	float GetFocal();
};

