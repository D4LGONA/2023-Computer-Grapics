#include "stdafx.h"

//random_device rd;
//mt19937 dre(rd());
//
//uniform_real_distribution<float> uidC{ 0.0f, 1.0f }; // 랜덤 컬러 생성
//GLuint vao, vbo[2];

std::random_device rd;
std::mt19937 dre(rd());
std::uniform_real_distribution<float> uidC{ 0.0f, 1.0f };

pair<float, float> WintoOpenGL(POINT pt)
{
	pair<float, float> a = { pt.x / 400. - 1, 1 - (pt.y / 400.) };
	return a;
}

float dist(int x1, int y1, int x2, int y2)
{
	return sqrt(powf(x2 - x1, 2) + powf(y2 - y1, 2));
}