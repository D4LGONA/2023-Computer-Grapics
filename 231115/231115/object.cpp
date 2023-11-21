#include "stdafx.h"
#include "Object.h"

void Object::UpdateBB()
{
	// centerpos ������Ʈ
	vCenterPos = glm::vec3(matrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	vAxisDir[0] = { 0.5f, 0.0f, 0.0f };
	vAxisDir[1] = { 0.0f, 0.5f, 0.0f };
	vAxisDir[2] = { 0.0f, 0.0f, 0.5f };

	// �� ���⺤�� ������Ʈ
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

void Object::Render(GLuint ShaderProgram)
{
	glBindVertexArray(vao);

	unsigned int lightPosLocation = glGetUniformLocation(ShaderProgram, "lightPos"); //--- lightPos �� ����: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);
	int lightColorLocation = glGetUniformLocation(ShaderProgram, "lightColor"); //--- lightColor �� ����: (1.0, 1.0, 1.0) ���
	glUniform3f(lightColorLocation, lightColor.r, lightColor.g, lightColor.b); // ���� ��
	int objColorLocation = glGetUniformLocation(ShaderProgram, "objectColor"); //--- object Color�� ����: (1.0, 0.5, 0.3)�� ��
	glUniform3f(objColorLocation, objColor.r, objColor.g, objColor.b);
	unsigned int viewPosLocation = glGetUniformLocation(ShaderProgram, "viewPos"); //--- viewPos �� ����: ī�޶� ��ġ
	glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);


	// Location ��ȣ ����
	unsigned int PosLocation = glGetAttribLocation(ShaderProgram, "in_Position"); //	: 0  Shader�� 'layout (location = 0)' �κ�
	unsigned int NormalLocation = glGetAttribLocation(ShaderProgram, "in_Normal"); //	: 1
	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "transform");
	unsigned int viewLocation = glGetUniformLocation(ShaderProgram, "view");
	unsigned int projLocation = glGetUniformLocation(ShaderProgram, "projection");
	unsigned int isLightLocation = glGetUniformLocation(ShaderProgram, "isLight");
	glUniform1i(isLightLocation, light_onf);

	glEnableVertexAttribArray(PosLocation); // Enable �ʼ�! ����ϰڴ� �ǹ�
	glEnableVertexAttribArray(NormalLocation);

	glBindBuffer(GL_ARRAY_BUFFER, vbo); // ���� ��ǥ�� VBO ���ε�
	glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, 0);
	glVertexAttribPointer(NormalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, (void*)(sizeof(glm::vec3)));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(matrix));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &proj[0][0]);
	
	for (int i = 0; i < v.size(); i += 3)
	{
		glDrawArrays(GL_TRIANGLES, i, 3);
	}

	glDisableVertexAttribArray(PosLocation); // Disable �ʼ�!
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
	//--- 1. ��ü ���ؽ� ���� �� �ﰢ�� ���� ����
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

	//--- 2. �޸� �Ҵ�
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

	/*for (int i = 0; i < vertexnum; ++i)
	{
		v.emplace_back(vertex[i]);
		c.push_back(objColor);
	}

	for (int i = 0; i < normalsnum; ++i)
		n.push_back(normals[i]);

	for (int j = 0; j < facenum; ++j)
	{
		i.push_back(face[j].x);
		i.push_back(face[j].y);
		i.push_back(face[j].z);
		ni.push_back(normalidx[j].x);
		ni.push_back(normalidx[j].y);
		ni.push_back(normalidx[j].z);
	}

	unsigned int tmpidx[] =
	{ 1, 5, 3, 4, 3, 8,
	  8, 7, 6, 6, 2, 8,
	  2, 1, 4, 6, 5, 2,
	  2, 5, 1, 6, 7, 5,
	  8, 2, 4, 3, 5, 7,
	  4, 1, 3, 8, 3, 7 };

	glm::vec3 BB[] = {
	{0.5f, 0.5f, -0.5f  },
	{0.5f, -0.5f, -0.5f },
	{0.5f, 0.5f, 0.5f   },
	{0.5f, -0.5f, 0.5f  },
	{-0.5f, 0.5f, -0.5f },
	{-0.5f, -0.5f, -0.5f},
	{-0.5f, 0.5f, 0.5f  },
	{-0.5f, -0.5f, 0.5f }
	};

	for (int j = 0; j < 36; ++j)
		i.push_back(tmpidx[j] + v.size());

	for (int i = 0; i < 8; ++i)
	{
		v.push_back(BB[i]);
		c.push_back({ 0.0f, 0.0f, 0.0f });
	}*/

	InitBuffer();
	UpdateMatrix();

	fclose(path);
}

void Object::UpdateMatrix()
{
	matrix = glm::mat4(1.0f);

	// �� ���� �������� �̵�
	matrix = glm::translate(matrix, rotBy);
	matrix = glm::rotate(matrix, glm::radians(rotByAngle.z), { 0.0f, 0.0f, 1.0f });
	matrix = glm::rotate(matrix, glm::radians(rotByAngle.y), { 0.0f, 1.0f, 0.0f });
	matrix = glm::rotate(matrix, glm::radians(rotByAngle.x), { 1.0f, 0.0f, 0.0f });
	matrix = glm::translate(matrix, -rotBy);

	// ��ü �̵�
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

void Object::InitBuffer()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);

	/*glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * i.size(), i.data(), GL_STATIC_DRAW);*/
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * v.size(), v.data(), GL_STATIC_DRAW);

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * ni.size(), ni.data(), GL_STATIC_DRAW);*/

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)0); //--- ��ġ �Ӽ�
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)(sizeof(glm::vec3))); //--- �븻 �Ӽ�
	glEnableVertexAttribArray(1);

	
}