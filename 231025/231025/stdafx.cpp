#include "stdafx.h"
#include "object.h"

std::random_device rd;
std::mt19937 dre(rd());
std::uniform_real_distribution<float> uidC{ 0.0f, 1.0f };
glm::mat4 proj = glm::mat4(1.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f); //--- 카메라 위치
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
glm::mat4 view = glm::mat4(1.0f);
glm::vec3 cameraAngle = { 0.0f, 0.0f, 0.0f };

pair<float, float> WintoOpenGL(POINT pt)
{
	pair<float, float> a = { pt.x / 400. - 1, 1 - (pt.y / 400.) };
	return a;
}

float dist(POINT p1, POINT p2)
{
	return sqrt(powf(p2.x - p1.x, 2) + powf(p2.y - p1.y, 2));
}

float dist(glm::vec3 p1, glm::vec3 p2)
{
	return sqrt(powf(p2.x - p1.x, 2) + powf(p2.y - p1.y, 2) + powf(p2.z - p1.z, 2));
}

bool aabb(object& a, object& b)
{
	glm::vec3 a_max = a.Origin + a.transition + a.scale;
	glm::vec3 a_min = a.Origin + a.transition + (-a.scale);
	glm::vec3 b_max = b.Origin + b.transition + b.scale;
	glm::vec3 b_min = b.Origin + b.transition + (-b.scale);

	if (a_max.x < b_min.x || a_min.x > b_max.x) {
		return false; // X 축에서 충돌하지 않음
	}
	if (a_max.y < b_min.y || a_min.y > b_max.y) {
		return false; // Y 축에서 충돌하지 않음
	}
	if (a_max.z < b_min.z || a_min.z > b_max.z) {
		return false; // Z 축에서 충돌하지 않음
	}

	return true;
}
