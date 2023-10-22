#include "stdafx.h"
#include "object.h"

object::object(const char* c)
{
	readobj(c);
}

void object::update()
{
	for (int i = 0; i < matrix.size(); ++i)
		matrix[i] = glm::mat4(1.0f);

	if (rotating)
		for(int i = 0; i < rotation.size(); ++i)
			rotation[i].y += 5.0f;

	if (topRotate)
		rotation[1].y += 5.0f;

	if (openF)
	{
		if (openRot < 90.0f)
			openRot += 5.0f;
	}
	else
	{
		if (openRot > 0.0f)
			openRot -= 5.0f;
		else
			openRot = 0.0f;
	}
	
	if (moveSide)
	{
		if (transition[4].y < 1.0f)
		{
			transition[4].y += 0.1f;
			transition[5].y += 0.1f;
		}
		else
		{
			transition[4].y = 1.0f;
			transition[5].y = 1.0f;
		}
	}
	else
	{
		if(transition[4].y > 0.0f)
		{
			transition[4].y -= 0.1f;
			transition[5].y -= 0.1f;
		}
		else
		{
			transition[4].y = 0.0f;
			transition[5].y = 0.0f;
		}
	}

	if (backS)
	{
		if (backSc > 0.0f)
			backSc -= 0.1f;
		else
			backSc = 0.0f;
	}
	else
	{
		if (backSc < 0.5f)
			backSc += 0.1f;
		else
			backSc = 0.5f;
	}

	for (int i = 0; i < 6; ++i)
	{
		if (isOpen[i].first)
		{
			if (openall)
			{
				if (isOpen[i].second < 235.0f)
					isOpen[i].second += 5.0f;
				else
					isOpen[i].second = 235.0f;
			}
			else
			{
				if (isOpen[i].second < 90.0f)
					isOpen[i].second += 5.0f;
				else
					isOpen[i].second = 90.0f;
			}
		}
			
	}

	for (int i = 0; i < 6; ++i)
	{
		if (!isOpen[i].first)
			if (isOpen[i].second > 0.0f)
				isOpen[i].second -= 5.0f;
			else
				isOpen[i].second = 0.0f;
	}
	

	if (v.size() > 20)
	{
		for (int i = 0; i < matrix.size(); ++i)
		{

			matrix[i] = glm::translate(matrix[i], transition[i]);
			matrix[i] = glm::rotate(matrix[i], glm::radians(rotation[i].x), glm::vec3(1.0f, 0.0f, 0.0f));
			matrix[i] = glm::rotate(matrix[i], glm::radians(rotation[i].y), glm::vec3(0.0f, 1.0f, 0.0f));
			matrix[i] = glm::rotate(matrix[i], glm::radians(rotation[i].z), glm::vec3(0.0f, 0.0f, 1.0f));

			if (backSc != 0.5f && i == 0)
			{
				matrix[0] = glm::translate(matrix[0], { 0.0f, 0.0f, -0.5f });
				matrix[0] = glm::scale(matrix[0], { backSc, backSc, backSc });
				matrix[0] = glm::translate(matrix[0], { 0.0f, 0.0f, 0.5f });
			}
			else
			{
				matrix[i] = glm::scale(matrix[i], scale[i]);
			}

			if (openRot > 0.0f && i == 2)
			{
				matrix[2] = glm::translate(matrix[2], {0.0f, -1.0f, 1.0f });
				matrix[2] = glm::rotate(matrix[2], glm::radians(openRot), glm::vec3(1.0f, 0.0f, 0.0f));
				matrix[2] = glm::translate(matrix[2], {0.0f, 1.0f, -1.0f });
			}
		}
	}
	else
	{
		for(int i = 0 ; i < matrix.size(); ++i)
		{
			matrix[i] = glm::translate(matrix[i], transition[i]);
			matrix[i] = glm::rotate(matrix[i], glm::radians(rotation[i].x), glm::vec3(1.0f, 0.0f, 0.0f));
			matrix[i] = glm::rotate(matrix[i], glm::radians(rotation[i].y), glm::vec3(0.0f, 1.0f, 0.0f));
			matrix[i] = glm::rotate(matrix[i], glm::radians(rotation[i].z), glm::vec3(0.0f, 0.0f, 1.0f));
			matrix[i] = glm::scale(matrix[i], scale[i]);

			switch (i)
			{
			case 0:
				matrix[i] = glm::translate(matrix[i], { 0.0f, -1.0f, -1.0f });
				matrix[i] = glm::rotate(matrix[i], glm::radians(-isOpen[i].second), glm::vec3(1.0f, 0.0f, 0.0f));
				matrix[i] = glm::translate(matrix[i], { 0.0f, 1.0f, 1.0f });
				break;
			case 1:
				matrix[i] = glm::translate(matrix[i], { 0.0f, -1.0f, 1.0f });
				matrix[i] = glm::rotate(matrix[i], glm::radians(isOpen[i].second), glm::vec3(1.0f, 0.0f, 0.0f));
				matrix[i] = glm::translate(matrix[i], { 0.0f, 1.0f, -1.0f });
				break;
			case 4:
				matrix[i] = glm::translate(matrix[i], { -1.0f, -1.0f, 0.0f });
				matrix[i] = glm::rotate(matrix[i], glm::radians(isOpen[i].second), glm::vec3(0.0f, 0.0f, 1.0f));
				matrix[i] = glm::translate(matrix[i], { 1.0f, 1.0f, 0.0f });
				break;
			case 5:
				matrix[i] = glm::translate(matrix[i], { 1.0f, -1.0f, 0.0f });
				matrix[i] = glm::rotate(matrix[i], glm::radians(-isOpen[i].second), glm::vec3(0.0f, 0.0f, 1.0f));
				matrix[i] = glm::translate(matrix[i], { -1.0f, 1.0f, 0.0f });
				break;
			default:
				break;
			}
		}
	}
}

