#include "stdafx.h"
#include "object.h"

object::object(int sides, float size)
{ 
	S = { size, size, size };

	// 어디서 튀어 나올지 정하는 녀석
	float x, y;
	dir.x = uidD(dre) * 4;
	dir.y = uidD(dre) * 4;
	while (abs(dir.x) < 0.2f)
		dir.x = uidD(dre) * 4;

	if (dir.y < 0.0f)
		dir.y *= -1; // 무조건 위 방향으로 가게

	if (dir.x < 0.0f) // 왼쪽 방향으로 가는 녀석은 오른쪽에서 발사
	{
		T.x = 1.0f + size;
		T.y = uidY(dre);
	}
	else // 오른쪽으로 가는 녀석은 왼쪽에서 발사
	{
		T.x = -1.0f - size;
		T.y = uidY(dre);
	}

	color = { uidC(dre), uidC(dre), uidC(dre)};
	for (int i = 0; i < sides; ++i)
	{
		GLfloat angle = 2.0 * 3.14f * i / sides;
		glm::vec3 tmp = { cos(angle), sin(angle), 0.0f };
		v.emplace_back(tmp);
		c.emplace_back(color);
	}

	InitBuffer();
}

object::object(vector<glm::vec3> input, glm::vec2 d, glm::vec3 s, glm::vec3 r, glm::vec3 t)
{
	v = input;
	dir = d;
	S = s;
	R = r;
	T = t;
	sortVertex();
	color = { uidC(dre), uidC(dre), uidC(dre) };
	for (int i = 0; i < input.size(); ++i)
		c.push_back(color);

	InitBuffer();
}

object::object(vector<glm::vec3> pts)
{
	v = pts;
	for (int i = 0; i < pts.size(); ++i)
		c.push_back({ 1.0f, 1.0f, 1.0f });
	InitBuffer();
}

vector<glm::vec3> object::getRoute()
{
	vector<glm::vec3> result;

	float tmpG = gravity;
	float tmpX = T.x;
	float tmpY = T.y;

	while (true)
	{
		// 점들중 y좌표가 가장 큰 녀석이 -1보다 밑으로 내려갈 때까지
		if (tmpY < -1.0f) break;

		tmpG += (1.0f / 400.);
		tmpX += 2.0f;
		tmpX += dir.x * (20 / 400.);
		tmpX -= 2.0f;
		tmpY += 2.0f;
		tmpY += dir.y * (20 / 400.);
		tmpY -= 2.0f;
		tmpY -= tmpG;

		result.push_back({tmpX, tmpY, 0.0f});
	}

	return result;
}

void object::sortVertex() // 이렇게 해도 되려나..
{
	int temp = 0;
	for (int i = 1; i < v.size(); i++)
		if (v[i].y < v[temp].y || (v[i].y == v[temp].y && v[i].x < v[temp].x))
			temp = i;

	sort(v.begin() + 1, v.end(), [this](glm::vec3 a, glm::vec3 b) {
	int val = ccw(v[0], a, b);
	if (val > 0) return true;
	if (val < 0) return false;
	if (dist(v[0], a) < dist(v[0], b)) return true;
	return false; });//기준점을 제외한 점을 정렬시킵니다.
}

void object::render(GLuint shaderProgramID, bool isLine)
{
	glBindVertexArray(vao);

	update();

	// Location 번호 저장
	unsigned int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position"); //	: 0  Shader의 'layout (location = 0)' 부분
	unsigned int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color"); //	: 1
	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "transform");

	glEnableVertexAttribArray(PosLocation); // Enable 필수! 사용하겠단 의미
	glEnableVertexAttribArray(ColorLocation);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // 정점 좌표용 VBO 바인딩
	glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // 색상 데이터용 VBO 바인딩
	glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(matrix));

	if (isLine)
	{
		glLineWidth(1.0f);
		glDrawArrays(GL_LINES, 0, v.size());
	}
	else
	{
		if (isSolid)
			glDrawArrays(GL_POLYGON, 0, v.size());
		else
			glDrawArrays(GL_LINE_LOOP, 0, v.size());
	}

	glDisableVertexAttribArray(PosLocation); // Disable 필수!
	glDisableVertexAttribArray(ColorLocation);
}

void object::update()
{
	matrix = glm::mat4(1.0f);
	matrix = glm::translate(matrix, T);
	matrix = glm::rotate(matrix, glm::radians(R.z), { 0.0f, 0.0f, 1.0f });
	matrix = glm::scale(matrix, S);
}

void object::remove()
{
	c.clear();
	v.clear();
}

