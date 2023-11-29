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
vector<vector<Object*>> v;

bool spinLight = false;
int w_count = 0;
int h_count = 0;

bool _1 = false;
bool _2 = false;
bool _3 = false;
int timertime = 50;

bool drag = false;

// 전체 사이즈는 무조건 50*50으로 고정합니다. 그래서 x와 z scale이 달라질 수 있습니다.
void Reset()
{
	if (v.size() != 0)
	{
		for (auto& i : v)
		{
			for (auto& j : i)
				delete j;
			i.clear();
		}
		v.clear();
	}

	cout << "가로, 세로 개수 값을 입력하세요: ";
	cin >> w_count >> h_count;

	float s_X = 50.0f / float(w_count);
	float s_Z = 50.0f / float(h_count);

	for (int i = 0; i < h_count; ++i)
	{
		v.push_back(vector<Object*>());
		for (int j = 0; j < w_count; ++j)
		{
			v[i].push_back(new Object("cube.obj", shaderProgramID, { s_X, 1.0f, s_Z }, { 0.0f, 0.0f, 0.0f }, {-25.0f + (j * s_X) + s_X / 2.0f, 0.0f, 25.0f - (i * s_Z) - s_Z / 2.0f}, {1.0f, 0.0f, 1.0f}));
		}
	}


	cameraPos = glm::vec3(0.0f, 100.0f, 100.0f); //--- 카메라 위치
	cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
	view = glm::mat4(1.0f);
	cameraAngle = { 0.0f, 0.0f, 0.0f };

	proj = glm::mat4(1.0f);
	proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 200.0f); //--- 투영 공간 설정: fovy, aspect, near, far
	proj = glm::translate(proj, glm::vec3(0.0, 0.0, 30.0f));

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	view = glm::rotate(view, glm::radians(cameraAngle.z), { 0.0f, 0.0f, 1.0f });
	view = glm::rotate(view, glm::radians(cameraAngle.y), { 0.0f, 1.0f, 0.0f });
	view = glm::rotate(view, glm::radians(cameraAngle.x), { 1.0f, 0.0f, 0.0f });
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
	glutTimerFunc(timertime, TimerFunction, 1);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	
	glEnable(GL_DEPTH_TEST); 


	for (auto& i : v)
	{
		for (auto& j : i)
			j->Render();
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
	case '1':
		_1 = !_1;
		_2 = false;
		_3 = false;
		break;

	case '2':
		break;

	case '3':
		break;

	case 't': // 조명 온오프
		light_onf = !light_onf;
		break;

	case 'y': // 카메라 양의방향 공전
		break;

	case 'Y': // 카메라 음의방향 공전
		break;

	case '+': // 속도증가
		timertime = max(timertime - 1, 0);
		break;

	case '-': // 속도감소
		timertime = min(timertime + 1, 200);
		break;

	case 'c': // 조명 색 바꾸기
		lightColor = { uidC(dre), uidC(dre), uidC(dre) };
		break;

	case 'r':
		Reset();
		break;
		
	case 'p': // 직각투영?
		proj = glm::mat4(1.0f);
		proj = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, -50.0f, 50.0f);
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
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	if (_1)
	{
		for (auto& i : v)
			for (auto& j : i)
				j->ani1();
	}

	for (auto& i : v)
		for (auto& j : i)
			j->Update();

	glutPostRedisplay();
	glutTimerFunc(timertime, TimerFunction, 1);
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		drag = true;

		glm::vec3 ray_origin = glm::unProject(glm::vec3(x, HEIGHT - y, 0.0f), view, proj, glm::vec4(0, 0, WIDTH, HEIGHT));
		glm::vec3 ray_direction = glm::normalize(glm::unProject(glm::vec3(x, HEIGHT - y, 1.0f), view, proj, glm::vec4(0, 0, WIDTH, HEIGHT)) - ray_origin);

		for (auto& i : v)
		{
			for (auto& j : i)
			{
				if (obb_ray(*j, ray_origin, ray_direction))
				{
					j->isSelected = true;
					break;
				}
			}
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		drag = false;
		for (auto& i : v)
		{
			for (auto& j : i)
			{
				j->isSelected = false;
			}
		}
	}
	mousept = { x, y };
	return GLvoid();
}

GLvoid Motion(int x, int y)
{
	if (drag)
	{
		//glm::vec3 ray_origin = glm::unProject(glm::vec3(x, HEIGHT - y, 0.0f), view, proj, glm::vec4(0, 0, WIDTH, HEIGHT));
		//glm::vec3 ray_direction = glm::normalize(glm::unProject(glm::vec3(x, HEIGHT - y, 1.0f), view, proj, glm::vec4(0, 0, WIDTH, HEIGHT)) - ray_origin);

		for (auto& i : v)
		{
			for (auto& j : i)
			{
				if (j->isSelected)
				{
					//pair<float, float> glpt = { WintoOpenGL({ x, y }).first - WintoOpenGL(mousept).first, WintoOpenGL({ x, y }).second - WintoOpenGL(mousept).second };
					j->Move(1, x - mousept.x);
				}
			}	
		}
		mousept = { x, y };
	}

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