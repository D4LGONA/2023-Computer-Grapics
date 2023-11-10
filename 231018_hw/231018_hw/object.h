#pragma once

class object
{
	glm::vec3 color;
	vector<glm::vec3> v;
	vector<glm::vec3> c;
	glm::vec2 dir;

	GLuint vbo[2], vao;

	glm::mat4 matrix = glm::mat4(1.0f);

	float gravity = 0.5f / 800.;
	float angle = uidA(dre); // 얼마만큼 회전시킬지 정하는 것

public:
	bool isrender = true;
	bool isput = false; // 바구니 안에 담겼는지
	bool isSliced = false; // 한번 이상 슬라이스 됐는지

	glm::vec3 S{ 1.0f, 1.0f, 1.0f };
	glm::vec3 R{ 0.0f, 0.0f, 0.0f };
	glm::vec3 T{ 0.0f, 0.0f, 0.0f };

	object(int sides, float size); // 모양, 크기
	object(vector<glm::vec3> input, glm::vec2 d, glm::vec3 s, glm::vec3 r, glm::vec3 t);
	object(vector<glm::vec3> pts);

	vector<glm::vec3> getRoute();

	void sortVertex();
	void render(GLuint shaderProgramID, bool isline);
	void update();
	void remove();
	void move();
	void move(glm::vec2 d);
	pair<bool, pair<vector<glm::vec3>, vector<glm::vec3>>> isCross(glm::vec3, glm::vec3);
	bool isRemove();
	bool isIntersect(glm::vec3, glm::vec2);
	glm::vec3 updateMatrix(glm::vec3 pt)
	{
		glm::vec4 tmppt(pt, 1.0f);
		glm::vec3 resultPoint(matrix * tmppt);
		return resultPoint;
	}

	void InitBuffer();

	glm::vec2 GETDIR() { return dir; }
};

