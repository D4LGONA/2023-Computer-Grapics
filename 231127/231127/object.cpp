#include "stdafx.h"
#include "Object.h"

void Object::UpdateBB()
{
	// centerpos 업데이트
	vCenterPos = glm::vec3(matrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	vAxisDir[0] = { 0.5f, 0.0f, 0.0f };
	vAxisDir[1] = { 0.0f, 0.5f, 0.0f };
	vAxisDir[2] = { 0.0f, 0.0f, 0.5f };

	// 각 방향벡터 업데이트
	for (int i = 0; i < 3; ++i)
	{
		vAxisDir[i] = glm::vec3(matrix * glm::vec4(vAxisDir[i], 1.0f)) - vCenterPos;
		fAxisLen[i] = glm::length(vAxisDir[i]);
		if (abs(fAxisLen[i]) < FLT_EPSILON)
			vAxisDir[i] = { 0.0f, 0.0f, 0.0f };
		else
			vAxisDir[i] /= fAxisLen[i];
	}
}

Object::Object(const char* c, GLuint sh, glm::vec3 s = { 1.0f, 1.0f, 1.0f }, glm::vec3 r = { 0.0f, 0.0f, 0.0f }, glm::vec3 t = { 0.0f, 0.0f, 0.0f }, glm::vec3 color = { uidC(dre), uidC(dre), uidC(dre) }, bool b = false)
	: S{s}, R{r}, T{t}, ShaderProgram{sh}, objColor{color, 0.0f}, blending{b}
{
	Readobj(c);
	
	speed = uidC(dre);
}

Object::Object(const char* c, GLuint sh, glm::vec3 color, int sz)
	: ShaderProgram(sh), objColor(color, 0.0f)
{
	S = { 50.0f, 50.0f, 50.0f };
	R = { 0.0f, 0.0f, 0.0f };
	T = { 0.0f, 12.6f, 0.0f };

	Readobj(c);

	vector<glm::vec3> tmp;
	for (int i = 0; i < sz; ++i)
	{
		for (int j = 0; j < v.size() - 12; j += 6)
		{
			tmp.push_back(v[j]);
			tmp.push_back(v[j+1]);
			tmp.push_back({ (v[j].x + v[j+2].x) / 2.0f, (v[j].y + v[j+2].y) / 2.0f, (v[j].z + v[j+2].z) / 2.0f });
			tmp.push_back(v[j+1]);
			tmp.push_back({ (v[j].x + v[j+4].x) / 2.0f, (v[j].y + v[j+4].y) / 2.0f, (v[j].z + v[j+4].z) / 2.0f });
			tmp.push_back(v[j+1]);

			tmp.push_back(v[j+2]);
			tmp.push_back(v[j+1]);
			tmp.push_back({ (v[j+2].x + v[j+4].x) / 2.0f, (v[j+2].y + v[j+4].y) / 2.0f, (v[j+2].z + v[j+4].z) / 2.0f });
			tmp.push_back(v[j+1]);
			tmp.push_back({ (v[j+2].x + v[j].x) / 2.0f, (v[j+2].y + v[j].y) / 2.0f, (v[j+2].z + v[j].z) / 2.0f });
			tmp.push_back(v[j+1]);

			tmp.push_back(v[j+4]);
			tmp.push_back(v[j+1]);
			tmp.push_back({ (v[j+4].x + v[j].x) / 2.0f, (v[j+4].y + v[j].y) / 2.0f, (v[j+4].z + v[j].z) / 2.0f });
			tmp.push_back(v[j+1]);
			tmp.push_back({ (v[j+4].x + v[j+2].x) / 2.0f, (v[j+4].y + v[j+2].y) / 2.0f, (v[j+4].z + v[j+2].z) / 2.0f });
			tmp.push_back(v[j+1]);
		}
		for (int k = 0; k < 12; ++k)
			tmp.push_back(v[v.size() - 12 + k]);
		v.clear();
		v = tmp;
		tmp.clear();
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * v.size(), v.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)0); //--- 위치 속성
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)(sizeof(glm::vec3))); //--- 노말 속성
	glEnableVertexAttribArray(1);
}

