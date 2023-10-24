#pragma once

class object
{
	vector<glm::vec3> v; // ���� ����
	vector<glm::vec3> c; // �� ����
	vector<unsigned int> i; // �ε��� ����

	glm::mat4 matrix = glm::mat4(1.0f); // ��ü ��� �༮
	glm::vec3 scale{ 1.0f,1.0f,1.0f }; // ũ��
	glm::vec3 rotation{ 0.0f,0.0f,0.0f }; // ȸ�� 
	glm::vec3 transition{ 0.0f,0.0f,0.0f }; // �̵�

	object* parent = nullptr;

public:


	object(const char*);
	void remove();
	void update();
	void render(GLuint vbo[], GLuint ebo, GLuint shaderProgramID);
	void readobj(const char* s);
	void bind(GLuint vbo[], GLuint ebo);
};
