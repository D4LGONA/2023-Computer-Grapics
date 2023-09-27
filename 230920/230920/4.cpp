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
	int tmppt;
	bool isturn = false;
	bool iswidth = true;
};

vector<Rect> v;
vector<POINT> pt;
pair<int, int> mousept;

bool onA = false;
bool onI = false;
bool onC = false;
bool onO = false;
bool isWidth = true;
POINT Size = { 25, 25 };
int cnt = 0;

pair<float, float> func(int x, int y)
{
	pair<float, float> a = { x / 400. - 1, 1 - (y / 400.) };
	return a;
}

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{ //--- 윈도우 생성하기
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
	case 'a': // 대각선 이동 ㅇ
		onA = !onA;
		onI = false;
		for (Rect& i : v)
		{ // 방향 다시 바꿔주고
			while (i.dir.x == 0)
				i.dir.x = uid(dre);
			while (i.dir.y == 0)
				i.dir.y = uid(dre);
		}
		break;
	
	case 'i': // 지그재그 ㅋㅋ
		onI = !onI;
		onA = false;
		for (Rect& i : v)
			i.dir.y = 0;
		break;

	case 'c': // 크기
		onC = !onC;
		break;

	case 'o': // 색상
		onO = !onO;
		cnt = 0;
		break;

	case 's': // 애니메이션 멈추기
		onA = false;
		onI = false;
		onC = false;
		onO = false;
		break;

	case 'm': // 처음 위치로 리셋
		onA = false;
		onI = false;
		onC = false;
		onO = false;
		for(int i = 0; i < v.size(); ++i)
			v[i].rc = {pt[i].x - Size.x, pt[i].y - Size.y, pt[i].x + Size.x, pt[i].y + Size.y};
		break;

	case 'r': // 지우기 인가
		onA = false;
		onI = false;
		onC = false;
		onO = false;
		v.clear();
		pt.clear();
		break;

	case 'q': // 프로그램 종료
		v.clear();
		pt.clear();
		exit(0);
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	if (onA)
	{
		for (Rect& i : v)
		{
			i.rc.left += i.dir.x * 10;
			i.rc.right += i.dir.x * 10;

			i.rc.top += i.dir.y * 10;
			i.rc.bottom += i.dir.y * 10;

			if ((i.rc.top + i.rc.bottom) / 2 <= 0 || (i.rc.top + i.rc.bottom) / 2 >= 800)
				i.dir.y *= -1;
			if ((i.rc.left + i.rc.right) / 2 <= 0 || (i.rc.left + i.rc.right) / 2 >= 800)
				i.dir.x *= -1;
		}
	}
	if (onI)
	{
		for (Rect& i : v)
		{
			i.rc.left += i.dir.x * 10;
			i.rc.right += i.dir.x * 10;

			i.rc.top += i.dir.y * 10;
			i.rc.bottom += i.dir.y * 10;

			if (((i.rc.left + i.rc.right) / 2 <= 0 || (i.rc.left + i.rc.right) / 2 >= 800) && i.dir.x != 0 && i.dir.y == 0)
			{
				i.tmppt = (i.rc.top + i.rc.bottom) / 2 + 25;
				if (i.tmppt < 0) i.tmppt = 0;
				if (i.tmppt > 800) i.tmppt = 800;
				i.dir.x = 0;
				i.dir.y = 1;
			}

			if (i.rc.top >= i.tmppt)
			{
				i.tmppt = INT_MAX;
				if ((i.rc.left + i.rc.right) / 2 <= 0)
					i.dir.x = -1;
				if ((i.rc.left + i.rc.right) / 2 >= 800)
					i.dir.x = 1;
				i.dir.y = 0;
			}

			if ((i.rc.left + i.rc.right) / 2 <= 0 || (i.rc.left + i.rc.right) / 2 >= 800)
				i.dir.x *= -1;
			
			if ((i.rc.top + i.rc.bottom) / 2 > 800)
			{
				i.rc.top -= 800;
				i.rc.bottom -= 800;
				i.tmppt = 25;
			}

		}
	}
	if (onC)
	{
		for (Rect& i : v)
		{
			if (isWidth)
			{
				if (i.rc.right - i.rc.left < 100)
				{
					i.rc.right+= 5;
					i.rc.left-= 5;
					i.rc.top+= 5;
					i.rc.bottom-= 5;
				}
				else
					isWidth = false;
			}
			else
			{
				if (i.rc.bottom - i.rc.top < 100)
				{
					i.rc.right-= 5;
					i.rc.left+= 5;
					i.rc.top-= 5;
					i.rc.bottom+= 5;
				}
				else
					isWidth = true;
			}
		}
	}
	if (onO)
	{
		cnt++;
		if (cnt >= 10)
		{
			for (Rect& i : v)
			{
				i.c = { uidC(dre), uidC(dre), uidC(dre) };
			}
			cnt = 0;
		}
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