#pragma once

class object
{
	vector<glm::vec3> v; // 정점 정보
	vector<glm::vec3> c; // 색 정보
	vector<unsigned int> i; // 인덱스 정보

	vector<glm::vec3> rotation; // 회전 
	vector<glm::vec3> transition; // 이동
	vector<glm::vec3> scale; // 크기

	vector<glm::mat4> matrix; // 이동 행렬 > 필요할까?
	int shape = 0; // 형태
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

