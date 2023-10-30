#include "stdafx.h"
#include "object.h"

GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수
GLuint vertexShader, fragmentShader; //--- 세이더 객체
GLuint shaderProgramID; //--- 셰이더 프로그램

uniform_real_distribution<float> uidp{-20.0f, 20.0f};

void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboarddown(unsigned char key, int x, int y);
GLvoid Keyboardup(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);
char* filetobuf(const char*);

POINT mousept;
vector<object> stage;
vector<object> o;
vector<object> box;

bool moves[4] = { false, false, false, false }; // w(z++)a(x++)s(z--)d(x--)
bool C_rotY = false;
int walk_count = 0;
bool count_up = true;
float walk_angle = 5.0f;
int walk_speed = 1;

bool isjump = false;
bool jump_up = true;

int opencount = 0;

glm::vec3 origin = { 0.0f, 0.0f, 0.0f };

void cameraRot(int n)
{
	cameraAngle[n] += 5.0f;
}

void walk()
{
	if (walk_count == 5 || walk_count == -5)
		count_up = !count_up;

	o[0].rotPoint.y -= 14.0f;
	o[0].rotOrigin.x = walk_angle * walk_count;
	o[1].rotPoint.y -= 14.0f;
	o[1].rotOrigin.x = -1 * walk_angle * walk_count;

	o[4].rotPoint.y -= 8.0f;
	o[4].rotOrigin.x = -1 * walk_angle * walk_count;
	o[5].rotPoint.y -= 8.0f;
	o[5].rotOrigin.x = walk_angle * walk_count;

	if (count_up)
		walk_count++;
	else
		walk_count--;
}

void open()
{
	if(opencount < 20)
	{
		stage[5].transition.x += 1.0f;
		stage[6].transition.x -= 1.0f;
		opencount++;
	}
}

void Reset()
{
	origin = { 0.0f, 0.0f, 0.0f };

	stage.push_back({ "plane.obj", {20.0f, 1.0f, 20.0f}, {0.0f,0.0f,0.0f}, {0.0f,-20.0f,0.0f} });
	stage.push_back({ "plane.obj", {20.0f, 1.0f, 20.0f}, {0.0f,0.0f,0.0f}, {0.0f,20.0f,0.0f} });
	stage.push_back({ "plane.obj", {20.0f, 1.0f, 20.0f}, {0.0f,0.0f,90.0f}, {20.0f,0.0f,0.0f} });
	stage.push_back({ "plane.obj", {20.0f, 1.0f, 20.0f}, {0.0f,0.0f,90.0f}, {-20.0f,0.0f,0.0f} });
	stage.push_back({ "plane.obj", {20.0f, 1.0f, 20.0f}, {90.0f,0.0f,0.0f}, {0.0f,0.0f,-20.0f} });
	stage.push_back({ "plane.obj", {10.0f, 1.0f, 20.0f}, {90.0f,0.0f,0.0f}, {10.0f,0.0f,20.0f} });
	stage.push_back({ "plane.obj", {10.0f, 1.0f, 20.0f}, {90.0f,0.0f,0.0f}, {-10.0f,0.0f,20.0f} });

	// 다리
	o.push_back({ "cube.obj", {1.0f, 3.0f, 1.0f}, {0.0f,0.0f,0.0f}, {-1.0f, -17.0f, 0.0f} });
	o.push_back({ "cube.obj", {1.0f, 3.0f, 1.0f}, {0.0f,0.0f,0.0f}, {1.0f, -17.0f, 0.0f} }); 

	// 몸통
	o.push_back({ "cube.obj", {2.0f, 3.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, -11.0f, 0.0f } });

	// 머리
	o.push_back({ "cube.obj", {1.0f, 1.0f, 1.0f}, {0.0f,0.0f,0.0f}, {0.0f, -7.0f, 0.0f} });

	// 팔
	o.push_back({ "cube.obj", {1.0f, 3.0f, 1.0f}, {0.0f,0.0f,0.0f}, {-3.0f, -11.0f, 0.0f} });
	o.push_back({ "cube.obj", {1.0f, 3.0f, 1.0f}, {0.0f,0.0f,0.0f}, {3.0f, -11.0f, 0.0f} });

	// nose
	o.push_back({ "cube.obj", {0.1f,0.1f,1.0f}, {0.0f,0.0f,0.0f}, {0.0f, -7.0f, 1.0f} });

	for(int i = 0; i < 3; ++i)
		box.push_back({ "cube.obj", {2.0f, 2.0f, 2.0f}, {0.0f, 0.0f, 0.0f}, {uidp(dre), -18.0f, uidp(dre)} });

	proj = glm::mat4(1.0f);
	proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f); //--- 투영 공간 설정: fovy, aspect, near, far
	proj = glm::translate(proj, glm::vec3(0.0, 0.0, -10.0f));
}

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Example1");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	make_shaderProgram();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(Keyboarddown);
	glutKeyboardUpFunc(Keyboardup);
	Reset();
	glutTimerFunc(100, TimerFunction, 1);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	
	glEnable(GL_DEPTH_TEST); 

	for (object& i : stage)
		i.render(shaderProgramID);

	for (object& i : box)
		i.render(shaderProgramID);

	for (object& i : o)
		i.render(shaderProgramID);

	glutSwapBuffers(); //--- 화면에 출력하기
}
//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboarddown(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'j':
		if(!isjump)
			isjump = true;
		break;

	case'o':
		open();
		break;

	case 'w':
		moves[0] = true;
		break;

	case 'a':
		moves[1] = true;
		break;

	case 's':
		moves[2] = true;
		break;

	case 'd':
		moves[3] = true;
		break;

	case 'x':
		cameraPos.x += 0.5f;
		cameraDirection.x += 0.5f;
		break;

	case 'X':
		cameraPos.x -= 0.5f;
		cameraDirection.x -= 0.5f;
		break;

	case 'y':
		C_rotY = !C_rotY;
		break;

	case 'z':
		cameraPos.z += 0.5f;
		cameraDirection.z += 0.5f;
		break;

	case 'Z':
		cameraPos.z -= 0.5f;
		cameraDirection.z -= 0.5f;
		break;

	case '+':
		if (walk_speed < 10)
		{
			walk_angle += 1.0f;
			walk_speed++;
		}
		break;

	case '-':
		if (walk_speed > 1)
		{
			walk_angle -= 1.0f;
			walk_speed--;
		}
		break;

	case 'p': // 직각투영?
		proj = glm::mat4(1.0f);
		proj = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, -50.0f, 50.0f);
		break;

	case 'P': // 원근투영?
		proj = glm::mat4(1.0f);
		proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f); //--- 투영 공간 설정: fovy, aspect, near, far
		proj = glm::translate(proj, glm::vec3(0.0, 0.0, -10.0f));
		break;

	case 'q': // 프로그램 종료
		exit(0);
		break;
	}
	glutPostRedisplay();
}

