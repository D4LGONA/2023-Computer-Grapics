#include <iostream>
#include <random>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
using namespace std;

random_device rd;
mt19937 dre(rd());
uniform_real_distribution<float> uid{ 0.0f, 1.0f };

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);
GLvoid Mouse(int button, int state, int x, int y);

struct Color
{
	float R = uid(dre);
	float G = uid(dre);
	float B = uid(dre);
	float ALPHA = 1.0f;
};

Color bg[4];
Color front[4];
RECT bgRects[4] = { {0, 0, 400, 400}, {0, 400, 400, 800}, {400, 0, 800, 400}, {400, 400, 800, 800} };
RECT frRects[4] = { {0, 0, 400, 400}, {0, 400, 400, 800}, {400, 0, 800, 400}, {400, 400, 800, 800} };
int scales[4] = { 10, 10, 10, 10 };

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
	glutKeyboardFunc(Keyboard);
	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // ������ ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.

	for (int i = 0; i < 4; ++i)
	{
		glColor3f(bg[i].R, bg[i].G, bg[i].B);
		pair<float, float> pt1 = func(bgRects[i].left, bgRects[i].top);
		pair<float, float> pt2 = func(bgRects[i].right, bgRects[i].bottom);
		glRectf(pt1.first, pt1.second, pt2.first, pt2.second);

		glColor3f(front[i].R, front[i].G, front[i].B);
		pt1 = func(frRects[i].left, frRects[i].top);
		pt2 = func(frRects[i].right, frRects[i].bottom);
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
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) // ���� ���� ����
	{
		bool isClick = false;
		for (int i = 0; i < 4; ++i) // �ȿ� �ִ� �簢�� Ŭ��
		{
			if (PtInRect(&frRects[i], { x, y }))
			{
				front[i].R = uid(dre);
				front[i].G = uid(dre);
				front[i].B = uid(dre);
				isClick = true;
				break;
			}
		}
		if (!isClick) // �ۿ� �ִ� �簢�� Ŭ��
		{
			for (int i = 0; i < 4; ++i)
			{
				if (PtInRect(&bgRects[i], { x, y }))
				{
					bg[i].R = uid(dre);
					bg[i].G = uid(dre);
					bg[i].B = uid(dre);
					break;
				}
			}
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		bool isClick = false;
		for (int i = 0; i < 4; ++i) // �ȿ� �ִ� �簢�� Ŭ��
		{
			if (PtInRect(&frRects[i], { x, y }))
			{
				if (scales[i] > 1)
				{
					scales[i]--;
					frRects[i].left += 20;
					frRects[i].top += 20;
					frRects[i].right -= 20;
					frRects[i].bottom -= 20;
				}
				isClick = true;
				break;
			}
		}
		if (!isClick) // �ۿ� �ִ� �簢�� Ŭ��
		{
			for (int i = 0; i < 4; ++i)
			{
				if (PtInRect(&bgRects[i], { x, y }))
				{
					if (scales[i] < 10)
					{
						scales[i]++;
						frRects[i].left -= 20;
						frRects[i].top -= 20;
						frRects[i].right += 20;
						frRects[i].bottom += 20;
					}
					break;
				}
			}
		}
	}
	return GLvoid();
}