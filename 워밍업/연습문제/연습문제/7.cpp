#include <iostream>
#include <vector>
#include <windows.h>
#include <random>
using namespace std;

random_device rd;
mt19937 dre(rd());
uniform_int_distribution uid{ 0,3 };

#define maxX 25
#define maxY 25

char map[maxY][maxX];
vector<POINT> route;
pair<POINT, int> pt = { { 0, 0 }, 0 };
bool isDraw = false;


void reset()
{
	isDraw = false;

	pt.first = { 0,0 };
	pt.second = 0;
	int count = 0;
	route.clear();

	route.push_back({ 0, 0 });
	POINT rpt = { 0, 0 };
	vector<POINT> pts;

	// �ʱ�ȭ
	for (int i = 0; i < maxY; ++i)
		for (int j = 0; j < maxX; ++j)
			map[i][j] = '1';

	map[route[0].y][route[0].x] = '0';

	while (true)
	{
		pts.clear();
		if (route.size() > 0 && route.back().x == maxX - 1 && route.back().y == maxY - 1)
			break;
		// �� �� �ִ� ���� ���� ã�� -> �������� �̾�
		// ������ �˹��� ����

		if (route.size() > 3
			&& (route[route.size() - 1].x == route[route.size() - 2].x)
			&& (route[route.size() - 2].x == route[route.size() - 3].x)
			&& (route[route.size() - 3].x == route[route.size() - 4].x))
		{
			if (route[route.size() - 1].y > route[route.size() - 2].y)
				map[route.back().y + 1][route.back().x] = 'X';
			else
				map[route.back().y - 1][route.back().x] = 'X';
		}
		else if (route.size() > 3
			&& (route[route.size() - 1].y == route[route.size() - 2].y)
			&& (route[route.size() - 2].y == route[route.size() - 3].y)
			&& (route[route.size() - 3].y == route[route.size() - 4].y))
		{
			if (route[route.size() - 1].x > route[route.size() - 2].x)
				map[route.back().y][route.back().x + 1] = 'X';
			else
				map[route.back().y][route.back().x - 1] = 'X';
		}

		// �� �� �ִ°��� �ִ����� ã��
		// 4�������� �ϴ� Ž���� ����
		// ����
		if (rpt.y - 1 >= 0 && map[rpt.y - 1][rpt.x] == '1')
			pts.push_back({rpt.x, rpt.y - 1});
		// �Ʒ�
		if (rpt.y + 1 < maxY && map[rpt.y + 1][rpt.x] == '1')
			pts.push_back({ rpt.x, rpt.y + 1 });
		// ��
		if (rpt.x - 1 >= 0 && map[rpt.y][rpt.x - 1] == '1')
			pts.push_back({ rpt.x - 1, rpt.y });
		// ��
		if (rpt.x + 1 < maxX && map[rpt.y][rpt.x + 1] == '1')
			pts.push_back({ rpt.x + 1, rpt.y });


		// Ž�� ����� �������� �ϳ� �̾�
		// ���� �� ���� ������
		if (pts.size() == 0)
		{
			map[rpt.y][rpt.x] = 'X';
			route.pop_back();
			if(route.size() > 0)
				rpt = route.back();
		}
		else
		{
			uniform_int_distribution uidt{ 0, int(pts.size() - 1) };
			route.push_back(pts[uidt(dre)]);
			rpt = route.back();
			map[rpt.y][rpt.x] = '0';
		}
	}


	for (int i = 0; i < maxY; ++i)
		for (int j = 0; j < maxX; ++j)
			map[i][j] = '1';

	for (int i = 0; i < route.size(); ++i)
		map[route[i].y][route[i].x] = '0';
}

void draw()
{
	system("cls");
	for (int i = 0; i < maxY; ++i)
	{
		for (int j = 0; j < maxX; ++j)
		{
			if (i == pt.first.y && j == pt.first.x && isDraw)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
				cout << "*";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
			else
				cout << map[i][j];
		}
		cout << endl;
	}
}

int main()
{

	reset();
	char c;
	while (true)
	{
		draw();
		cin >> c;

		switch (c)
		{
		case 'n': // ����
			reset();
			break;

		case 'r': // ��� ����ĭ�� pt �׸���
			isDraw = true;
			break;

		case '+':
			if (pt.second == route.size() - 1) break;
			pt.second++;
			pt.first = route[pt.second];
			break;

		case '-':
			if (pt.second == 0) break;
			pt.second--;
			pt.first = route[pt.second];
			break;

		case 'a':
			for (int i = 0; i < route.size(); ++i)
			{
				pt.first = route[i];
				draw();
				Sleep(10);
			}
			break;

		case 'q':
			exit(0);
			break;

		default:
			cout << "�߸��� �Է��Դϴ�." << endl;
			break;
		}
	}
}