#pragma once

class object
{
	GLUquadric* obj;
	glm::mat4 matrix{ 1.0f };
	int angleX = 0;
	int angleY = 0;
	glm::vec3 transition{ 0.0f, 0.0f, 0.0f };

	int shape = 0; // วüลย
	//float scale = 0.2f;

public:
	object();
	~object();

	void update();
	void render();
};

