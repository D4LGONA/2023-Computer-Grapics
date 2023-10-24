#pragma once

class object
{
	vector<glm::vec3> v; // 정점 정보
	vector<glm::vec3> c; // 색 정보
	vector<unsigned int> i; // 인덱스 정보
	GLUquadric* o;

	float radius = 1.0f;
	float bigspin = 0.0f;
	float tbigspin = 0.0f;

public:
	float orbit = 0.0f;
	bool isSpinY = false;
	bool isSpinZ = false;
	bool isSpinPlus = false;
	glm::mat4 matrix = glm::mat4(1.0f); // 이동 행렬 > 필요할까?

	glm::vec3 rotation{ 0.0f,0.0f,0.0f }; // 회전 
	glm::vec3 transition{ 0.0f,0.0f,0.0f }; // 이동
	glm::vec3 scale{ 1.0f,1.0f,1.0f }; // 크기

	object(const char*);
	object(float, glm::vec3, glm::vec3, float);

	void setmatrix(glm::vec3 r, glm::vec3 t, glm::vec3 s, glm::vec3 r2, glm::vec3 t2, glm::vec3 s2, float orbit);
	void setmatrix(glm::mat4 p);
	void setmatrix(glm::vec3 s, glm::vec3 r, glm::vec3 t);
	void setmatrix();
	void remove();
	void update();
	void render(GLuint vbo[], GLuint ebo, GLuint shaderProgramID);
	void readobj(const char* s);
	void bind(GLuint vbo[]);
	void bigSpin(glm::vec3 s, glm::vec3 r, glm::vec3 t);
};

