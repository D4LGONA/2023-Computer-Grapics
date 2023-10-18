#pragma once

class object
{
	// 들어가야 할 것 -> 정점 들...
	vector<POINT> pts;
	glm::vec3 color;
	vector<glm::vec3> pt;
	vector<glm::vec3> c;

	int size = 50;
	bool isSolid = true;


public:
	object(int x, int y, int sides, int size ); // 중심 좌표와  모양, 크기

	void render(GLuint vbo[]);
	void update();
	void remove();
	pair<bool, vector<POINT>> isCross(POINT, POINT);
};

