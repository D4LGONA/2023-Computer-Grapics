#pragma once

class object
{
	vector<glm::vec3> v; // ���� ����
	vector<glm::vec3> c; // �� ����
	vector<unsigned int> i; // �ε��� ����

	vector<glm::vec3> rotation; // ȸ�� 
	vector<glm::vec3> transition; // �̵�
	vector<glm::vec3> scale; // ũ��

	vector<glm::mat4> matrix; // �̵� ��� > �ʿ��ұ�?
	int shape = 0; // ����
	bool rotating = false;
	bool topRotate = false;
	bool openF = false;
	bool moveSide = false;
	float openRot = 0.0f;
	bool backS = false;
	float backSc = 1.0f;

	bool openall = false;
	pair<bool, float> isOpen[6] = { {false, 0.0f}, {false, 0.0f}, {false, 0.0f}, {false, 0.0f}, {false, 0.0f}, {false, 0.0f} };

public:
	object(const char*);
	object(float fx, float fy, float fz, int s, GLuint vbo[]);
	//~object();

	void remove();
	void update();
	void render(GLuint vbo[], GLuint ebo, GLuint shaderProgramID);
	void readobj(const char* s);
	void bind(GLuint vbo[]);
	
	void SETROTATING(bool b) { rotating = b; }
	void SETTROTATING(bool b) { topRotate = b; }
	void SETOPENF(bool b) { openF = b; }
	void SETSLIDES(bool b) { moveSide = b; }
	void SETBACK(bool b) {backS = b;}
	void SETOPEN(int n, bool b) { isOpen[n].first = b; }
	void SETOPENALL(bool b) { openall = b;  }
	//glm::mat4 GETMATRIX() { return matrix; }
};

