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

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{ //--- 윈도우 생성하기
	// 잠깐 할일이 있어서...
	v.push_back({ 400-25,400-25,425,425 });

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
	glutMainLoop(); // 이벤트 처리 시작 
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 바탕색 검정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.
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
	case 'a': // 추가
	{
		POINT pt = { uidXY(dre), uidXY(dre) };
		v.push_back({ pt.x - 25, pt.y - 25, pt.x + 25, pt.y + 25 });
		break;
	}
	case 'd': // 삭제
		if(v.size() > 0)
			v.erase(v.end() - 1);
		break;

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