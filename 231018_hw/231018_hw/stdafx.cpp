#include "stdafx.h"

std::random_device rd;
std::mt19937 dre(rd());
std::uniform_real_distribution<float> uidC{ 0.0f, 1.0f };
std::uniform_real_distribution<float> uidD{ -0.25f, 0.25f }; // 너무 작은 수는 안나오게...
std::uniform_real_distribution<float> uidA{1.0f, 10.0f}; // 회전 각도
std::uniform_real_distribution<float> uidY{-0.2f, 0.8f}; // y좌표 값
std::uniform_int_distribution<int> uidS{ 3, 8 }; // 꼭짓점 개수
int score = 0;
bool isSolid = true;

pair<float, float> WintoOpenGL(POINT pt)
{
	pair<float, float> a = { pt.x / 400. - 1, 1 - (pt.y / 400.) };
	return a;
}

int ccw(glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
	float rot = (B.x - A.x) * (C.y - A.y) - (C.x - A.x) * (B.y - A.y);

	if (rot > 0) return 1;       //0보다 크면 좌회전
	else if (rot < 0) return -1; //0보다 작으면 우회전
	else return 0;
}

float dist(glm::vec3 p1, glm::vec3 p2)
{
	return sqrt(powf(p2.x - p1.x, 2) + powf(p2.y - p1.y, 2) + powf(p2.z - p1.z, 2));
}


