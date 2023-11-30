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

Object::Object(const char* c, GLuint sh, glm::vec3 s = { 1.0f, 1.0f, 1.0f }, glm::vec3 r = { 0.0f, 0.0f, 0.0f }, glm::vec3 t = { 0.0f, 0.0f, 0.0f }, glm::vec3 color = {uidC(dre), uidC(dre), uidC(dre) })
	: S{s}, R{r}, T{t}, ShaderProgram{sh}, objColor{color}
{
	Readobj(c);
	
	speed = uidC(dre) * 2.0f;
	max_size = uidMax(dre);
	min_size = uidMin(dre);
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
	glBindVertexArray(vao);

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	view = glm::translate(view, +cameraDirection);
	view = glm::rotate(view, glm::radians(cameraAngle.x), { 1.0f, 0.0f, 0.0f });
	view = glm::rotate(view, glm::radians(cameraAngle.y), { 0.0f, 1.0f, 0.0f });
	view = glm::rotate(view, glm::radians(cameraAngle.z), { 0.0f, 0.0f, 1.0f });
	view = glm::translate(view, -cameraDirection);

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
	glUniform1i(isLightLocation, light_onf);

	glEnableVertexAttribArray(PosLocation); // Enable 필수! 사용하겠단 의미
	glEnableVertexAttribArray(NormalLocation);

	glBindBuffer(GL_ARRAY_BUFFER, vbo); // 정점 좌표용 VBO 바인딩
	glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, 0);
	glVertexAttribPointer(NormalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, (void*)(sizeof(glm::vec3)));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(matrix));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &proj[0][0]);
	
	for (int i = 0; i < v.size(); i += 3)
		glDrawArrays(GL_TRIANGLES, i, 3);

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
	matrix = glm::rotate(matrix, glm::radians(rotByAngle.z), { 0.0f, 0.0f, 1.0f });
	matrix = glm::rotate(matrix, glm::radians(rotByAngle.y), { 0.0f, 1.0f, 0.0f });
	matrix = glm::rotate(matrix, glm::radians(rotByAngle.x), { 1.0f, 0.0f, 0.0f });
	matrix = glm::translate(matrix, -rotBy);

	// 자체 이동
	matrix = glm::translate(matrix, T + rotBy);
	matrix = glm::rotate(matrix, glm::radians(R.x), { 1.0f, 0.0f, 0.0f });
	matrix = glm::rotate(matrix, glm::radians(R.y), { 0.0f, 1.0f, 0.0f });
	matrix = glm::rotate(matrix, glm::radians(R.z), { 0.0f, 0.0f, 1.0f });
	matrix = glm::scale(matrix, S);
}

void Object::RotByPoint(int n, bool b, glm::vec3 o)
{
	rotBy = o;
	if (b) rotByAngle[n] += 5.0f;
	else rotByAngle[n] -= 5.0f;
}

void Object::ani1()
{
	if (be_large)
	{
		S.y += speed;
		T.y = S.y / 2.0f;
		if (S.y > max_size)
		{
			S.y = max_size;
			T.y = max_size / 2.0f;
			be_large = !be_large;
		}
	}
	else
	{
		S.y -= speed;
		T.y = S.y / 2.0f;
		if (S.y < min_size)
		{
			S.y = min_size;
			T.y = min_size / 2.0f;
			be_large = !be_large;
		}
	}
}

void Object::ani2()
{
	if (be_large)
	{
		S.y += speed;
		T.y = S.y / 2.0f;
		if (S.y > max_size)
		{
			S.y = max_size;
			T.y = max_size / 2.0f;
			be_large = !be_large;
		}
	}
	else
	{
		S.y -= speed;
		T.y = S.y / 2.0f;
		if (S.y < min_size)
		{
			S.y = min_size;
			T.y = min_size / 2.0f;
			be_large = !be_large;
		}
	}
}

void Object::ani3()
{
	if (be_large)
	{
		speed += 0.1f;
		S.y += speed;
		T.y = S.y / 2.0f;

		if (S.y > max_size)
		{
			speed *= 0.5f;
			be_large = !be_large;
			max_size = max(0.5f * max_size, 10.0f);
		}
	}
	else
	{
		speed -= 0.1f;
		S.y += speed;
		if (S.y < min_size)
		{
			speed *= 0.5f;
			be_large = !be_large;
		}
	}
	if (abs(speed) < 0.1f and abs(S.y - 10.0f) < 0.5f)
	{
		S.y = 10.0f;
		speed = 0.0f;
		falling = false;
	}

	T.y = S.y / 2.0f;
}

void Object::dragging(float a)
{
	S.y += a;
	T.y = S.y / 2.0f;
	speed = 0.0f;
}

void Object::set_ani2(int n, float ss)
{
	S.y = n * ss;
	T.y = S.y / 2.0f;
	be_large = true;
	speed = 2.0f;
	max_size = 70.0f;
	min_size = 0.0f;
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