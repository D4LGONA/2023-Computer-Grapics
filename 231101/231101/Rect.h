#pragma once
#include "Object.h"

class Rect : public Object
{
	float gravity = -0.2f;
	float speed = 0.0f;

public:
	Rect(const char* c, glm::vec3 s = { 1.0f, 1.0f, 1.0f }, glm::vec3 r = { 0.0f, 0.0f, 0.0f }, glm::vec3 t = { 0.0f, 0.0f, 0.0f })
		: Object(c, s, r, t)
	{}

	void Update() override;

};