void Object::Update()
{
	UpdateMatrix();

	vCenterPos = T + origin;
	vAxisDir[0] = T + origin + glm::vec3{ 1.0f, 0.0f, 0.0f };
	vAxisDir[1] = T + origin + glm::vec3{ 0.0f, 1.0f, 0.0f };
	vAxisDir[2] = T + origin + glm::vec3{ 0.0f, 0.0f, 1.0f };
	UpdateBB();
}

void Object::Render()
{
	if (blending)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);

	glBindVertexArray(vao);

	unsigned int lightPosLocation = glGetUniformLocation(ShaderProgram, "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);
	int lightColorLocation = glGetUniformLocation(ShaderProgram, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, lightColor.r, lightColor.g, lightColor.b); // 조명 색
	int objColorLocation = glGetUniformLocation(ShaderProgram, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, objColor.r, objColor.g, objColor.b);
	unsigned int viewPosLocation = glGetUniformLocation(ShaderProgram, "viewPos"); //--- viewPos 값 전달: 카메라 위치
	glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);


	// Location 번호 저장
	unsigned int PosLocation = glGetAttribLocation(ShaderProgram, "in_Position"); //	: 0  Shader의 'layout (location = 0)' 부분
	unsigned int NormalLocation = glGetAttribLocation(ShaderProgram, "in_Normal"); //	: 1
	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "transform");
	unsigned int viewLocation = glGetUniformLocation(ShaderProgram, "view");
	unsigned int projLocation = glGetUniformLocation(ShaderProgram, "projection");
	unsigned int isLightLocation = glGetUniformLocation(ShaderProgram, "isLight");
	unsigned int LightLocation = glGetUniformLocation(ShaderProgram, "ambientLight");
	glUniform1i(isLightLocation, light_onf);
	glUniform1f(LightLocation, light_hardness);

	glEnableVertexAttribArray(PosLocation); // Enable 필수! 사용하겠단 의미
	glEnableVertexAttribArray(NormalLocation);

	glBindBuffer(GL_ARRAY_BUFFER, vbo); // 정점 좌표용 VBO 바인딩
	glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, 0);
	glVertexAttribPointer(NormalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, (void*)(sizeof(glm::vec3)));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(matrix));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &proj[0][0]);
	
	for (int i = 0; i < v.size(); i += 3)
	{
		if(blending)
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDrawArrays(GL_POLYGON, i, 3);
	}

	glDisableVertexAttribArray(PosLocation); // Disable 필수!
	glDisableVertexAttribArray(NormalLocation);
}

void Object::Remove()
{
	i.clear();
	v.clear();
	c.clear();
}

