#include <iostream>
#include <random>
#include <gl/glew.h> //--- 필요한 헤더파일 include
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
	glutKeyboardFunc(Keyboard);
	glutMainLoop(); // 이벤트 처리 시작 
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 바탕색 검정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.

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
	case 'q': // 프로그램 종료
		exit(0);
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value) // 이번엔 안씀
{
	glutPostRedisplay();
	glutTimerFunc(100, TimerFunction, 1);
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) // 색깔 랜덤 변경
	{
		bool isClick = false;
		for (int i = 0; i < 4; ++i) // 안에 있는 사각형 클릭
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
		if (!isClick) // 밖에 있는 사각형 클릭
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
		for (int i = 0; i < 4; ++i) // 안에 있는 사각형 클릭
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
		if (!isClick) // 밖에 있는 사각형 클릭
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