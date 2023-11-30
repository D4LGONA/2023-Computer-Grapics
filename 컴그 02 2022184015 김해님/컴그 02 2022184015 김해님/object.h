#pragma once

class Object
{
	GLuint vbo;
	GLuint ebo;
	GLuint vao;
	GLuint ShaderProgram;


	vector<glm::vec3> v; // ���� ���� + �ٿ�� �ڽ� ����, size - 8 �ؼ� ���� ��ü�� ������ ��������
	vector<glm::vec3> c; // �� ���� 
	vector<glm::vec3> n; // �븻 ��
	vector<glm::vec3> uv; // �ؽ��� uv
	vector<unsigned int> i; // �ε��� ����, size - 
	vector<unsigned int> ni;

	glm::mat4 matrix = glm::mat4(1.0f); // ��ü ��� �༮
	glm::vec3 S{ 1.0f,1.0f,1.0f }; // ũ��
	glm::vec3 R{ 0.0f,0.0f,0.0f }; // ȸ�� ([0] - x, [1] - y, [2] - z) ������ �󸶳� ȸ�� ����
	glm::vec3 T{ 0.0f,0.0f,0.0f }; // �̵�

	glm::vec3 origin{ 0.0f, 0.0f, 0.0f }; // ��ü�� �⺻���� ���� �� ���� �༮�� ��, �μ� �������� ������ �����ִ� �� 
	glm::vec3 rotBy{ 0.0f, 0.0f, 0.0f }; // ���� ���� ������ �� �� ���� �������� ȸ���ϴ� ��
	glm::vec3 rotByAngle{ 0.0f, 0.0f, 0.0f }; // rotTo ���� ȸ�� ����

public:
	// obb���� ����ϴ� �༮�� // 
	glm::vec3 vCenterPos = { 0.0f, 0.0f, 0.0f }; // ���� �߾��� ��ǥ
	glm::vec3 vAxisDir[3] = { {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} }; //���ڿ� ������ �� ���� ��������
	float fAxisLen[3]; // ������ ������ �� ���� ���� fAxisLen[n]�� vAxisDir[n]�� ���� �����Ѵ�.
	void UpdateBB();
	// // // // // // // // //

	// ���� ���� //
	glm::vec3 objColor{ 1.0f, 1.0f, 1.0f };

	// ��ŷ ���� //
	bool isSelected = false;

	// �̵� ���� //
	float speed = 0.0f;
	float max_size = 0.0f;
	float min_size = 0.0f;
	bool be_large = true;
	bool falling = false;

	// �������
	Object(const char*, GLuint, glm::vec3, glm::vec3, glm::vec3, glm::vec3);
	Object() {}
	~Object() {}
	void InitBuffer(); // ������ �ȿ� ������ �ֽ��ϴ�
	void Remove();
	void Render();
	void Readobj(const char* s); // ���� �д� �༮
	void UpdateMatrix(); // render ���� ��ĺ�ȯ �����ϴ� �༮, update �Լ� �ȿ� �־��
	virtual void Update(); // ��ü���� �Լ� �ٸ��� �����Ű�� ���� virtual�� ���� ���� �̱� �ؿ�
	// ��������� �ǵ帮�� ������

	void Move(int n, float value) { T[n] += value; origin[n] += value; } // T[n]�� ���� value ��ŭ ����
	void Rot(int n, float value) { R[n] += value; } // ��ü ��ü�� ȸ��, R[n]�� ���� value ��ŭ ����
	void RotByPoint(int n, bool b, glm::vec3 o);
	void SetRot(int n, float value) { R[n] = value; } // ��ü ��ü�� ȸ���� �־��� �� �� ����
	void SetMove(int n, float value) { T[n] = value; }
	void SetScale(int n, float value) { S[n] = value; T[n] = S[n] / 2.0f; }
	float GetScale(int n) { return S[n]; }
	glm::vec3 GetRot() { return R; }

	void ani1();
	void ani2();
	void ani3();
	void dragging(float);
	void set_ani1() { speed = uidC(dre) * 2.0f; max_size = uidMax(dre); min_size = uidMin(dre); }
	void set_ani2(int, float);
	void set_ani3() { S.y = 10.0f; T.y = S.y / 2.0f; }
};
