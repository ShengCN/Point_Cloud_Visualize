#pragma once
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <filesystem>

std::ostream& PrintGlmVec3(std::ostream& out, glm::vec3 v);

inline std::ostream& operator<<(std::ostream& out, glm::vec3 v) {
	out << v.x << "," << v.y << "," << v.z;
	return out;
}

template<typename T>
inline float Degree2Radian(T d)
{
	return static_cast<float>((d / 180.0f) * 3.1415926);
}

template<typename T>
inline float Radian2Degree(T r)
{
	return static_cast<float>((r / 3.14159265) * 180.0f);
}

inline bool FloatEqual(float a, float b)
{
	return abs(a - b) < 1e-6;
}

inline float random_float(float min=0.0, float max=1.0) {
	// engine
	std::random_device rd;
	std::mt19937 engine(rd());
	std::uniform_real_distribution<float> u(min, max);
	return u(engine);
}

inline std::vector<std::string> get_files(const std::string folder) {
	std::vector<std::string> files;
	for (const auto& entry : std::filesystem::directory_iterator(folder)) {
		files.push_back(entry.path().string());
	}
	return files;
}
