#include "stdafx.h"
#include "object.h"

object::object(int sides, int size)
{ 
	// 초기 위치랑 방향값 줘야 함
	// 중력 상수 정해야 함

	int x, y;
	dir.first = uidD(dre) * 2;
	dir.second = uidD(dre) * 2;
	if (dir.second > 0.0f)
		dir.second *= -1; // 무조건 위 방향으로 가게

	if (dir.first < 0.0f) // 왼쪽 방향으로 가는 녀석은 오른쪽에서 발사
	{
		x = 800 + size;
		y = uidY(dre);
	}
	else // 오른쪽으로 가는 녀석은 왼쪽에서 발사
	{
		x = -size;
		y = uidY(dre);
	}


	for (int i = 0; i < sides; ++i)
	{
		GLfloat angle = 2.0 * 3.14f * i / sides;
		POINT tmp = { x + size * cos(angle), y + size * sin(angle) };
		pts.emplace_back(tmp);
		color = { uidC(dre), uidC(dre), uidC(dre)};
	}
}

object::object(vector<POINT> input)
{
	pts = input;
	sortVertex();
	color = { uidC(dre), uidC(dre), uidC(dre) };
}

void object::sortVertex()
{
	int temp = 0;
	for (int i = 1; i < pts.size(); i++)
		if (pts[i].y < pts[temp].y || (pts[i].y == pts[temp].y && pts[i].x < pts[temp].x))
			temp = i;

	sort(pts.begin() + 1, pts.end(), [this](POINT a, POINT b) {
	int val = ccw(pts[0], a, b);
	if (val > 0) return true;
	if (val < 0) return false;
	if (dist(pts[0], a) < dist(pts[0], b)) return true;
	return false; });//기준점을 제외한 점을 정렬시킵니다.

	vector<POINT> res = pts;//그라함 스캔으로 볼록 다각형의 꼭지점을 찾아봅니다.
}

void object::sliceMove(int dir)
{
	// 잘랐을때 dir y축대칭 갈기면 될듯
	if (dir <= 0)
		for (POINT& i : pts)
			i.x -= 10;
	else
		for (POINT& i : pts)
			i.x += 10;
}

void object::render(GLuint vbo[])
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * pt.size(), pt.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * c.size(), c.data(), GL_DYNAMIC_DRAW);
	
	if(isSolid)
		glDrawArrays(GL_POLYGON, 0, pt.size());
	else
		glDrawArrays(GL_LINE_LOOP, 0, pt.size());
}

void object::update()
{
	pt.clear();
	c.clear();

	for (int i = 0; i < pts.size(); ++i)
	{
		POINT tmp = pts[i];
		pair<float, float> tmppt = WintoOpenGL(tmp);
		pt.push_back({ tmppt.first, tmppt.second, 0.0f });
		if (pt.back().x <= -1 || pt.back().y >= 1)
			int a = 0;
		c.emplace_back(color);
	}
}

void object::remove()
{
	pt.clear();
	c.clear();
	pts.clear();
}

void object::move()
{
	gravity *= 1.06f; // 요기 값 잘 조절해서 어떻게 해볼 것
	for (int i = 0; i < pts.size(); ++i)
	{
		POINT tmp = pts[i];
		tmp.x += 800, tmp.y += 800;
		tmp.x += dir.first * 10;
		tmp.y += dir.second * 10;
		tmp.y += gravity;
		tmp.x -= 800, tmp.y -= 800;
		pts[i] = tmp;
	}
}

pair<bool, pair<vector<POINT>, vector<POINT>>> object::isCross(POINT pt1, POINT pt2)
{
	bool breturn = false;
	vector<POINT> v;
	vector<POINT> left;
	vector<POINT> right;

	for (int i = 0; i < pts.size(); ++i)
	{
		POINT pt3 = pts[i];
		POINT pt4;
		if (i + 1 == pts.size())
			pt4 = pts[0];
		else
			pt4 = pts[i + 1];

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

		v.push_back({ int(pt3.x + t * (double)(pt4.x - pt3.x)),  int(pt3.y + t * (double)(pt4.y - pt3.y)) });

		breturn = true;

	}
	if (v.size() == 0) breturn = false;
	else if (v.size() == 1) breturn = false;
	else
	{
		for (int i = 0; i < pts.size(); ++i)
		{
			float tmp = (pts[i].x - v[0].x) * (v[1].y - v[0].y) - (pts[i].y - v[0].y) * (v[1].x - v[0].x);
			if (tmp > 0) left.push_back(pts[i]);
			else if (tmp < 0) right.push_back(pts[i]);
		}
		left.push_back(v[1]);
		right.push_back(v[1]);
		left.push_back(v[0]);
		right.push_back(v[0]);

	}


	return { breturn, {left, right} };
}