void object::move()
{
	// 회전
	R.z += angle;

	// 이동
	gravity += (1.0f / 400.);
	T.x += 2.0f;
	T.x += dir.x * (20 / 400.);
	T.x -= 2.0f;
	T.y += 2.0f;
	T.y += dir.y * (20 / 400.);
	T.y -= 2.0f;
	T.y -= gravity;
}

void object::move(glm::vec2 d)
{
	dir = d;

	T.x += dir.x * (10 / 400.);
}

pair<bool, pair<vector<glm::vec3>, vector<glm::vec3>>> object::isCross(glm::vec3 pt1, glm::vec3 pt2) // 헐 뇌 정지 옴 받아오는건 마우스 좌표임
{
	bool breturn = false;
	vector<glm::vec3> result;
	vector<glm::vec3> left;
	vector<glm::vec3> right;

	if (isput)
		return { breturn, {left, right} };

	for (int i = 0; i < v.size(); ++i)
	{
		glm::vec3 pt3 = updateMatrix(v[i]);
		int idx3 = i;
		glm::vec3 pt4;
		int idx4 = -1;

		if (i + 1 == v.size())
		{
			pt4 = updateMatrix(v[0]);
			idx4 = 0;
		}
		else
		{
			pt4 = updateMatrix(v[i + 1]);
			idx4 = i + 1;
		}

		double t;
		double s;
		double under = (pt2.y - pt1.y) * (pt4.x - pt3.x) - (pt2.x - pt1.x) * (pt4.y - pt3.y);
		if (under == 0) continue;

		double _t = (pt2.x - pt1.x) * (pt3.y - pt1.y) - (pt2.y - pt1.y) * (pt3.x - pt1.x);
		double _s = (pt4.x - pt3.x) * (pt3.y - pt1.y) - (pt4.y - pt3.y) * (pt3.x - pt1.x);

		t = _t / under;
		s = _s / under;

		if (t < 0.0 || t > 1.0 || s < 0.0 || s > 1.0) continue;
		if (_t == 0 && _s == 0) continue;

		glm::vec3 p3 = glm::affineInverse(matrix) * glm::vec4{pt3, 1.0f};
		glm::vec3 p4 = glm::affineInverse(matrix) * glm::vec4{pt4, 1.0f};

		result.push_back({ p3.x + t * (double)(p4.x - p3.x),  p3.y + t * (double)(p4.y - p3.y), 0.0f });

		breturn = true;
	}

	if (result.size() == 0) breturn = false;
	else if (result.size() == 1) breturn = false;
	else
	{
		for (int i = 0; i < v.size(); ++i)
		{
			float tmp = (v[i].x - result[0].x) * (result[1].y - result[0].y) - (v[i].y - result[0].y) * (result[1].x - result[0].x);
			if (tmp > 0) left.push_back(v[i]);
			else if (tmp < 0) right.push_back(v[i]);
		}
		left.push_back(result[1]);
		right.push_back(result[1]);
		left.push_back(result[0]);
		right.push_back(result[0]);
	}

	return { breturn, {left, right} };
}

bool object::isRemove()// 고쳐야댐
{
	vector<glm::vec3> result;

	for (int i = 0; i < v.size(); ++i)
	{
		result.push_back(updateMatrix(v[i]));
	}

	auto a = max_element(result.begin(), result.end(), [](const glm::vec3& a, const glm::vec3& b) {return a.y < b.y; });
	
	if (a->y <= -1.0f) 
		return true;
	return false;
}

bool object::isIntersect(glm::vec3 pt, glm::vec2 d) // 바구니랑 도형이랑 닿았는지?
{
	if (isput || !isSliced) return false;
	glm::vec3 tmp{0.0f,0.0f,0.0f};
	
	for (int i = 0; i < v.size(); ++i)
	{
		glm::vec4 tmppt(v[i], 1.0f);
		glm::vec3 result(matrix * tmppt);

		tmp += result;
	}

	tmp.x /= float(v.size());
	tmp.y /= float(v.size());

	if (tmp.y < pt.y && (tmp.x < pt.x + (200 / 400.0f) && tmp.x > pt.x))
	{
		score++;
		isput = true;
		dir.x = d.x;
		dir.y = 0.0f;
		return true;
	}
	return false;
}

void object::InitBuffer()
{
	glGenVertexArrays(1, &vao); // VAO를 생성하고 할당합니다.
	glBindVertexArray(vao); // VAO를 바인드합니다.
	glGenBuffers(2, vbo); // 2개의 VBO를 생성하고 할당합니다.

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * v.size(), v.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * c.size(), c.data(), GL_STATIC_DRAW);

	// 정점 좌표 데이터를 VAO에 바인딩하고 활성화합니다.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// 색상 데이터를 VAO에 바인딩하고 활성화합니다.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}