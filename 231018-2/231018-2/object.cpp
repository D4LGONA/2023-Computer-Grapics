#include "stdafx.h"
#include "object.h"

object::object(const char* c)
{
	readobj(c);
}

void object::update()
{
	if (isSpinY)
	{
		if (isSpinPlus)
			rotation.y += 5.0f;
		else
			rotation.y -= 5.0f;
	}
	bigspin += tbigspin;

	if (isSpinZ)
		rotation.z += 5.0f;
	
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

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(matrix));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &proj[0][0]);
	glColor3f(1.0f, 1.0f, 1.0f);
	gluSphere(o, radius, 20, 20);

	glDisableVertexAttribArray(PosLocation); // Disable 필수!
	glDisableVertexAttribArray(ColorLocation);
}

object::object(float r, glm::vec3 t, glm::vec3 R, float speed) : radius{ r }, transition{ t }, tbigspin{speed}
{
	rotation = { R.x, R.y, -R.z };
	orbit = R.z;
	o = gluNewQuadric();
	gluQuadricDrawStyle(o, GLU_LINE);
}

//srt가 부모, s2t2r2가 할머니
void object::setmatrix(glm::vec3 r, glm::vec3 t, glm::vec3 s, glm::vec3 r2, glm::vec3 t2, glm::vec3 s2, float orbit )
{
	matrix = glm::mat4(1.0f);


	matrix = glm::translate(matrix, t);
	matrix = glm::rotate(matrix, glm::radians(bigspin), { 0.0f,1.0f,0.0f });

	// 나
	matrix = glm::translate(matrix, transition);
	matrix = glm::rotate(matrix, glm::radians(rotation.z), { 0.0f,0.0f,1.0f });
	matrix = glm::rotate(matrix, glm::radians(rotation.y), { 0.0f,1.0f,0.0f });
	matrix = glm::rotate(matrix, glm::radians(rotation.x), { 1.0f,0.0f,0.0f });
	matrix = glm::scale(matrix, scale);
}

void object::setmatrix(glm::mat4 p)
{
	matrix = glm::mat4(1.0f);
	glm::vec3 t;
	t.x = p[3][0] + transition.x;
	t.y = p[3][1] + transition.y;
	t.z = p[3][2] + transition.z;

	matrix = glm::translate(matrix, {p[3][0], p[3][1], p[3][2]});
	matrix = glm::rotate(matrix, glm::radians(rotZ), { 0.0f,0.0f,1.0f });
	matrix = glm::rotate(matrix, glm::radians(orbit), { 0.0f,0.0f,1.0f });
	matrix = glm::rotate(matrix, glm::radians(bigspin), { 0.0f,1.0f,0.0f });
	matrix = glm::translate(matrix, {-p[3][0], -p[3][1], -p[3][2]});

	matrix = glm::translate(matrix, t);
	// 나
	matrix = glm::translate(matrix, transition);
	matrix = glm::rotate(matrix, glm::radians(rotation.z), { 0.0f,0.0f,1.0f });
	matrix = glm::rotate(matrix, glm::radians(rotation.y), { 0.0f,1.0f,0.0f });
	matrix = glm::rotate(matrix, glm::radians(rotation.x), { 1.0f,0.0f,0.0f });
	matrix = glm::scale(matrix, scale);
}


void object::setmatrix(glm::vec3 r, glm::vec3 t, glm::vec3 s)
{
	matrix = glm::mat4(1.0f);
	
	matrix = glm::rotate(matrix, glm::radians(rotZ), { 0.0f,0.0f,1.0f });
	// 내 부모
	matrix = glm::translate(matrix, t);
	matrix = glm::rotate(matrix, glm::radians(orbit), { 0.0f,0.0f,1.0f });
	matrix = glm::rotate(matrix, glm::radians(bigspin), { 0.0f,1.0f,0.0f });
	matrix = glm::rotate(matrix, glm::radians(r.z), { 0.0f,0.0f,1.0f });
	matrix = glm::rotate(matrix, glm::radians(r.y), { 0.0f,1.0f,0.0f });
	matrix = glm::rotate(matrix, glm::radians(r.x), { 1.0f,0.0f,0.0f });
	matrix = glm::translate(matrix, -t);

	//  나
	matrix = glm::translate(matrix, transition);
	matrix = glm::rotate(matrix, glm::radians(rotation.z), { 0.0f,0.0f,1.0f });
	matrix = glm::rotate(matrix, glm::radians(rotation.y), { 0.0f,1.0f,0.0f });
	matrix = glm::rotate(matrix, glm::radians(rotation.x), { 1.0f,0.0f,0.0f });
	matrix = glm::scale(matrix, scale);
}

void object::setmatrix()
{
	matrix = glm::mat4(1.0f);

	matrix = glm::rotate(matrix, glm::radians(rotZ), { 0.0f,0.0f,1.0f });

	matrix = glm::translate(matrix, transition);
	matrix = glm::rotate(matrix, glm::radians(rotation.z), { 0.0f,0.0f,1.0f });
	matrix = glm::rotate(matrix, glm::radians(rotation.y), { 0.0f,1.0f,0.0f });
	matrix = glm::rotate(matrix, glm::radians(rotation.x), { 1.0f,0.0f,0.0f });
	matrix = glm::scale(matrix, scale);
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

void object::bigSpin(glm::vec3 s, glm::vec3 r, glm::vec3 t)
{
	
}
