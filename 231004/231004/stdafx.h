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

struct color
{
	GLfloat r;
	GLfloat g;
	GLfloat b;
};

struct vec3
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

struct vertex
{
	vector<vec3> pt;
	vector<color> c;
};