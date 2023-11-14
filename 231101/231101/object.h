#pragma once

class Object
{
	GLuint vbo[2];
	GLuint ebo;
	GLuint vao;
	
	vector<glm::vec3> v; // ���� ����
	vector<glm::vec3> c; // �� ���� 
	vector<unsigned int> i; // �ε��� ����

	glm::mat4 matrix = glm::mat4(1.0f); // ��ü ��� �༮
	glm::vec3 S{ 1.0f,1.0f,1.0f }; // ũ��
	glm::vec3 R{ 0.0f,0.0f,0.0f }; // ȸ�� ([0] - x, [1] - y, [2] - z) ������ �󸶳� ȸ�� ����
	glm::vec3 T{ 0.0f,0.0f,0.0f }; // �̵�


public:
	class BB* bb; // �ٿ�� �ڽ�
	glm::vec3 dir;

	glm::vec3 origin{0.0f, 0.0f, 0.0f}; // ��ü�� �⺻���� ���� �� ���� �༮�� ��, �μ� �������� ������ �����ִ� �� 
	glm::vec3 rotBy{ 0.0f, 0.0f, 0.0f }; // ���� ���� ������ �� �� ���� �������� ȸ���ϴ� ��
	glm::vec3 rotByAngle{ 0.0f, 0.0f, 0.0f }; // rotTo ���� ȸ�� ����
	
	// �������
	Object(const char*, glm::vec3, glm::vec3, glm::vec3);
	Object() {}
	~Object() { } 
	void InitBuffer(); // ������ �ȿ� ������ �ֽ��ϴ�
	void Remove();
	void Render(GLuint shaderProgramID);
	void Readobj(const char* s); // ���� �д� �༮
	void UpdateMatrix(); // render ���� ��ĺ�ȯ �����ϴ� �༮, update �Լ� �ȿ� �־��
	virtual void Update(); // ��ü���� �Լ� �ٸ��� �����Ű�� ���� virtual�� ���� ���� �̱� �ؿ�
	// ��������� �ǵ帮�� ������

	void Move(int n, float value) { T[n] += value;  } // T[n]�� ���� value ��ŭ ����
	void Rot(int n, float value) { R[n] += value; } // ��ü ��ü�� ȸ��, R[n]�� ���� value ��ŭ ����
	void RotByPoint(int n, bool b, glm::vec3 o);
	void SetRot(int n, float value) { R[n] = value; } // ��ü ��ü�� ȸ���� �־��� �� �� ����
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
