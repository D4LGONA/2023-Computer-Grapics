#include "stdafx.h"
#include "object.h"
#include "Robot.h"
#include "BB.h"

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
vector<Object> stage;
vector<Robot> r;
vector<Object> box; // 밟으면 들어가는 애
vector<Object> cyl; 

struct obs
{

};


void cameraRot(int n)
{
	cameraAngle[n] += 1.0f;
}

void Reset()
{
	stage.clear();
	r.clear();

	cameraPos = glm::vec3(0.0f, 0.0f, 120.0f); //--- 카메라 위치
	cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
	view = glm::mat4(1.0f);
	cameraAngle = { 0.0f, 0.0f, 0.0f };


	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			stage.push_back({ "plane.obj", {5.0f, 0.1f, 5.0f}, {0.0f,0.0f,0.0f}, {(-5.0f * 5) + (5.0f * i) + 2.5f,-20.0f,(-5.0f * 5) + (5.0f * j) + 2.5f}});
		}
	}

	r.push_back({ glm::vec3{ 0.0f, 0.0f ,0.0f }, 1.0f });
	r.push_back({ glm::vec3{ 0.0f, 0.0f, 5.0f }, 0.5f });


	//for(int i = 0; i < 5; ++i)
	box.push_back({ "cube.obj", {5.0f, 5.0f, 5.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, -20.0f, 0.0f} });

	cyl.push_back({ "cylinder.obj", {5.0f, 100.0f, 5.0f}, {0.0f, 0.0f, 0.0f}, {-25.0f + 2.5f, -20.0f, -25.0f + 2.5f} });

	proj = glm::mat4(1.0f);
	proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 200.0f); //--- 투영 공간 설정: fovy, aspect, near, far
	proj = glm::translate(proj, glm::vec3(0.0, 0.0, 30.0f));
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
	glutTimerFunc(50, TimerFunction, 1);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	
	glEnable(GL_DEPTH_TEST); 

	for (Object& i : stage)
	{
		i.Render(shaderProgramID);
		i.bb->Render(shaderProgramID);
	}

	for (Robot& i : r)
	{
		i.Render(shaderProgramID);
	}


	for (Object& i : box)
	{
		i.Render(shaderProgramID);
		i.bb->Render(shaderProgramID);
	}


	for (Object& i : cyl)
	{
		i.Render(shaderProgramID);
		i.bb->Render(shaderProgramID);
	}


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
	case 'j': // 점프
		r[0].Jump();
		break;

	case 'k':
		for (int i = 1; i < r.size(); ++i)
			r[i].Jump();
		break;

	case 'w':
		for (Robot& i : r)
		{
			i.RotationY(180.0f);
			i.dir.z = -1.0f;
		}
		break;

	case 'a':
		for (Robot& i : r)
		{
			i.RotationY(270.0f);
			i.dir.x = -1.0f;
		}
		break;

	case 's':
		for (Robot& i : r)
		{
			i.RotationY(0.0f);
			i.dir.z = 1.0f;
		}
		break;

	case 'd':
		for (Robot& i : r)
		{
			i.RotationY(90.0f);
			i.dir.x = 1.0f;
		}
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
		break;

	case '-':
		break;

	case 'p': // 직각투영?
		proj = glm::mat4(1.0f);
		proj = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, -50.0f, 50.0f);
		break;

	case 'P': // 원근투영?
		proj = glm::mat4(1.0f);
		proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 200.0f); //--- 투영 공간 설정: fovy, aspect, near, far
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
		for (Robot& i : r)
			i.dir.z = 0.0f;
		break;

	case 'a':
		for (Robot& i : r)
			i.dir.x = 0.0f;
		break;

	case's':
		for (Robot& i : r)
			i.dir.z = 0.0f;
		break;
		
	case 'd':
		for (Robot& i : r)
			i.dir.x = 0.0f;
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	for (Robot& i : r)
		i.Move();

	for (Robot& i : r)
		i.Update();

	for (Object& i : stage)
		i.Update();

	for (Object& j : box)
		j.Update();

	for (Object& i : cyl)
		i.Update();


	// 충돌 체크 - 플레이어와 땅
	for (Object& j : stage)
	{
		for (Robot& i : r)
		{
			for (int k = 0; k < 7; ++k)
			{
				if (obb(*i.GetBB(k), *j.bb))
				{
					while (obb(*i.GetBB(k), *j.bb))
					{
						i.origin.y += 0.1f;
						i.dir.y = 0.0f;
						i.Update();
					}
				}
			}
		}
	}
	
	bool b = false;
	// 충돌 체크 - 플레이어와 박스
	for (Object& j : box)
	{
		for (Robot& i : r)
		{
			for (int k = 0; k < 7; ++k)
			{
				if (obb(*i.GetBB(k), *j.bb) && i.origin.y > 0.0f)
				{
					b = true;
					while (obb(*i.GetBB(k), *j.bb))
					{
						i.origin.y += 0.1f;
						i.dir.y = 0.0f;
						i.Update();
					}
				}
			}
			
		}
		if (b)
			j.Move(1, -0.1f);
		else if (!b && j.bb->vCenterPos.y < -17.5f)
			j.Move(1, 0.1f);
	}

	//충돌체크 - 플레이어와 기둥
	for (Object& j : cyl)
	{
		for (Robot& i : r)
		{
			for (int k = 0; k < 7; ++k)
			{
				if (obb(*i.GetBB(k), *j.bb) && i.origin.y > 0.0f)
				{
					while (obb(*i.GetBB(k), *j.bb))
					{
						i.origin.x -= i.dir.x/ 10.0f;
						i.origin.z -= i.dir.z/ 10.0f;
						i.Update();
					}
				}
			}
		}
	}


	glutPostRedisplay();
	glutTimerFunc(50, TimerFunction, 1);
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