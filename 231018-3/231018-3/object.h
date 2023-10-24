#pragma once

class object
{
	vector<glm::vec3> v; // 정점 정보
	vector<glm::vec3> c; // 색 정보
	vector<unsigned int> i; // 인덱스 정보

	glm::mat4 matrix = glm::mat4(1.0f); // 전체 행렬 녀석
	glm::vec3 scale{ 1.0f,1.0f,1.0f }; // 크기
	glm::vec3 rotation{ 0.0f,0.0f,0.0f }; // 회전 
	glm::vec3 transition{ 0.0f,0.0f,0.0f }; // 이동

	object* parent = nullptr;

public:


	object(const char*);
	void remove();
	void update();
	void render(GLuint vbo[], GLuint ebo, GLuint shaderProgramID);
	void readobj(const char* s);
	void bind(GLuint vbo[], GLuint ebo);
};
