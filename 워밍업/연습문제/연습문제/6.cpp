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
			cout << "�ٽ� �Է��ϼ���" << endl;
	}


	while (true)
	{
		draw();
		cin >> c;

		switch (c)
		{
		case 'x': // ���� ��ĭ �̵� d
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

		case 'X': // ���� ��ĭ �̵�
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

		case 'y': // �Ʒ�
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

		case 'Y': // ��
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

		case 's': // ��ĭ ���d 
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
		case 'S': // ��ĭ Ȯ�� ���� �ȸ����� �ִµ� �� �н�
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
			
		case 'i': // x����ĭȮ��
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
		case 'j': // x����ĭ���
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
		case 'k': //y����ĭȮ��
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
		case 'l': // y����ĭ���
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

		case 'r': // ����
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
					cout << "�ٽ� �Է��ϼ���" << endl;
			}

			break;
		case 'q': // ����
			exit(0);
			break;
		default:
			cout << "�߸��� �Է��Դϴ�" << endl;
			break;
		}
	}
}