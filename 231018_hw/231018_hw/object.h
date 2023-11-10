#pragma once

class object
{
	vector<POINT> route;
	vector<POINT> pts;
	glm::vec3 color;
	vector<glm::vec3> pt;
	vector<glm::vec3> c;
	glm::vec2 dir;

	float gravity = 0.5f;
	float angle = 1.0f; // 새로 추가

public:
	bool isput = false;
	bool isSliced = false;
	object(int sides, int size ); // 모양, 크기
	object(vector<POINT> input, glm::vec2 d);

	void sortVertex();
	void render(GLuint vbo[]);
	void update();
	void remove();
	void move();
	void move(glm::vec2 d);
	pair<bool, pair<vector<POINT>, vector<POINT>>> isCross(POINT, POINT);
	bool isRemove();
	bool isIntersect(POINT, glm::vec2);

	glm::vec2 GETDIR() { return dir; }
};

