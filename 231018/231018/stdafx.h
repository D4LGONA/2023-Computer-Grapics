#pragma once

#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <random>
#include <vector>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
using namespace std;

#define WIDTH 800
#define HEIGHT 800

extern std::random_device rd;
extern std::mt19937 dre;
extern std::uniform_real_distribution<float> uidC;
extern glm::mat4 proj;

pair<float, float> WintoOpenGL(POINT pt);
float dist(POINT p1, POINT p2);