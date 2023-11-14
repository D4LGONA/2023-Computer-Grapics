#pragma once

class Object
{
	GLuint vbo[2];
	GLuint ebo;
	GLuint vao;
	
	vector<glm::vec3> v; // 정점 정보
	vector<glm::vec3> c; // 색 정보 
	vector<unsigned int> i; // 인덱스 정보

	glm::mat4 matrix = glm::mat4(1.0f); // 전체 행렬 녀석
	glm::vec3 S{ 1.0f,1.0f,1.0f }; // 크기
	glm::vec3 R{ 0.0f,0.0f,0.0f }; // 회전 ([0] - x, [1] - y, [2] - z) 축으로 얼마나 회전 할지
	glm::vec3 T{ 0.0f,0.0f,0.0f }; // 이동


public:
	class BB* bb; // 바운딩 박스
	glm::vec3 dir;

	glm::vec3 origin{0.0f, 0.0f, 0.0f}; // 물체가 기본도형 여러 개 붙은 녀석일 때, 부속 도형들의 원점을 정해주는 것 
	glm::vec3 rotBy{ 0.0f, 0.0f, 0.0f }; // 공전 등을 구현할 때 한 점을 기준으로 회전하는 것
	glm::vec3 rotByAngle{ 0.0f, 0.0f, 0.0f }; // rotTo 기준 회전 각도
	
	// 여기부터
	Object(const char*, glm::vec3, glm::vec3, glm::vec3);
	Object() {}
	~Object() { } 
	void InitBuffer(); // 생성자 안에 무조건 넣습니다
	void Remove();
	void Render(GLuint shaderProgramID);
	void Readobj(const char* s); // 파일 읽는 녀석
	void UpdateMatrix(); // render 전에 행렬변환 수정하는 녀석, update 함수 안에 넣어요
	virtual void Update(); // 물체별로 함수 다르게 실행시키기 위해 virtual로 뺐음 굳이 이긴 해요
	// 여기까지는 건드리지 말도록

	void Move(int n, float value) { T[n] += value;  } // T[n]의 값을 value 만큼 변경
	void Rot(int n, float value) { R[n] += value; } // 객체 자체의 회전, R[n]의 값을 value 만큼 변경
	void RotByPoint(int n, bool b, glm::vec3 o);
	void SetRot(int n, float value) { R[n] = value; } // 객체 자체의 회전을 주어진 값 을 변경
	void SetMove(int n, float value) { T[n] = value; }
	glm::vec3 GetRot() { return R; }
	glm::vec3 GetMinY()
	{
		vector<glm::vec3> tmp;

		for (int i = 0; i < v.size(); ++i)
		{
			glm::vec3 t = matrix * glm::vec4(v[i], 1.0f);
			tmp.push_back(t);
		}

		auto result = min_element(tmp.begin(), tmp.end(), [this](const glm::vec3& a, const glm::vec3& b) {return a.y > b.y; });

		return *result;
	}
}; 
