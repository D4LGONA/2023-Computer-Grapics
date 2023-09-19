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

struct Color
{
	float R = 0.0f;
	float G = 0.0f;
	float B = 0.0f;
	float ALPHA = 1.0f;
};

Color c;
bool b = false;

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
	glutKeyboardFunc(Keyboard);
	glutMainLoop(); // 이벤트 처리 시작 
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor( c.R, c.G, c.B, c.ALPHA ); // 바탕색을 ‘blue’ 로 지정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.
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
	case 'c': // 시안
		c = { 0.0f, 1.0f, 1.0f, 1.0f };
		break;
	case 'm': // 마젠타
		c = { 1.0f, 0.0f, 1.0f, 1.0f };
		break;
	case 'y': // 노랑
		c = { 1.0f, 1.0f, 0.0f, 1.0f };
		break;
	case 'a': // 랜덤
		c = { uid(dre), uid(dre), uid(dre), 1.0f };
		break;
	case 'w': // 하얀색
		c = { 1.0f, 1.0f, 1.0f, 1.0f };
		break;
	case 'k': // 검정
		c = { 0.0f, 0.0f, 0.0f, 1.0f };
		break;
	case 't': // 타이머는 어케 하는거야
		b = true;
		glutTimerFunc(100, TimerFunction, 1);
		break;
	case 's': // 타이머 종료
		b = false;
		break;
	case 'q': // 프로그램 종료
		exit(0);
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	c = { uid(dre), uid(dre), uid(dre), 1.0f };
	glutPostRedisplay();
	if (b)
		glutTimerFunc(100, TimerFunction, 1);
}