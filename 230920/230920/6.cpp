#include <iostream>
#include <random>
#include <vector>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
using namespace std;

random_device rd;
mt19937 dre(rd());
uniform_real_distribution<float> uidC{ 0.0f, 1.0f }; // ���� �÷� ����
uniform_int_distribution uidPT{ 30, 770 };
uniform_int_distribution uidS{ 1,3 };

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);

struct Color
{
	float R = uidC(dre);
	float G = uidC(dre);
	float B = uidC(dre);
	float ALPHA = 1.0f;
};

struct Rect
{
	RECT rc;
	int size = 30;
	POINT dir = { 0, 0 };
	Color c;
	int state = 0;
	bool isSlice = false;
};

pair<int, int> mousept;
vector<vector<Rect>> v;

pair<float, float> func(int x, int y)
{
	pair<float, float> a = { x / 400. - 1, 1 - (y / 400.) };
	return a;
}

bool IntersectRect(RECT a, RECT b)
{
	if (a.right >= b.left && a.left <= b.right
		&& a.top <= b.bottom && a.bottom >= b.top)
		return true;
	return false;
}

void move(Rect& r)
{
	r.rc.left += r.dir.x * 5;
	r.rc.right += r.dir.x * 5;
	r.rc.top += r.dir.y * 5;
	r.rc.bottom += r.dir.y * 5;
}

void to_small(Rect& r)
{
	if(r.size != 0)
		r.size -= 1;
	r.rc.left = (r.rc.left + r.rc.right) / 2 - r.size;
	r.rc.right = (r.rc.left + r.rc.right) / 2 + r.size;
	r.rc.top = (r.rc.top + r.rc.bottom) / 2 - r.size;
	r.rc.bottom = (r.rc.top + r.rc.bottom) / 2 + r.size;
}

void RESET()
{
	for (int i = 0; i < v.size(); ++i)
		v[i].clear();
	v.clear();

	for (int i = 0; i < 5; ++i)
	{
		int x = uidPT(dre);
		int y = uidPT(dre);
		int s = uidS(dre);
		switch (s)
		{
		case 1: // �¿���� -> 4��
			v.push_back({ { {x - 30, y - 30, x + 30, y + 30} }, {{x - 30, y - 30, x, y}, 15, {-1,0}}, {{x, y - 30, x + 30, y}, 15, {0,-1}}, {{x, y, x + 30,y + 30}, 15, {1,0}}, {{x - 30, y, x, y + 30}, 15, {0,1}} });
			break;
		case 2: // �밢�� -> 4��
			v.push_back({ { {x - 30, y - 30, x + 30, y + 30} }, {{x - 30, y - 30, x, y}, 15, {-1,-1}}, {{x, y - 30, x + 30, y}, 15, {1,-1}}, {{x, y, x + 30,y + 30}, 15, {1,1}}, {{x - 30, y, x, y + 30}, 15, {-1,1}} });
			break;
		case 3: // �¿���ϴ밢�� -> 9��
			v.push_back({ { {x - 30, y - 30, x + 30, y + 30} }, {{x - 30, y - 30, x - 10, y - 10}, 10, {-1,0}}, {{x + 10, y - 30, x + 30, y - 10}, 10, {0,-1}}, {{x + 10, y + 10, x + 30,y + 30}, 10, {1,0}}, {{x - 30, y + 10, x - 10, y + 30}, 10, {0,1}},
				{{x - 10, y - 30, x + 10, y - 10}, 10, {-1,-1}}, {{x + 10, y - 10, x + 30, y + 10}, 10, {1,-1}}, {{x - 10, y + 10, x + 10, y + 30}, 10, {1,1}}, {{x - 30, y - 10, x - 10, y + 10}, 10, {-1,1}}, {{x - 10, y - 10, x + 10, y + 10}, 10 }});
			break;
		}
		v[i][0].state = s;
	}
}

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�
	RESET();

	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(800, 800); // �������� ũ�� ����
	glutCreateWindow("Example1"); // ������ ����(������ �̸�)

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";


	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(Keyboard);

	glutTimerFunc(100, TimerFunction, 1);

	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // ������ ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.

	pair<float, float> pt1, pt2;
	for (int i = 0; i < v.size(); ++i) // �簢�� �׸��� ��
	{
		if (v[i][0].isSlice)
		{
			for (int j = 1; j < v[i].size(); ++j)
			{
				pt1 = func(v[i][j].rc.left, v[i][j].rc.top);
				pt2 = func(v[i][j].rc.right, v[i][j].rc.bottom);
				glColor3f(v[i][0].c.R, v[i][0].c.G, v[i][0].c.B);
				glRectf(pt1.first, pt1.second, pt2.first, pt2.second);
			}
		}
		else
		{
			pt1 = func(v[i][0].rc.left, v[i][0].rc.top);
			pt2 = func(v[i][0].rc.right, v[i][0].rc.bottom);
			glColor3f(v[i][0].c.R, v[i][0].c.G, v[i][0].c.B);
			glRectf(pt1.first, pt1.second, pt2.first, pt2.second);
		}
	}

	// // // // // // //

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
		RESET();
		break;

	case 'q': // ���α׷� ����
		v.clear();
		exit(0);
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	for (int i = 0; i < v.size(); ++i)
	{
		if (v[i].front().isSlice)
		{
			switch (v[i].front().state)
			{
			case 1:
				for (int j = 1; j < v[i].size(); ++j)
				{
					move(v[i][j]);
					to_small(v[i][j]);
				}
				break;
			case 2:
				for (int j = 1; j < v[i].size(); ++j)
				{
					move(v[i][j]);
					to_small(v[i][j]);
				}
				break;
			case 3:
				for (int j = 1; j < v[i].size(); ++j)
				{
					move(v[i][j]);
					to_small(v[i][j]);
				}
				break;
			}
		}
	}

	for (int i = 0; i < v.size(); ++i)
	{
		if (v[i][1].size == 0)
			v.erase(v.begin() + i);
	}
	glutPostRedisplay();
	glutTimerFunc(100, TimerFunction, 1);
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		for (int i = 0; i < v.size(); ++i)
		{
			if (PtInRect(&v[i][0].rc, { x,y }))
			{
				v[i][0].isSlice = true;
				break;
			}
		}
	}

	return GLvoid();
}

GLvoid Motion(int x, int y)
{
	glutPostRedisplay();
}