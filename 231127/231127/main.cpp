#include "stdafx.h"
#include "object.h"

uniform_real_distribution _uid{ -50.0f, 50.0f };

GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수
GLuint vertexShader, fragmentShader; //--- 세이더 객체
GLuint shaderProgramID; //--- 셰이더 프로그램

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

Object* stage;
vector<Object*> corns;
vector<Object*> snows;
vector<Object*> spheres;
vector<Object*> cubes;

Object lightbox;

bool _s = false;
bool _r = false;

Object* target = nullptr;

float lightangle = -90.0f;
float _distance = 100.0f;
float RA = 5.0f;
glm::vec3 pts[4] = { {_distance, _distance, 0.0f}, {0.0f, _distance, _distance}, {-_distance, _distance, 0.0f}, {0.0f, _distance, -_distance} };
int cadfadrg = -1;

void Reset()
{

	for (int i = 0; i < 3; ++i)
	{
		spheres.push_back(new Object("sphere.obj", shaderProgramID, { 10.0f + i * 2.0f, 10.0f + i * 2.0f, 10.0f + i * 2.0f }, { 0.0f, 30.0f * i, 0.0f }, { 30.0f + i * 5.0f, 0.0f, 0.0f }, { uidC(dre), uidC(dre), uidC(dre) }));
	}

	spheres[1]->SetRotPoint(1, 30.0f, {0.0f, 0.0f, 0.0f});
	spheres[2]->SetRotPoint(1, 60.0f, {0.0f, 0.0f, 0.0f});

	for (int i = 0; i < 6; ++i)
		corns.push_back(new Object("corn.obj", shaderProgramID, { 0.0f, 1.0f, 1.0f }, i));
	target = corns[0];
	
	for (int i = 0; i < 100; ++i)
		snows.push_back( new Object("sphere.obj", shaderProgramID, {1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, { _uid(dre), 200.0f, _uid(dre)}, {1.0f, 1.0f, 1.0f}) );
	
	lightbox = Object{ "cube.obj", shaderProgramID, {2.0f, 2.0f, 2.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f} };

	stage = new Object{ "plane.obj", shaderProgramID, {50.0f, 1.0f, 50.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 1.0f} };

	cameraPos = glm::vec3(0.0f, 150.0f, 150.0f); //--- 카메라 위치
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
	glutTimerFunc(50, TimerFunction, 1);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	
	glEnable(GL_DEPTH_TEST); 

	for (Object*& o : snows)
		o->Render();
	for (Object*& o : spheres)
		o->Render();

	stage->Render();
	lightbox.Render();

	target->Render();

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
	case '0':
		target = corns[0];
		break;

	case '1':
		target = corns[1];
		break;

	case '2':
		target = corns[2];
		break;

	case '3':
		target = corns[3];
		break;

	case '4':
		target = corns[4];
		break;

	case '5':
		target = corns[5];
		break;

	case 'r': // 조명 공전
		_r = !_r;
		break;

	case 's': // 눈
		for (Object*& o : snows)
		{
			o->SetMove(1, 200.0f);
			o->isMoving = true;
		}
		_s = !_s;
		break;

	case 'm': // 조명 온오프
		light_onf = !light_onf;
		break;

	case 'p': // 조명을 옮긴다?
		cadfadrg = (cadfadrg + 1) % 4;
		lightPos = pts[cadfadrg];
		break;

	case 'n': // 가까워지기
		_distance -= 5.0f;
		break;

	case 'f': // 멀어지기
		_distance += 5.0f;
		break;

	case '+': 
		light_hardness = min(1.0f, light_hardness + 0.5f);
		break;

	case '-': 
		light_hardness = max(0.0f, light_hardness - 0.5f);
		break;

	case 'c': // 조명 색 바꾸기
		lightColor = { uidC(dre), uidC(dre), uidC(dre) };
		break;

	case '9':
		Reset();
		break;
		
	case 'q': // 프로그램 종료
		delete stage;
		for (Object*& o : snows)
			delete o;
		for (Object*& o : corns)
			delete o;
		for (Object*& o : spheres)
			delete o;
		spheres.clear();
		corns.clear();
		snows.clear();
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
	lightbox.SetMove(0, lightPos.x);
	lightbox.SetMove(1, lightPos.y);
	lightbox.SetMove(2, lightPos.z);

	for (Object*& o : spheres)
	{
		/*o->RotByPoint(0, true, {0.0f, 0.0f, 0.0f});
		o->RotByPoint(1, true, {0.0f, 0.0f, 0.0f});*/
		o->RotByPoint(2, true, {0.0f, 0.0f, 0.0f});
	}

	if (_r)
	{
		lightangle += RA;
		lightPos = { _distance * glm::cos(glm::radians(lightangle)), _distance, _distance * glm::sin(glm::radians(lightangle)) };
	}

	if (_s)
	{
		for (Object*& o : snows)
		{
			if (o->isMoving && o->T.y <= 0.0f)
			{
				o->SetMove(1, 0.0f);
				o->isMoving = false;
			}
			else if (!o->isMoving)
			{
				o->count += 1;
				if (o->count == 20)
				{
					o->count = 0;
					o->SetMove(1, 200.0f);
					o->isMoving = true;
				}
			}
			else
				o->SnowMove();
		}
	}

	for (Object*& o : snows)
		o->Update();
	lightbox.Update();
	stage->Update();

	for (Object*& o : corns)
		o->Update();

	for (Object*& o : spheres)
		o->Update();

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