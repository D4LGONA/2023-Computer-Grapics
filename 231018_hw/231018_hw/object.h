#pragma once

class object
{
	// ���� �� �� -> ���� ��...
	vector<POINT> pts;
	glm::vec3 color;
	vector<glm::vec3> pt;
	vector<glm::vec3> c;

	int size = 50;
	bool isSolid = true;


public:
	object(int x, int y, int sides, int size ); // �߽� ��ǥ��  ���, ũ��

	void render(GLuint vbo[]);
	void update();
	void remove();
	pair<bool, vector<POINT>> isCross(POINT, POINT);
};

