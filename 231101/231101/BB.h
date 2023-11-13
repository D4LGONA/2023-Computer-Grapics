#pragma once
class BB
{
	int type = 1; // 1 = box, 2 = circle
	glm::mat4 matrix = glm::mat4(1.0f);

	GLuint vbo[2];
	GLuint ebo;
	GLuint vao;

	vector<glm::vec3> v; // ���� ����
	vector<glm::vec3> c; // �� ���� 
	vector<unsigned int> i; // �ε��� ����

public:
	BB(int t);

	void Update(glm::mat4);
	void Render(GLuint shaderProgramID);
	void Readobj(const char* s);
	void InitBuffer();
};

