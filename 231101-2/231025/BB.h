#pragma once
class BB
{
	int type = 1; // 1 = box, 2 = circle
	glm::mat4 matrix = glm::mat4(1.0f);

	GLuint vbo[2];
	GLuint ebo;
	GLuint vao;

	vector<glm::vec3> v; // 정점 정보
	vector<glm::vec3> c; // 색 정보 
	vector<unsigned int> i; // 인덱스 정보

public:
	glm::vec3 vCenterPos = {0.0f, 0.0f, 0.0f}; // 상자 중앙의 좌표
	glm::vec3 vAxisDir[3] = { {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} }; //상자에 평행한 세 축의 단위벡터
	float fAxisLen[3]; // 상자의 평행한 세 축의 길이 fAxisLen[n]은 vAxisDir[n]에 각각 대응한다.



	BB(int t);

	void Update(glm::mat4);
	void Render(GLuint shaderProgramID);
	void Readobj(const char* s);
	void InitBuffer();
};

