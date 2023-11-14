#pragma once
#include "Object.h"

class Rect : public Object
{
	float gravity = -0.2f;

public:
	float accel = 0.05f;
	bool isStop = false;
	glm::vec3 dest = { 0.0f, 0.0f, 0.0f };
	glm::vec3 speed = { 0.0f, 0.0f, 0.0f };
	glm::vec3 dir = { 0.0f, 0.0f, 0.0f };
	Rect(const char* c, glm::vec3 s = { 1.0f, 1.0f, 1.0f }, glm::vec3 r = { 0.0f, 0.0f, 0.0f }, glm::vec3 t = { 0.0f, 0.0f, 0.0f })
		: Object(c, s, r, t)
	{}

	void Update() override;
	void MoveRect(); // y축으로만 이동하는 녀석
	void MoveRectSlide(glm::vec3 dest);
};