void object::render(GLuint vbo[], GLuint ebo, GLuint shaderProgramID)
{
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 2.0f); //--- 카메라 위치
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	// Location 번호 저장
	unsigned int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position"); //	: 0  Shader의 'layout (location = 0)' 부분
	unsigned int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color"); //	: 1
	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "transform");
	unsigned int viewLocation = glGetUniformLocation(shaderProgramID, "view");
	unsigned int projLocation = glGetUniformLocation(shaderProgramID, "projection");

	glEnableVertexAttribArray(PosLocation); // Enable 필수! 사용하겠단 의미
	glEnableVertexAttribArray(ColorLocation);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // 정점 좌표용 VBO 바인딩
	glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // 색상 데이터용 VBO 바인딩
	glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);


	if (v.size() < 20)
	{
		for (int i = 0; i < v.size(); i += 3)
		{
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(matrix[i / 3]));
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
			glUniformMatrix4fv(projLocation, 1, GL_FALSE, &proj[0][0]);
			glDrawArrays(GL_TRIANGLES, i, 3);
		}
	}
	else
	{
		for (int i = 0; i < v.size(); i += 6)
		{
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(matrix[(i)/6]));
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
			glUniformMatrix4fv(projLocation, 1, GL_FALSE, &proj[0][0]);
			glDrawArrays(GL_TRIANGLES, i,6);
		}
	}

	glDisableVertexAttribArray(PosLocation); // Disable 필수!
	glDisableVertexAttribArray(ColorLocation);
}

