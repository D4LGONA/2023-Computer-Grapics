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

struct Color
{
	float R = 0.0f;
	float G = 0.0f;
	float B = 0.0f;
	float ALPHA = 1.0f;
};

Color c;
bool b = false;

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
	glutKeyboardFunc(Keyboard);
	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor( c.R, c.G, c.B, c.ALPHA ); // �������� ��blue�� �� ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.
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
	case 'c': // �þ�
		c = { 0.0f, 1.0f, 1.0f, 1.0f };
		break;
	case 'm': // ����Ÿ
		c = { 1.0f, 0.0f, 1.0f, 1.0f };
		break;
	case 'y': // ���
		c = { 1.0f, 1.0f, 0.0f, 1.0f };
		break;
	case 'a': // ����
		c = { uid(dre), uid(dre), uid(dre), 1.0f };
		break;
	case 'w': // �Ͼ��
		c = { 1.0f, 1.0f, 1.0f, 1.0f };
		break;
	case 'k': // ����
		c = { 0.0f, 0.0f, 0.0f, 1.0f };
		break;
	case 't': // Ÿ�̸Ӵ� ���� �ϴ°ž�
		b = true;
		glutTimerFunc(100, TimerFunction, 1);
		break;
	case 's': // Ÿ�̸� ����
		b = false;
		break;
	case 'q': // ���α׷� ����
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