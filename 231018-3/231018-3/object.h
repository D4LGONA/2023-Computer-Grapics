#pragma once

class object
{
	GLuint vbo[2];
	GLuint ebo;
	GLuint vao;
	
	vector<glm::vec3> v; // ���� ����
	vector<glm::vec3> c; // �� ����
	vector<unsigned int> i; // �ε��� ����

	glm::mat4 matrix = glm::mat4(1.0f); // ��ü ��� �༮
	glm::vec3 scale{ 0.5f,0.5f,0.5f }; // ũ��
	glm::vec3 rotation{ 0.0f,0.0f,0.0f }; // ȸ�� 
	glm::vec3 transition{ 0.0f,0.0f,0.0f }; // �̵�

	object* parent = nullptr;

public:
	object(const char*, glm::vec3, glm::vec3, glm::vec3);
	object() {}
	void InitBuffer();

	void remove();
	void update();
	void render(GLuint shaderProgramID);
	void readobj(const char* s);
	void updateMatrix();

	void move(int n) { transition[n] += 1.0f; }
	void rot(int n) { rotation[n] += 5.0f; }
};
