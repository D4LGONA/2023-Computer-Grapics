#pragma once

class object
{
	// ���� �� �� -> ���� ��...
	vector<POINT> pts;
	glm::vec3 color;
	vector<glm::vec3> pt;
	vector<glm::vec3> c;
	glm::vec2 dir;

	float gravity = 0.5f;

	bool isput = false;
public:
	object(int sides, int size ); // ���, ũ��
	object(vector<POINT> input, glm::vec2 d);

	void sortVertex();
	void render(GLuint vbo[]);
	void update();
	void remove();
	void move();
	pair<bool, pair<vector<POINT>, vector<POINT>>> isCross(POINT, POINT);
	bool isRemove();
	bool isIntersect(POINT, glm::vec2);

	glm::vec2 GETDIR() { return dir; }
};

