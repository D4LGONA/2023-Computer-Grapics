#include <iostream>
#include <random>
#include <vector>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
using namespace std;

random_device rd;
mt19937 dre(rd());
uniform_real_distribution<float> uidC{ 0.0f, 1.0f }; // 랜덤 컬러 생성
uniform_int_distribution uidPT{ 10, 790 };
uniform_int_distribution uidN{ 20, 40 };

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
pair<int, int> mousept;
int erasersize = 20;
bool leftClick = false;
Rect eraser{ {0,40,0,40},{0.0f,0.0f,0.0f,1.0f} };

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

void RESET()
{
	if (!v.empty())
		v.clear();
	for (int i = 0; i < uidN(dre); ++i)
	{
		int x = uidPT(dre);
		int y = uidPT(dre);
		v.push_back({x - 10, y - 10, x + 10, y + 10});
	}
	erasersize = 20;
	eraser.c = { 0.0f,0.0f,0.0f,1.0f };
}

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{ //--- 윈도우 생성하기
	RESET();

	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(800, 800); // 윈도우의 크기 지정
	glutCreateWindow("Example1"); // 윈도우 생성(윈도우 이름)

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";


	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(Keyboard);

	glutTimerFunc(100, TimerFunction, 1);

	glutMainLoop(); // 이벤트 처리 시작 
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 바탕색 검정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기

	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.

	pair<float, float> pt1, pt2;
	for (Rect& i : v) // 사각형 그리는 것
	{
		pt1 = func(i.rc.left, i.rc.top);
		pt2 = func(i.rc.right, i.rc.bottom);
		glColor3f(i.c.R, i.c.G, i.c.B);
		glRectf(pt1.first, pt1.second, pt2.first, pt2.second);
	}

	if (leftClick)
	{
		pt1 = func(eraser.rc.left, eraser.rc.top);
		pt2 = func(eraser.rc.right, eraser.rc.bottom);
		glColor3f(eraser.c.R, eraser.c.G, eraser.c.B);
		glRectf(pt1.first, pt1.second, pt2.first, pt2.second);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(1.0f, 1.0f, 1.0f);
		glRectf(pt1.first, pt1.second, pt2.first, pt2.second);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}


	// // // // // // //

	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
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

	case 'q': // 프로그램 종료
		v.clear();
		exit(0);
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	glutPostRedisplay();
	glutTimerFunc(100, TimerFunction, 1);
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		leftClick = true;
		eraser.rc = { x - erasersize, y - erasersize, x + erasersize, y + erasersize };
		mousept = { x, y };
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		leftClick = false;
		erasersize = 20;
		eraser.rc = { x - 20, y - 20, x + 20, y + 20 };
		eraser.c = { 0.0f, 0.0f, 0.0f, 1.0f };
	}

	return GLvoid();
}

GLvoid Motion(int x, int y)
{
	if (leftClick)
	{
		eraser.rc.left = x - erasersize;
		eraser.rc.right = x + erasersize;
		eraser.rc.top = y - erasersize;
		eraser.rc.bottom = y + erasersize;
		for (int i = 0; i < v.size(); ++i)
		{
			if (IntersectRect(v[i].rc, eraser.rc))
			{
				eraser.c = v[i].c;
				v.erase(v.begin() + i);
				erasersize += 10;
			}
		}
	}
	glutPostRedisplay();
}