#pragma once

class object
{
	// ���� �� �� -> ���� ��...
	vector<POINT> pts;
	glm::vec3 color;
	vector<glm::vec3> pt;
	vector<glm::vec3> c;

	int size = 50;
	bool isWire = true;


public:

	void render(GLuint vbo[]);
	void update();
	void remove();
};

