#include "stdafx.h"
#include "Object.h"

std::random_device rd;
std::mt19937 dre(rd());
std::uniform_real_distribution<float> uidC{ 0.0f, 1.0f };
glm::mat4 proj = glm::mat4(1.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 50.0f); //--- ī�޶� ��ġ
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- ī�޶� �ٶ󺸴� ����
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- ī�޶� ���� ����
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

//bool aabb(Object& a, Object& b)
//{
//	glm::vec3 a_max = a.origin + a.T + a.S;
//	glm::vec3 a_min = a.origin + a.T + (-a.S);
//	glm::vec3 b_max = b.origin + b.T + b.S;
//	glm::vec3 b_min = b.origin + b.T + (-b.S);
//
//	if (a_max.x < b_min.x || a_min.x > b_max.x) {
//		return false; // X �࿡�� �浹���� ����
//	}
//	if (a_max.y < b_min.y || a_min.y > b_max.y) {
//		return false; // Y �࿡�� �浹���� ����
//	}
//	if (a_max.z < b_min.z || a_min.z > b_max.z) {
//		return false; // Z �࿡�� �浹���� ����
//	}
//
//	return true;
//}

bool obb(BB& a, BB& b)
{
	return false;
}
