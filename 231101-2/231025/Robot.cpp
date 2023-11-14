#include "stdafx.h"
#include "Robot.h"
#include "BB.h"
#include "object.h"

Robot::Robot(glm::vec3 startpt, float scale)
{
	origin = startpt;

	// ´Ù¸®
	parts.push_back({ "cube.obj", {2.0f, 6.0f, 2.0f}, {0.0f,0.0f,0.0f}, {-1.0f, -17.0f, 0.0f} });
	parts.push_back({ "cube.obj", {2.0f, 6.0f, 2.0f}, {0.0f,0.0f,0.0f}, {1.0f, -17.0f, 0.0f} });

	// ¸öÅë
	parts.push_back({ "cube.obj", {4.0f, 6.0f, 2.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, -11.0f, 0.0f } });

	// ¸Ó¸®
	parts.push_back({ "cube.obj", {2.0f, 2.0f, 2.0f}, {0.0f,0.0f,0.0f}, {0.0f, -7.0f, 0.0f} });

	// ÆÈ
	parts.push_back({ "cube.obj", {2.0f, 6.0f, 2.0f}, {0.0f,0.0f,0.0f}, {-3.0f, -11.0f, 0.0f} });
	parts.push_back({ "cube.obj", {2.0f, 6.0f, 2.0f}, {0.0f,0.0f,0.0f}, {3.0f, -11.0f, 0.0f} });

	// nose
	parts.push_back({ "cube.obj", {0.2f,0.2f,2.0f}, {0.0f,0.0f,0.0f}, {0.0f, -7.0f, 1.0f} });

	for (int i = 0; i < parts.size(); ++i)
	{
		parts[i].scale = { scale, scale, scale };
	}

}

void Robot::RotationY(float angle)
{
	for (int i = 0; i < parts.size(); ++i)
	{
		parts[i].rotBy = origin;
		parts[i].rotByAngle = { 0.0f, angle, 0.0f };
	}
}

void Robot::Update()
{
	/*speed.y += gravity;
	Move(1, speed.y);*/

	for (int i = 0; i < parts.size(); ++i)
		parts[i].rotBy = origin;

	for (int i = 0; i < parts.size(); ++i)
		parts[i].Update();
}

void Robot::Render(GLuint shaderProgramID)
{
	for (int i = 0; i < parts.size(); ++i)
		parts[i].Render(shaderProgramID);

	for (int i = 0; i < parts.size(); ++i)
		parts[i].bb->Render(shaderProgramID);
}

void Robot::Walk(bool arr[])
{
	if (walkcount == 5 || walkcount == -5)
		count_up = !count_up;

	/*parts[0].rotPoint.y -= 14.0f;
	parts[0].rotOrigin.x = walk_angle * walkcount;
	parts[1].rotPoint.y -= 14.0f;
	parts[1].rotOrigin.x = -1 * walk_angle * walkcount;


	parts[4].rotPoint.y -= 8.0f;
	parts[4].rotOrigin.x = -1 * walk_angle * walkcount;
	parts[5].rotPoint.y -= 8.0f;
	parts[5].rotOrigin.x = walk_angle * walkcount;*/

	if (count_up)
		walkcount++;
	else
		walkcount--;
}

void Robot::Move()
{
	dir.y += gravity;

	origin += dir;
}

void Robot::Move(int n, float s)
{
	for (int i = 0; i < parts.size(); ++i)
	{
		parts[i].Move(n, s);
	}
}

void Robot::Jump()
{


	dir.y += 2.0f;
}
