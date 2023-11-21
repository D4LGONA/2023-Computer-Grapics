#include "stdafx.h"

std::random_device rd;
std::mt19937 dre(rd());
std::uniform_real_distribution<float> uidC{ 0.0f, 1.0f };
glm::mat4 proj = glm::mat4(1.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 5.0f); //--- 카메라 위치
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
glm::mat4 view = glm::mat4(1.0f);
glm::vec3 cameraAngle = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 lightPos = { 8.0f, 5.0f, 0.0f };
glm::vec3 lightColor = { 1.0f, 1.0f, 1.0f };
bool light_onf = true;
bool is_ambient = true;

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
