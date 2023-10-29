#include "stdafx.h"
#include "object.h"

GLuint vao, vbo[2], ebo;

GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수
GLuint vertexShader, fragmentShader; //--- 세이더 객체
GLuint shaderProgramID; //--- 셰이더 프로그램

void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);
//void InitBuffer();
char* filetobuf(const char*);

POINT mousept;
vector<object> o;
vector<glm::vec3> pts;
vector<glm::vec3> pts2;
int cnt = 0;
pair<bool, bool> b = {false, false};
pair<bool, bool> m = {false, false};
pair<bool, bool> f = {false, false};
pair<bool, bool> e = {false, false};
pair<bool, bool> t = {false, false};
pair<bool, bool> y_ = {false, false};
pair<bool, bool> r = {false, false};
pair<bool, bool> a = {false, false};
bool viewport = true;

glm::vec3 origin = { 0.0f, 0.0f, 0.0f };

object bg{};
glm::vec3 tmppos = cameraPos;


void setRoute(glm::vec3 dest)
{
	pts.clear();
	float r = dist({ cameraPos.x, 0.0f, cameraPos.z }, { dest.x, 0.0f, dest.z });
	for (int i = 0; i < 360; ++i)
		pts.push_back({ r * cos(glm::radians(float(i))) + dest.x, cameraPos.y, r * sin(glm::radians(float(i))) + dest.z });
	cameraDirection = dest;
}

void setView(glm::vec3 dest)
{
	pts2.clear();
	float r = dist({ cameraPos.x, 0.0f, cameraPos.z }, { dest.x, 0.0f, dest.z });
	for (int i = 0; i < 360; ++i)
		pts2.push_back({ r * cos(glm::radians(float(i))) + cameraPos.x, dest.y, r * sin(glm::radians(float(i))) + cameraPos.z });
}

