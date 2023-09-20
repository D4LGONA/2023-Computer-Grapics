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
uniform_int_distribution uid{ -1, 1 };

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
	Color c;
	POINT dir = {uid(dre), uid(dre)};
	POINT tmppt;
};

enum State // ��.. ����̱�
{
	a = 1,
	i = 2,
	c = 3,
	o = 4
};

vector<Rect> v;
vector<POINT> pt;
pair<int, int> mousept;
int state = 0;

bool isWidth = true;
POINT Size = { 25, 25 };

pair<float, float> func(int x, int y)
{
	pair<float, float> a = { x / 400. - 1, 1 - (y / 400.) };
	return a;
}

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�
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
	for (Rect& i : v) // �簢�� �׸��� ��
	{
		pt1 = func(i.rc.left, i.rc.top);
		pt2 = func(i.rc.right, i.rc.bottom);
		glColor3f(i.c.R, i.c.G, i.c.B);
		glRectf(pt1.first, pt1.second, pt2.first, pt2.second);
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
	case 'a': // �밢�� �̵� d
		state = a;
		for (Rect& i : v)
		{ // ���� �ٽ� �ٲ��ְ�
			while (i.dir.x == 0)
				i.dir.x = uid(dre);
			while (i.dir.y == 0)
				i.dir.y = uid(dre);
		}
		break;
	
	case 'i': // ������� ����
		state = i;
		for (Rect& i : v)
		{
			i.dir.y = 0;
		}
		break;

	case 'c': // ũ��
		state = c;
		break;

	case 'o': // ����
		for (Rect& i : v)
		{
			i.c = { uidC(dre), uidC(dre), uidC(dre) };
		}
		break;

	case 's': // �ִϸ��̼� ���߱�
		state = 0;
		break;

	case 'm': // ó�� ��ġ�� ����
		state = 0;
		for(int i = 0; i < v.size(); ++i)
			v[i].rc = {pt[i].x - Size.x, pt[i].y - Size.y, pt[i].x + Size.x, pt[i].y + Size.y};
		break;

	case 'r':
		state = 0;
		v.clear();
		pt.clear();
		break;

	case 'q': // ���α׷� ����
		v.clear();
		pt.clear();
		exit(0);
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	switch (state)
	{
	case a:
		for (Rect& i : v)
		{
			i.rc.left += i.dir.x * 10;
			i.rc.right += i.dir.x * 10;

			i.rc.top += i.dir.y * 10;
			i.rc.bottom += i.dir.y * 10;

			if (i.rc.top <= 0 || i.rc.bottom >= 800)
				i.dir.y *= -1;
			if (i.rc.left <= 0 || i.rc.right >= 800)
				i.dir.x *= -1;
		}
		break;
	case i:
		for (Rect& i : v)
		{
			i.rc.left += i.dir.x * 10;
			i.rc.right += i.dir.x * 10;

			i.rc.top += i.dir.y * 10;
			i.rc.bottom += i.dir.y * 10;
			
			if (i.rc.left <= 0 || i.rc.right >= 800)
			{
				i.dir.x *= -1;
			}
		}
		break;
	case c:
		if (isWidth)
		{

		}
		break;
	}
	glutPostRedisplay();
	glutTimerFunc(100, TimerFunction, 1);
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		pt.push_back({ x, y });
		v.push_back({ x - Size.x, y - Size.y, x + Size.x, y + Size.y });

		while (v.back().dir.x == 0)
			v.back().dir.x = uid(dre);
		while (v.back().dir.y == 0)
			v.back().dir.y = uid(dre);
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{

	}

	return GLvoid();
}

GLvoid Motion(int x, int y)
{
	glutPostRedisplay();
}