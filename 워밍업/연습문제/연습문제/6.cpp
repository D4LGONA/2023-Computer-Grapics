#include <iostream>
#include <Windows.h>
using namespace std;

#define maxX 20
#define maxY 20

bool board[maxY][maxX];

void draw()
{
	system("cls");
	for (int i = 0; i < maxY; ++i)
	{
		for (int j = 0; j < maxX; ++j)
			if (board[i][j])
				cout << "o ";
			else
				cout << "* ";
		cout << endl;
	}
}

void reset()
{
	for (int i = 0; i < maxY; ++i)
		for (int j = 0; j < maxX; ++j)
			board[i][j] = '*';
}

int main()
{
	POINT pt1, pt2;
	char c;

	while (true)
	{
		cin >> pt1.x >> pt1.y >> pt2.x >> pt2.y;
		if (pt1.x <= pt2.x && pt1.y <= pt2.y && pt1.x >= 0 && pt2.x < maxX && pt1.y >= 0 && pt2.y < maxY)
		{
			for (int i = 0; i < maxY; ++i)
			{
				for (int j = 0; j < maxX; ++j)
				{
					if (i >= pt1.y && i <= pt2.y && j >= pt1.x && j <= pt2.x)
						board[i][j] = true;
					else
						board[i][j] = false;
				}
			}
			break;
		}
		else
			cout << "다시 입력하세요" << endl;
	}


	while (true)
	{
		draw();
		cin >> c;

		switch (c)
		{
		case 'x': // 좌측 한칸 이동 d
			pt1.x--;
			if (pt1.x < 0) pt1.x += maxX;
			for (int i = 0; i < maxY; ++i)
			{
				for (int j = 0; j < maxX; ++j)
				{
					if (j == pt1.x && board[i][pt2.x]) 
						board[i][j] = true;
				}
			}

			for (int i = 0; i < maxY; ++i)
			{
				for (int j = 0; j < maxX; ++j)
				{
					if (j == pt2.x) board[i][j] = false;
				}
			}
			pt2.x--;
			if (pt2.x < 0) pt2.x += maxX;
			break;

		case 'X': // 우측 한칸 이동
			pt2.x++;
			if (pt2.x >= maxX) pt2.x -= maxX;
			for (int i = 0; i < maxY; ++i)
			{
				for (int j = 0; j < maxX; ++j)
				{
					if (j == pt2.x && board[i][pt1.x])
						board[i][j] = true;
				}
			}

			for (int i = 0; i < maxY; ++i)
			{
				for (int j = 0; j < maxX; ++j)
				{
					if (j == pt1.x) board[i][j] = false;
				}
			}
			pt1.x++;
			if (pt1.x >= maxX) pt1.x -= maxX;
			break;

		case 'y': // 아래
			pt2.y++;
			if (pt2.y >= maxY) pt2.y -= maxY;
			for (int i = 0; i < maxY; ++i)
			{
				for (int j = 0; j < maxX; ++j)
				{
					if (i == pt2.y && board[pt1.y][j])
						board[i][j] = true;
				}
			}

			for (int i = 0; i < maxY; ++i)
			{
				for (int j = 0; j < maxX; ++j)
				{
					if (i == pt1.y) board[i][j] = false;
				}
			}
			pt1.y++;
			if (pt1.y >= maxY) pt1.y -= maxY;
			break;

		case 'Y': // 위
			pt1.y--;
			if (pt1.y < 0) pt1.y += maxY;
			for (int i = 0; i < maxY; ++i)
			{
				for (int j = 0; j < maxX; ++j)
				{
					if (i == pt1.y && board[pt2.y][j])
						board[i][j] = true;
				}
			}

			for (int i = 0; i < maxY; ++i)
			{
				for (int j = 0; j < maxX; ++j)
				{
					if (i == pt2.y) board[i][j] = false;
				}
			}
			pt2.y--;
			if (pt2.y < 0) 
				pt2.y += maxY;
			break;

		case 's': // 한칸 축소d 
			if (pt1.x == pt2.x || pt1.y == pt2.y) break;
			
			for (int i = 0; i < maxY; ++i)
			{
				for (int j = 0; j < maxX; ++j)
				{
					if (i == pt2.y || j == pt2.x) board[i][j] = false;
				}
			}

			pt2.x--;
			pt2.y--;
			if (pt2.x < 0) pt2.x += maxX;
			if (pt2.y < 0) pt2.y += maxY;

			break;
		case 'S': // 한칸 확대 조건 안막은거 있는데 걍 패스
			pt2.x++;

			if (pt2.x >= maxX) pt2.x -= maxX;
			for (int i = 0; i < maxY; ++i)
			{
				for (int j = 0; j < maxX; ++j)
				{
					if (j == pt2.x && board[i][pt1.x])
						board[i][j] = true;
				}
			}

			pt2.y++;
			if (pt2.y >= maxY) pt2.y -= maxY;
			for (int i = 0; i < maxY; ++i)
			{
				for (int j = 0; j < maxX; ++j)
				{
					if (i == pt2.y && board[pt1.y][j])
						board[i][j] = true;
				}
			}
			break;
			
		case 'i': // x축한칸확대
			if (pt1.x - 1 == pt2.x) break;
			pt2.x++;

			if (pt2.x >= maxX) pt2.x -= maxX;
			for (int i = 0; i < maxY; ++i)
			{
				for (int j = 0; j < maxX; ++j)
				{
					if (j == pt2.x && board[i][pt1.x])
						board[i][j] = true;
				}
			}
			break;
		case 'j': // x축한칸축소
			if (pt1.x == pt2.x) break;

			for (int i = 0; i < maxY; ++i)
			{
				for (int j = 0; j < maxX; ++j)
				{
					if (j == pt2.x) board[i][j] = false;
				}
			}

			pt2.x--;
			if (pt2.x < 0) pt2.x += maxX;
			break;
		case 'k': //y축한칸확대
			if (pt1.y - 1 == pt2.y) break;
			pt2.y++;
			if (pt2.y >= maxY) pt2.y -= maxY;
			for (int i = 0; i < maxY; ++i)
			{
				for (int j = 0; j < maxX; ++j)
				{
					if (i == pt2.y && board[pt1.y][j])
						board[i][j] = true;
				}
			}
			break;
		case 'l': // y축한칸축소
			if (pt1.y == pt2.y) break;

			for (int i = 0; i < maxY; ++i)
			{
				for (int j = 0; j < maxX; ++j)
				{
					if (i == pt2.y) board[i][j] = false;
				}
			}

			pt2.y--;

			if (pt2.y < 0)
				pt2.y += maxY;
			break;

		case 'r': // 리셋
			system("cls");
			reset();
			while (true)
			{
				cin >> pt1.x >> pt1.y >> pt2.x >> pt2.y;
				if (pt1.x <= pt2.x && pt1.y <= pt2.y && pt1.x >= 0 && pt2.x < maxX && pt1.y >= 0 && pt2.y < maxY)
				{
					for (int i = 0; i < maxY; ++i)
					{
						for (int j = 0; j < maxX; ++j)
						{
							if (i >= pt1.y && i <= pt2.y && j >= pt1.x && j <= pt2.x)
								board[i][j] = true;
							else
								board[i][j] = false;
						}
					}
					break;
				}
				else
					cout << "다시 입력하세요" << endl;
			}

			break;
		case 'q': // 종료
			exit(0);
			break;
		default:
			cout << "잘못된 입력입니다" << endl;
			break;
		}
	}
}