object::object(float fx, float fy, float fz, int s, GLuint vbo[])
{
	int i = 0;
	if (s == 0)
	{
		v.resize(36);
		c.resize(36);
		// 여기 뒤
		rotation.push_back({ 0.0f,0.0f,0.0f });
		transition.push_back({ 0.0f,0.0f,0.0f });
		scale.push_back({ 0.5f,0.5f,0.5f });
		matrix.push_back({ glm::mat4(1.0f) });
		c[i] = glm::vec3(1.0f, 0.0f, 0.0f);
		v[i++] = glm::vec3(-fx, +fy, -fz);
		c[i] = glm::vec3(1.0f, 0.0f, 0.0f);
		v[i++] = glm::vec3(+fx, +fy, -fz);
		c[i] = glm::vec3(1.0f, 0.0f, 0.0f);
		v[i++] = glm::vec3(+fx, -fy, -fz);

		c[i] = glm::vec3(1.0f, 0.0f, 0.0f);
		v[i++] = glm::vec3(-fx, +fy, -fz);
		c[i] = glm::vec3(1.0f, 0.0f, 0.0f);
		v[i++] = glm::vec3(+fx, -fy, -fz);
		c[i] = glm::vec3(1.0f, 0.0f, 0.0f);
		v[i++] = glm::vec3(-fx, -fy, -fz);

		// 여기 윗면
		rotation.push_back({ 0.0f,0.0f,0.0f });
		transition.push_back({ 0.0f,0.0f,0.0f });
		scale.push_back({ 0.5f,0.5f,0.5f });
		matrix.push_back({ glm::mat4(1.0f) });
		c[i] = glm::vec3(0.0f, 1.0f, 0.0f);
		v[i++] = glm::vec3(-fx, +fy, +fz);
		c[i] = glm::vec3(0.0f, 1.0f, 0.0f);
		v[i++] = glm::vec3(+fx, +fy, +fz);
		c[i] = glm::vec3(0.0f, 1.0f, 0.0f);
		v[i++] = glm::vec3(+fx, +fy, -fz);

		c[i] = glm::vec3(0.0f, 1.0f, 0.0f);
		v[i++] = glm::vec3(-fx, +fy, +fz);
		c[i] = glm::vec3(0.0f, 1.0f, 0.0f);
		v[i++] = glm::vec3(+fx, +fy, -fz);
		c[i] = glm::vec3(0.0f, 1.0f, 0.0f);
		v[i++] = glm::vec3(-fx, +fy, -fz);

		// 여기 앞면
		rotation.push_back({ 0.0f,0.0f,0.0f });
		transition.push_back({ 0.0f,0.0f,0.0f });
		scale.push_back({ 0.5f,0.5f,0.5f });
		matrix.push_back({ glm::mat4(1.0f) });
		c[i] = glm::vec3(0.0f, 0.0f, 1.0f);
		v[i++] = glm::vec3(-fx, -fy, +fz);
		c[i] = glm::vec3(0.0f, 0.0f, 1.0f);
		v[i++] = glm::vec3(+fx, -fy, +fz);
		c[i] = glm::vec3(0.0f, 0.0f, 1.0f);
		v[i++] = glm::vec3(+fx, +fy, +fz);

		c[i] = glm::vec3(0.0f, 0.0f, 1.0f);
		v[i++] = glm::vec3(-fx, -fy, +fz);
		c[i] = glm::vec3(0.0f, 0.0f, 1.0f);
		v[i++] = glm::vec3(+fx, +fy, +fz);
		c[i] = glm::vec3(0.0f, 0.0f, 1.0f);
		v[i++] = glm::vec3(-fx, +fy, +fz);

		//// 여기 아래
		rotation.push_back({ 0.0f,0.0f,0.0f });
		transition.push_back({ 0.0f,0.0f,0.0f });
		scale.push_back({ 0.5f,0.5f,0.5f });
		matrix.push_back({ glm::mat4(1.0f) });
		c[i] = glm::vec3(1.0f, 1.0f, 0.0f);
		v[i++] = glm::vec3(-fx, -fy, -fz);
		c[i] = glm::vec3(1.0f, 1.0f, 0.0f);
		v[i++] = glm::vec3(+fx, -fy, -fz);
		c[i] = glm::vec3(1.0f, 1.0f, 0.0f);
		v[i++] = glm::vec3(+fx, -fy, +fz);

		c[i] = glm::vec3(1.0f, 1.0f, 0.0f);
		v[i++] = glm::vec3(-fx, -fy, -fz);
		c[i] = glm::vec3(1.0f, 1.0f, 0.0f);
		v[i++] = glm::vec3(+fx, -fy, +fz);
		c[i] = glm::vec3(1.0f, 1.0f, 0.0f);
		v[i++] = glm::vec3(-fx, -fy, +fz);

		//// 여기 왼쪽
		rotation.push_back({ 0.0f,0.0f,0.0f });
		transition.push_back({ 0.0f,0.0f,0.0f });
		scale.push_back({ 0.5f,0.5f,0.5f });
		matrix.push_back({ glm::mat4(1.0f) });
		c[i] = glm::vec3(1.0f, 0.0f, 1.0f);
		v[i++] = glm::vec3(-fx, +fy, +fz);
		c[i] = glm::vec3(1.0f, 0.0f, 1.0f);
		v[i++] = glm::vec3(-fx, +fy, -fz);
		c[i] = glm::vec3(1.0f, 0.0f, 1.0f);
		v[i++] = glm::vec3(-fx, -fy, -fz);

		c[i] = glm::vec3(1.0f, 0.0f, 1.0f);
		v[i++] = glm::vec3(-fx, +fy, +fz);
		c[i] = glm::vec3(1.0f, 0.0f, 1.0f);
		v[i++] = glm::vec3(-fx, -fy, -fz);
		c[i] = glm::vec3(1.0f, 0.0f, 1.0f);
		v[i++] = glm::vec3(-fx, -fy, +fz);

		//// 여기 오른쪽
		rotation.push_back({ 0.0f,0.0f,0.0f });
		transition.push_back({ 0.0f,0.0f,0.0f });
		scale.push_back({ 0.5f,0.5f,0.5f });
		matrix.push_back({ glm::mat4(1.0f) });
		c[i] = glm::vec3(0.0f, 1.0f, 1.0f);
		v[i++] = glm::vec3(+fx, +fy, -fz);
		c[i] = glm::vec3(0.0f, 1.0f, 1.0f);
		v[i++] = glm::vec3(+fx, +fy, +fz);
		c[i] = glm::vec3(0.0f, 1.0f, 1.0f);
		v[i++] = glm::vec3(+fx, -fy, +fz);

		c[i] = glm::vec3(0.0f, 1.0f, 1.0f);
		v[i++] = glm::vec3(+fx, +fy, -fz);
		c[i] = glm::vec3(0.0f, 1.0f, 1.0f);
		v[i++] = glm::vec3(+fx, -fy, +fz);
		c[i] = glm::vec3(0.0f, 1.0f, 1.0f);
		v[i++] = glm::vec3(+fx, -fy, -fz);
	}
	else
	{
		v.resize(18);
		c.resize(18);
		// 여기 뒤
		rotation.push_back({ 0.0f,0.0f,0.0f });
		transition.push_back({ 0.0f,0.0f,0.0f });
		scale.push_back({ 0.5f,0.5f,0.5f });
		matrix.push_back({ glm::mat4(1.0f) });
		c[i] = glm::vec3(1.0f, 1.0f, 0.0f);
		v[i++] = glm::vec3(0.0f, +fy, 0.0f);
		c[i] = glm::vec3(1.0f, 1.0f, 0.0f);
		v[i++] = glm::vec3(+fx, -fy, -fz);
		c[i] = glm::vec3(1.0f, 1.0f, 0.0f);
		v[i++] = glm::vec3(-fx, -fy, -fz);

		// 여기 윗면 x

		// 여기 앞면
		rotation.push_back({ 0.0f,0.0f,0.0f });
		transition.push_back({ 0.0f,0.0f,0.0f });
		scale.push_back({ 0.5f,0.5f,0.5f });
		matrix.push_back({ glm::mat4(1.0f) });
		c[i] = glm::vec3(1.0f, 0.0f, 1.0f);
		v[i++] = glm::vec3(-fx, -fy, +fz);
		c[i] = glm::vec3(1.0f, 0.0f, 1.0f);
		v[i++] = glm::vec3(+fx, -fy, +fz);
		c[i] = glm::vec3(1.0f, 0.0f, 1.0f);
		v[i++] = glm::vec3(0.0f, +fy, 0.0f);

		// 여기 아래면
		rotation.push_back({ 0.0f,0.0f,0.0f });
		transition.push_back({ 0.0f,0.0f,0.0f });
		scale.push_back({ 0.5f,0.5f,0.5f });
		matrix.push_back({ glm::mat4(1.0f) });
		c[i] = glm::vec3(0.0f, 1.0f, 1.0f);
		v[i++] = glm::vec3(-fx, -fy, -fz);
		c[i] = glm::vec3(0.0f, 1.0f, 1.0f);
		v[i++] = glm::vec3(+fx, -fy, -fz);
		c[i] = glm::vec3(0.0f, 1.0f, 1.0f);
		v[i++] = glm::vec3(+fx, -fy, +fz);

		rotation.push_back({ 0.0f,0.0f,0.0f });
		transition.push_back({ 0.0f,0.0f,0.0f });
		scale.push_back({ 0.5f,0.5f,0.5f });
		matrix.push_back({ glm::mat4(1.0f) });
		c[i] = glm::vec3(0.0f, 1.0f, 1.0f);
		v[i++] = glm::vec3(-fx, -fy, -fz);
		c[i] = glm::vec3(0.0f, 1.0f, 1.0f);
		v[i++] = glm::vec3(+fx, -fy, +fz);
		c[i] = glm::vec3(0.0f, 1.0f, 1.0f);
		v[i++] = glm::vec3(-fx, -fy, +fz);

		// 여기 왼쪽
		rotation.push_back({ 0.0f,0.0f,0.0f });
		transition.push_back({ 0.0f,0.0f,0.0f });
		scale.push_back({ 0.5f,0.5f,0.5f });
		matrix.push_back({ glm::mat4(1.0f) });
		c[i] = glm::vec3(0.0f, 1.0f, 0.0f);
		v[i++] = glm::vec3(0.0f, +fy, 0.0f);
		c[i] = glm::vec3(0.0f, 1.0f, 0.0f);
		v[i++] = glm::vec3(-fx, -fy, -fz);
		c[i] = glm::vec3(0.0f, 1.0f, 0.0f);
		v[i++] = glm::vec3(-fx, -fy, +fz);

		// 여기 오른쪽
		rotation.push_back({ 0.0f,0.0f,0.0f });
		transition.push_back({ 0.0f,0.0f,0.0f });
		scale.push_back({ 0.5f,0.5f,0.5f });
		matrix.push_back({ glm::mat4(1.0f) });
		c[i] = glm::vec3(0.0f, 0.0f, 1.0f);
		v[i++] = glm::vec3(0.0f, +fy, 0.0f);
		c[i] = glm::vec3(0.0f, 0.0f, 1.0f);
		v[i++] = glm::vec3(+fx, -fy, +fz);
		c[i] = glm::vec3(0.0f, 0.0f, 1.0f);
		v[i++] = glm::vec3(+fx, -fy, -fz);
	}

	for (glm::vec3& i : rotation)
	{
		i.x = 45.0f;
		i.y = 45.0f;
	}

	bind(vbo);
}

void object::remove()
{
	i.clear();
	v.clear();
	c.clear();
}

void object::readobj(const char* s)
{
	FILE* path = fopen(s, "r");
	if (path == NULL)
		return;

	char count[128];
	int vertexnum = 0;
	int facenum = 0;
	int uvnum = 0;
	//--- 1. 전체 버텍스 개수 및 삼각형 개수 세기
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
	//--- 2. 메모리 할당
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
		c.push_back({ uidC(dre),uidC(dre),uidC(dre) });
	}

	for (int j = 0; j < facenum; ++j)
	{
		i.push_back(face[j].x);
		i.push_back(face[j].y);
		i.push_back(face[j].z);
	}

	fclose(path);
}

void object::bind(GLuint vbo[])
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * v.size(), v.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * c.size(), c.data(), GL_DYNAMIC_DRAW);
}
