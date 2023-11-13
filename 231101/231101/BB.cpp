#include "stdafx.h"
#include "BB.h"

BB::BB(int t = 1)
	: type{t}
{
	if (t == 1)
		Readobj("cube.obj");
	else if (t == 2)
		Readobj("sphere.obj");
}

void BB::Update(glm::mat4 m)
{
	matrix = m;

	// centerpos ������Ʈ
	vCenterPos = glm::vec3(matrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	vAxisDir[0] = { 1.0f, 0.0f, 0.0f };
	vAxisDir[1] = { 0.0f, 1.0f, 0.0f };
	vAxisDir[2] = { 0.0f, 0.0f, 1.0f };

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

void BB::Render(GLuint shaderProgramID)
{
	glBindVertexArray(vao);

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	view = glm::rotate(view, glm::radians(cameraAngle.z), { 0.0f, 0.0f, 1.0f });
	view = glm::rotate(view, glm::radians(cameraAngle.y), { 0.0f, 1.0f, 0.0f });
	view = glm::rotate(view, glm::radians(cameraAngle.x), { 1.0f, 0.0f, 0.0f });

	// Location ��ȣ ����
	unsigned int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position"); //	: 0  Shader�� 'layout (location = 0)' �κ�
	unsigned int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color"); //	: 1
	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "transform");
	unsigned int viewLocation = glGetUniformLocation(shaderProgramID, "view");
	unsigned int projLocation = glGetUniformLocation(shaderProgramID, "projection");

	glEnableVertexAttribArray(PosLocation); // Enable �ʼ�! ����ϰڴ� �ǹ�
	glEnableVertexAttribArray(ColorLocation);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // ���� ��ǥ�� VBO ���ε�
	glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // ���� �����Ϳ� VBO ���ε�
	glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(matrix));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &proj[0][0]);

	// ���⼭ �׸���
	glDrawElements(GL_LINES, i.size(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(PosLocation); // Disable �ʼ�!
	glDisableVertexAttribArray(ColorLocation);
}

void BB::Readobj(const char* s)
{
	FILE* path = fopen(s, "r");
	if (path == NULL)
		return;

	char count[128];
	int vertexnum = 0;
	int facenum = 0;
	int uvnum = 0;
	//--- 1. ��ü ���ؽ� ���� �� �ﰢ�� ���� ����
	while (!feof(path))
	{
		fscanf(path, "%s", count);
		if (count[0] == 'v' && count[1] == '\0')
			vertexnum++;
		else if (count[0] == 'f' && count[1] == '\0')
			facenum++;
		else if (count[0] == 'v' && count[1] == 't' && count[3] == '\0')
			uvnum++;
		memset(count, '\0', sizeof(count));
	}
	rewind(path);
	int vertIndex = 0;
	int faceIndex = 0;
	int uvIndex = 0;
	//--- 2. �޸� �Ҵ�
	glm::vec3* vertex = new glm::vec3[vertexnum];
	glm::vec3* face = new glm::vec3[facenum];
	glm::vec3* uvdata = new glm::vec3[facenum];
	glm::vec2* uv = new glm::vec2[uvnum];
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
			fscanf(path, "%d//%d %d//%d %d//%d\n",
				&temp_face[0], &temp_normal[0],
				&temp_face[1], &temp_normal[1],
				&temp_face[2], &temp_normal[2]);
			face[faceIndex].x = temp_face[0] - 1;
			face[faceIndex].y = temp_face[1] - 1;
			face[faceIndex].z = temp_face[2] - 1;
			uvdata[faceIndex].x = temp_uv[0];
			uvdata[faceIndex].y = temp_uv[1];
			uvdata[faceIndex].z = temp_uv[2];
			faceIndex++;
		}
		else if (bind[0] == 'v' && bind[1] == 't' && bind[2] == '\0') {
			fscanf(path, "%f %f\n", &uv[uvIndex].x, &uv[uvIndex].y);
			uvIndex++;
		}
	}

	for (int i = 0; i < vertexnum; ++i)
	{
		v.push_back(vertex[i]);
		c.push_back({ 0.0f, 0.0f, 0.0f });
	}

	for (int j = 0; j < facenum; ++j)
	{
		i.push_back(face[j].x);
		i.push_back(face[j].y);
		i.push_back(face[j].z);
	}

	InitBuffer();

	fclose(path);
}

void BB::InitBuffer()
{
	glGenVertexArrays(1, &vao); // VAO�� �����ϰ� �Ҵ��մϴ�.
	glBindVertexArray(vao); // VAO�� ���ε��մϴ�.
	glGenBuffers(2, vbo); // 2���� VBO�� �����ϰ� �Ҵ��մϴ�.

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * i.size(), i.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * v.size(), v.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * c.size(), c.data(), GL_DYNAMIC_DRAW);

	// ���� ��ǥ �����͸� VAO�� ���ε��ϰ� Ȱ��ȭ�մϴ�.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// ���� �����͸� VAO�� ���ε��ϰ� Ȱ��ȭ�մϴ�.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}