#pragma once

#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <algorithm>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
using namespace std;

struct vertex
{
	vector<glm::vec3> pt;
	vector<glm::vec3> c;
};

extern std::random_device rd;
extern std::mt19937 dre;
extern std::uniform_real_distribution<float> uidC;
extern std::uniform_real_distribution<float> uidD;
extern std::uniform_int_distribution<int> uidS;
extern std::uniform_int_distribution<int> uidY;
extern int score;
extern bool isSolid;

pair<float, float> WintoOpenGL(POINT pt);
float dist(POINT p1, POINT p2);
int ccw(POINT , POINT , POINT );