GLvoid Keyboardup(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		moves[0] = false;
		break;

	case 'a':
		moves[1] = false;
		break;

	case's':
		moves[2] = false;
		break;
		
	case 'd':
		moves[3] = false;
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	if (opencount != 0 && opencount != 20)
		open();

	for (object& i : o)
	{
		i.Origin = origin;
		i.rotPoint = origin;
	}

	if (moves[0])
	{
		for (object& i : o)
			i.rotOrigin.y = 180.0f;
		origin.z -= 0.5f * walk_speed;

		if (origin.z <= -19.0f)
		{
			for (object& i : o)
				i.rotOrigin.y = 0.0f;
			origin.z -= 0.5f * walk_speed;
		}
	}
	if (moves[1])
	{
		for (object& i : o)
			i.rotOrigin.y = 270.0f;
		origin.x -= 0.5f * walk_speed;

		if (origin.x <= -19.0f)
		{
			for (object& i : o)
				i.rotOrigin.y = 90.0f;
			origin.x += 0.5f * walk_speed;
		}
	}
	if (moves[2])
	{
		for (object& i : o)
			i.rotOrigin.y = 0.0f;
		origin.z += 0.5f * walk_speed;
	}
	if (moves[3])
	{
		for (object& i : o)
			i.rotOrigin.y = 90.0f;
		origin.x += 0.5f * walk_speed;
		if (origin.x >= 19.0f)
		{
			for (object& i : o)
				i.rotOrigin.y = 270.0f;
			origin.x -= 0.5f * walk_speed;
		}
	}
	
	if (moves[0] || moves[1] || moves[2] || moves[3])
		walk();
	else
	{
		walk_count = 0;
		o[0].rotOrigin.x = 0.0f;
		o[1].rotOrigin.x = 0.0f;

		o[4].rotOrigin.x = 0.0f;
		o[5].rotOrigin.x = 0.0f;
	}

	for (object& i : o)
		i.update();

	for (object& i : stage)
		i.update();

	if (C_rotY)
		cameraRot(1);

	glutPostRedisplay();
	glutTimerFunc(100, TimerFunction, 1);
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
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

void make_shaderProgram()
{
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
	glUseProgram(shaderProgramID);
}

void make_vertexShaders()
{
	vertexSource = filetobuf("vertex.glsl");
	//--- 버텍스 세이더 객체 만들기
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexShader);
	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cout << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	fragmentSource = filetobuf("fragment.glsl");
	//--- 프래그먼트 세이더 객체 만들기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentShader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cout << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading 
	if (!fptr) // Return NULL on failure 
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file 
	length = ftell(fptr); // Find out how many bytes into the file we are 
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator 
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file 
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer 
	fclose(fptr); // Close the file 
	buf[length] = 0; // Null terminator 
	return buf; // Return the buffer 
}