void Reset() // 필요한 객체 -> 
{
	bg = { "cube.obj", glm::vec3{ 10.0f, 0.1f, 10.0f }, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 0.0f} };

	o.push_back({ "cube.obj", glm::vec3{ 1.0f, 0.5f, 1.0f }, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.5f, 0.0f} });

	o.push_back({ "cube.obj", glm::vec3{ 0.1f, 0.1f, 1.0f }, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.5f, 0.25f, 0.5f} });
	o.push_back({ "cube.obj", glm::vec3{ 0.1f, 0.1f, 1.0f }, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{-0.5f, 0.25f, 0.5f} });

	o.push_back({ "cube.obj", glm::vec3{ 0.5f, 0.5f, 0.5f }, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f} });


	o.push_back({ "cube.obj", glm::vec3{ 0.1f, 1.0f, 0.1f }, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.25f, 1.0f, 0.0f} });
	o.push_back({ "cube.obj", glm::vec3{ 0.1f, 1.0f, 0.1f }, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{-0.25f, 1.0f, 0.0f} });

	proj = glm::mat4(1.0f);
	proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f); //--- 투영 공간 설정: fovy, aspect, near, far
	proj = glm::translate(proj, glm::vec3(0.0, 0.0, -5.0));
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
	glutKeyboardFunc(Keyboard);
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
	if (viewport)
	{
		proj = glm::mat4(1.0f);
		proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f); //--- 투영 공간 설정: fovy, aspect, near, far
		proj = glm::translate(proj, glm::vec3(0.0, 0.0, -5.0));
		glViewport(0, 0, 400, 800);
		bg.render(shaderProgramID);
		for (object& i : o)
		{
			i.render(shaderProgramID);
		}

		
		proj = glm::mat4(1.0f);
		proj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, -20.0f, 20.0f);
		cameraPos = { 0.0f, 5.0f, 0.0f };
		cameraDirection = { 0.0f,0.0f,0.1f };
		glViewport(400, 0, 400, 400);
		bg.render(shaderProgramID);
		for (object& i : o)
		{
			i.render(shaderProgramID);
		}

		proj = glm::mat4(1.0f);
		proj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, -20.0f, 20.0f);
		cameraPos = { 0.0f, 0.0f, 5.0f };
		cameraDirection = { 0.0f,0.0f,0.0f };
		glViewport(400, 400, 400, 400);
		bg.render(shaderProgramID);
		for (object& i : o)
		{
			i.render(shaderProgramID);
		}

		cameraPos = tmppos;
	}
	else
	{
		glViewport(0, 0, 800, 800);
		bg.render(shaderProgramID);
		for (object& i : o)
		{
			i.render(shaderProgramID);
		}
	}


	glutSwapBuffers(); //--- 화면에 출력하기
}
//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'b':
		b.first = !b.first;
		b.second = true;
		break;
		
	case 'B':
		b.first = !b.first;
		b.second = false;
		break;

	case 'm':
		m.first = !m.first;
		m.second = true;
		break;

	case 'M':
		m.first = !m.first;
		m.second = false;
		break;

	case 'f':
		f.first = !f.first;
		f.second = true;
		break;

	case 'F':
		f.first = !f.first;
		f.second = false;
		break;

	case 'e':
		e.first = !e.first;
		e.second = true;
		break;

	case 'E':
		e.first = !e.first;
		e.second = false;
		break;

	case 't':
		t.first = !t.first;
		t.second = true;
		break;

	case 'T':
		t.first = !t.first;
		t.second = false;
		break;

	case 'z':
		cameraPos.z += 1.0f;
		cameraDirection.z += 1.0f;
		break;

	case 'Z':
		cameraPos.z -= 1.0f;
		cameraDirection.z -= 1.0f;
		break;

	case 'x':
		cameraPos.x += 1.0f;
		cameraDirection.x += 1.0f;
		break;

	case 'X':
		cameraPos.x -= 1.0f;
		cameraDirection.x -= 1.0f;
		break;

	case 'y':
		y_.first = !y_.first;
		cnt = 0;
		break;

	case 'r':
		r.first = !r.first;
		cnt = 0;
		break;

	case 'a':
		a.first = !a.first;
		cnt = 0;
		break;

	case 'p': // 직각투영?
		proj = glm::mat4(1.0f);
		proj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, -20.0f, 20.0f);
		break;

	case 'P': // 원근투영?
		proj = glm::mat4(1.0f);
		proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f); //--- 투영 공간 설정: fovy, aspect, near, far
		proj = glm::translate(proj, glm::vec3(0.0, 0.0, -5.0));
		break;


	case 'q': // 프로그램 종료
		exit(0);
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{

	if (b.first)
	{
		if (o[0].transition.x < 10.0f && o[0].transition.x > -10.0f)
			o[0].move(0, b.second);
	}

	origin = o[0].GETORIGIN();
	for (int i = 1; i < o.size(); ++i)
		o[i].Origin = o[0].GETORIGIN();
	
	if (m.first)
	{
		for (int i = 3; i < o.size(); ++i)
			o[i].rotbyorigin(1, m.second, origin);
	}
	else
	{
		for (int i = 3; i < o.size(); ++i)
			o[i].rotPoint = origin;
	}

	if (f.first)
	{
		if (o[1].GETROTORIGIN().y > 180.0f || o[1].GETROTORIGIN().y < 0.0f)
			f.second = !f.second;

		o[1].rotbyorigin(1, f.second, { origin.x + 0.5f, origin.y + 0.25f, origin.z  });
		o[2].rotbyorigin(1, !f.second, { origin.x - 0.5f, origin.y + 0.25f, origin.z });
	}
	else
	{
		if (o[1].GETROTORIGIN().y - 0.0f > FLT_EPSILON)
		{
			o[1].rotbyorigin(1, !f.second, origin);
			o[2].rotbyorigin(1, f.second, origin);
		}
	}

	if (e.first)
	{
		if (o[1].GETROTORIGIN().y - 0.0f < FLT_EPSILON)
		{
			if (o[1].transition.x > 0.5f || o[1].transition.x <= 0.0f)
				e.second = !e.second;

			o[1].move(0, e.second);
			o[2].move(0, !e.second);
		}
	}
	else
	{
		if (0.5f - o[1].transition.x > FLT_EPSILON)
		{
			o[1].move(0, e.second);
			o[2].move(0, !e.second);
		}
	}

	for (object& i : o)
		i.update();
	bg.update();

	if (t.first)
	{
		if (o[4].GETROTORIGIN().x > 10.0f || o[4].GETROTORIGIN().x < -10.0f)
			t.second = !t.second;
		o[4].rotbyorigin(0, t.second, { o[4].rotPoint.x, o[4].rotPoint.y + 0.5f, o[4].rotPoint.z });
		o[5].rotbyorigin(0, !t.second, { o[5].rotPoint.x, o[5].rotPoint.y + 0.5f, o[5].rotPoint.z });
	}
	else
	{
		if (abs(o[4].GETROTORIGIN().x - 0.0f) > FLT_EPSILON)
		{
			if (t.second)
			{
				o[4].rotbyorigin(0, t.second, { o[4].rotPoint.x, o[4].rotPoint.y + 0.5f, o[4].rotPoint.z });
				o[5].rotbyorigin(0, !t.second, { o[5].rotPoint.x, o[5].rotPoint.y + 0.5f, o[5].rotPoint.z });
			}
			else
			{
				o[4].rotbyorigin(0, !t.second, { o[4].rotPoint.x, o[4].rotPoint.y + 0.5f, o[4].rotPoint.z });
				o[5].rotbyorigin(0, t.second, { o[5].rotPoint.x, o[5].rotPoint.y + 0.5f, o[5].rotPoint.z });
			}
		}
	}
	
	if (y_.first)
	{
		setView({ 0.0f, 0.0f, 0.0f });
		cnt = (cnt + 5) % int(pts2.size());
		cameraDirection = pts2[cnt];
	}

	if (r.first)
	{
		if (b.first && b.second)
			cameraPos.x += 1.0f;
		if (b.first && !b.second)
			cameraPos.x -= 1.0f;
		setRoute(origin);
		cnt = (cnt + 5) % int(pts.size());
		cameraPos = pts[cnt];
	}

	if (a.first)
	{
		setRoute({ 0.0f, 0.0f, 0.0f });
		cnt = (cnt + 5) % int(pts.size());
		cameraPos = pts[cnt];
	}

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