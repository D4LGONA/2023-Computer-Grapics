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
uniform_int_distribution uidXY{ 0, 800 };

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
};

vector<Rect> v;
bool left_button = false;
pair<int, int> mousept;

pair<float, float> func(int x, int y)
{
	pair<float, float> a = { x / 400. - 1, 1 - (y / 400.) };
	return a;
}

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�
	// ��� ������ �־...
	v.push_back({ 400-25,400-25,425,425 });

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
	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // ������ ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.
	pair<float, float> pt1, pt2;
	for (Rect& i: v)
	{
		pt1 = func(i.rc.left, i.rc.top);
		pt2 = func(i.rc.right, i.rc.bottom);
		glColor3f(i.c.R, i.c.G, i.c.B);
		glRectf(pt1.first, pt1.second, pt2.first, pt2.second);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1.0f, 1.0f, 1.0f);
	glRectf(pt1.first, pt1.second, pt2.first, pt2.second);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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
	case 'a': // �߰�
	{
		POINT pt = { uidXY(dre), uidXY(dre) };
		v.push_back({ pt.x - 25, pt.y - 25, pt.x + 25, pt.y + 25 });
		break;
	}
	case 'd': // ����
		if(v.size() > 0)
			v.erase(v.end() - 1);
		break;

	case 'q': // ���α׷� ����
		exit(0);
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value) // �̹��� �Ⱦ�
{
	glutPostRedisplay();
	glutTimerFunc(100, TimerFunction, 1);
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		bool isclick = false;
		left_button = true;
		mousept = {x, y};
		int idx;
		for (int i = 0; i < v.size(); ++i)
		{
			if (PtInRect(&v[i].rc, { x, y }))
			{
				idx = i;
				isclick = true;
				break;
			}
		}
		if(isclick)
			for (int i = idx; i < v.size() - 1; ++i)
				swap(v[i], v[i + 1]);
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		left_button = false;
	}
	return GLvoid();
}

GLvoid Motion(int x, int y)
{
	if (left_button && PtInRect(&v.back().rc, { x, y }))
	{ 
		v.back().rc.left += (x - mousept.first);
		v.back().rc.right += (x - mousept.first);
		v.back().rc.top += (y - mousept.second);
		v.back().rc.bottom += (y - mousept.second);
		mousept = { x,y };
		glutPostRedisplay();
	}
}