void Object::Readobj(const char* s)
{
	FILE* path = fopen(s, "r");
	if (path == NULL)
		return;

	char count[128];
	int vertexnum = 0;
	int facenum = 0;
	int uvnum = 0;
	int normalsnum = 0;
	//--- 1. 전체 버텍스 개수 및 삼각형 개수 세기
	while (!feof(path))
	{
		fscanf(path, "%s", count);
		if (count[0] == 'v' && count[1] == '\0')
			vertexnum++;
		else if (count[0] == 'f' && count[1] == '\0')
			facenum++;
		else if (count[0] == 'v' && count[1] == 't' && count[2] == '\0')
			uvnum++;
		else if (count[0] == 'v' && count[1] == 'n' && count[2] == '\0') 
			normalsnum++;
		memset(count, '\0', sizeof(count));
	}
	rewind(path);
	int vertIndex = 0;
	int faceIndex = 0;
	int uvIndex = 0;
	int normalIndex = 0;

	//--- 2. 메모리 할당
	glm::vec3* vertex = new glm::vec3[vertexnum];
	glm::vec3* face = new glm::vec3[facenum];
	glm::vec3* uvdata = new glm::vec3[facenum];
	glm::vec3* normalidx = new glm::vec3[facenum];
	glm::vec2* uv = new glm::vec2[uvnum];
	glm::vec3* normals = new glm::vec3[normalsnum];

	char bind[128];
	while (!feof(path)) {
		fscanf(path, "%s", bind);
		if (bind[0] == 'v' && bind[1] == '\0') {
			fscanf(path, "%f %f %f\n",
				&vertex[vertIndex].x, &vertex[vertIndex].y, &vertex[vertIndex].z);
			vertIndex++;
		}
		else if (bind[0] == 'f' && bind[1] == '\0') {
			unsigned int temp_face[3], temp_uv[3], temp_normal[3];
			fscanf(path, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&temp_face[0], &temp_uv[0], &temp_normal[0],
				&temp_face[1], &temp_uv[1], &temp_normal[1],
				&temp_face[2], &temp_uv[2], &temp_normal[2]);
			
			face[faceIndex].x = temp_face[0] - 1;
			face[faceIndex].y = temp_face[1] - 1;
			face[faceIndex].z = temp_face[2] - 1;
			uvdata[faceIndex].x = temp_uv[0];
			uvdata[faceIndex].y = temp_uv[1];
			uvdata[faceIndex].z = temp_uv[2];
			normalidx[faceIndex].x = temp_normal[0] - 1;
			normalidx[faceIndex].y = temp_normal[1] - 1;
			normalidx[faceIndex].z = temp_normal[2] - 1;
			faceIndex++;
		}
		else if (bind[0] == 'v' && bind[1] == 't' && bind[2] == '\0') {
			fscanf(path, "%f %f\n", &uv[uvIndex].x, &uv[uvIndex].y);
			uvIndex++;
		} 
		else if (bind[0] == 'v' && bind[1] == 'n' && bind[2] == '\0') {
			fscanf(path, "%f %f %f\n",
				&normals[normalIndex].x, &normals[normalIndex].y, &normals[normalIndex].z);
			normalIndex++;
		}
	}

	for (int j = 0; j < facenum; ++j)
	{
		v.push_back(vertex[int(face[j].x)]);
		v.push_back(normals[int(normalidx[j].x)]);
		v.push_back(vertex[int(face[j].y)]);
		v.push_back(normals[int(normalidx[j].y)]);
		v.push_back(vertex[int(face[j].z)]);
		v.push_back(normals[int(normalidx[j].z)]);
	}

	InitBuffer();
	UpdateMatrix();

	fclose(path);
}

void Object::UpdateMatrix()
{
	matrix = glm::mat4(1.0f);

	// 한 점을 기준으로 이동
	matrix = glm::translate(matrix, rotBy);
	matrix = glm::rotate(matrix, glm::radians(rotByAngle.x), { 1.0f, 0.0f, 0.0f });
	matrix = glm::rotate(matrix, glm::radians(rotByAngle.y), { 0.0f, 1.0f, 0.0f });
	matrix = glm::rotate(matrix, glm::radians(rotByAngle.z), { 0.0f, 0.0f, 1.0f });
	matrix = glm::translate(matrix, -rotBy);

	// 자체 이동
	matrix = glm::translate(matrix, T + origin);
	matrix = glm::rotate(matrix, glm::radians(R.x), { 1.0f, 0.0f, 0.0f });
	matrix = glm::rotate(matrix, glm::radians(R.y), { 0.0f, 1.0f, 0.0f });
	matrix = glm::rotate(matrix, glm::radians(R.z), { 0.0f, 0.0f, 1.0f });
	matrix = glm::scale(matrix, S);
}

void Object::RotByPoint(int n, bool b, glm::vec3 o)
{
	rotBy = o;
	if (b) rotByAngle[n] += speed * 10.0f;
	else rotByAngle[n] -= speed * 10.0f;
}

void Object::makeTriangle(int n)
{
}

void Object::InitBuffer()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * v.size(), v.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)0); //--- 위치 속성
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)(sizeof(glm::vec3))); //--- 노말 속성
	glEnableVertexAttribArray(1);

	
}