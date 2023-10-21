#include "stdafx.h"

std::random_device rd;
std::mt19937 dre(rd());
std::uniform_real_distribution<float> uidC{ 0.0f, 1.0f };
std::uniform_real_distribution<float> uidD{ -0.25f, 0.25f }; // �ʹ� ���� ���� �ȳ�����...
std::uniform_int_distribution<int> uidS{ 3, 8 }; // ������ ����
std::uniform_int_distribution<int> uidY{200, 600}; // y��ǥ ��
int score = 0;
bool isSolid = true;

pair<float, float> WintoOpenGL(POINT pt)
{
	pair<float, float> a = { pt.x / 400. - 1, 1 - (pt.y / 400.) };
	return a;
}

int ccw(POINT A, POINT B, POINT C)
{
	float rot = (B.x - A.x) * (C.y - A.y) - (C.x - A.x) * (B.y - A.y);

	if (rot > 0) return 1;       //0���� ũ�� ��ȸ��
	else if (rot < 0) return -1; //0���� ������ ��ȸ��
	else return 0;
}

float dist(POINT p1, POINT p2)
{
	return sqrt(powf(p2.x - p1.x, 2) + powf(p2.y - p1.y, 2));
}


