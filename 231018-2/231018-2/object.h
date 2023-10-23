#pragma once

class object
{
	vector<glm::vec3> v; // ���� ����
	vector<glm::vec3> c; // �� ����
	vector<unsigned int> i; // �ε��� ����
	GLUquadric* o;

	glm::mat4 matrix = glm::mat4(1.0f); // �̵� ��� > �ʿ��ұ�?
	float radius = 1.0f;
	float bigspin = 0.0f;
	float orbit = 0.0f;

public:
	bool isSpinY = false;
	bool isSpinPlus = false;

	glm::vec3 rotation{ 0.0f,0.0f,0.0f }; // ȸ�� 
	glm::vec3 transition{ 0.0f,0.0f,0.0f }; // �̵�
	glm::vec3 scale{ 1.0f,1.0f,1.0f }; // ũ��

	object(const char*);
	object(float, glm::vec3, glm::vec3);

	void setmatrix(glm::vec3 s, glm::vec3 r, glm::vec3 t);
	void setmatrix();
	void remove();
	void update();
	void render(GLuint vbo[], GLuint ebo, GLuint shaderProgramID);
	void readobj(const char* s);
	void bind(GLuint vbo[]);
	void bigSpin(glm::vec3 s, glm::vec3 r, glm::vec3 t);
};

