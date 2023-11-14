#pragma once
#include "Object.h"

class Sphere : public Object
{
	float gravity = -0.2f;

public:
	glm::vec3 dir = { 0.0f, 0.0f, 0.0f };
	Sphere(const char* c, glm::vec3 s = { 1.0f, 1.0f, 1.0f }, glm::vec3 r = { 0.0f, 0.0f, 0.0f }, glm::vec3 t = { 0.0f, 0.0f, 0.0f }, glm::vec3 dir = {0.0f, 1.0f, 0.0f})
		: Object(c, s, r, t), dir{dir}
	{}

	bool ischecked = false;
	void Update() override;
	void InverseDir(int n) { dir[n] *= -1.0f; }
};
