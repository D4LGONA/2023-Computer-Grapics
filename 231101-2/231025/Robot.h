#pragma once
#include "object.h"

class Robot
{
	vector<Object> parts;
	int walkcount = 0;
	bool count_up = true;
	glm::vec3 speed = { 0.0f, 0.0f, 0.0f };
	float gravity = -0.2f;
public:
	glm::vec3 dir = { 0.0f, 0.0f, 0.0f };
	Robot(glm::vec3 startpt, float scale);
	glm::vec3 origin = { 0.0f, 0.0f, 0.0f };

	void RotationY(float angle);

	void Update();
	void Render(GLuint);
	void Walk(bool arr[]);
	void Move();
	void Move(int n, float s);
	void Jump();
	BB* GetBB(int n) { return parts[n].bb; }
};

