#pragma once

class object
{
	// 들어가야 할 것 -> 정점 들...
	vector<POINT> pts;
	glm::vec3 color;
	vector<glm::vec3> pt;
	vector<glm::vec3> c;
	pair<float, float> dir;

	float gravity = 0.5f;

public:
	object(int sides, int size ); // 모양, 크기
	object(vector<POINT> input);

	void sortVertex();
	void sliceMove(int dir);
	void render(GLuint vbo[]);
	void update();
	void remove();
	void move();

	pair<bool, pair<vector<POINT>, vector<POINT>>> isCross(POINT, POINT);
};

