#pragma once

class object
{
	GLuint vbo[2];
	GLuint ebo;
	GLuint vao;
	
	vector<glm::vec3> v; // ���� ����
	vector<glm::vec3> c; // �� ����
	vector<unsigned int> i; // �ε��� ����

	glm::vec3 scale{ 0.5f,0.5f,0.5f }; // ũ��
	glm::vec3 rotation{ 0.0f,0.0f,0.0f }; // ȸ�� 

	glm::vec3 rotOrigin{ 0.0f, 0.0f, 0.0f };

	object* parent = nullptr;
public:

	glm::mat4 matrix = glm::mat4(1.0f); // ��ü ��� �༮
	glm::vec3 Origin{};
	glm::vec3 rotPoint{ 0.0f, 0.0f, 0.0f };
	glm::vec3 transition{ 0.0f,0.0f,0.0f }; // �̵�

	object(const char*, glm::vec3, glm::vec3, glm::vec3);
	object() {}
	void InitBuffer();

	void remove();
	void update();
	void render(GLuint shaderProgramID);
	void readobj(const char* s);
	void updateMatrix();
	void updateMatrix(glm::mat4);

	void move(int n, bool b) { if(b) transition[n] += 0.1f; else transition[n] -= 0.1f;}
	void rot(int n, bool b) { if (b) rotation[n] += 5.0f; else rotation[n] -= 5.0f; }
	void rotbyorigin(int n, bool b, glm::vec3 o);

	glm::vec3 GETROTORIGIN() { return rotOrigin; }
	glm::vec3 GETORIGIN() { return { transition.x, transition.y - 0.5f, transition.z }; }
};
