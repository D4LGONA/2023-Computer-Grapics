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
void InitBuffer();
char* filetobuf(const char*);

POINT mousept;
vector<object> objs;
object* selected = nullptr;
bool isDepthTest = true;
bool rotatingY = false;
bool topRotate = false;
bool openFront = false;
bool slideSide = false;
bool backs = false;
bool openall = false;
bool isOpen[6] = { false, false, false, false, false, false };
//glm::mat4 view = glm::mat4(1.0f);

vector<glm::vec3> lines = { 
	{1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},
	{0.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f},
	{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}
};

vector<glm::vec3> lineColor = {
	{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}
};

void Reset()
{
	objs.clear();
	objs.push_back({ 1.0f,1.0f,1.0f, 0, vbo });
	objs.push_back({ 1.0f,1.0f,1.0f, 1, vbo });
	selected = &objs[0];

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
	InitBuffer();
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);
	
	if(isDepthTest)
		glEnable(GL_DEPTH_TEST); 
	else
		glDisable(GL_DEPTH_TEST); 

	selected->render(vbo, ebo, shaderProgramID);

	//glm::mat4 axis{ 1.0f };
	//axis = glm::rotate(axis, glm::radians(-45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
	////axis = glm::rotate(axis, glm::radians(float(angleY)), glm::vec3(0.0f, 1.0f, 0.0f));
	//glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(axis));

	//for (int i = 0; i < lines.size(); i += 2)
	//{
	//	glDrawArrays(GL_LINES, i, i + 2);
	//}
	
	//glDisableVertexAttribArray(PosLocation); // Disable 필수!
	//glDisableVertexAttribArray(ColorLocation);

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
	case 'h':
		isDepthTest = !isDepthTest;
		break;

	case 'y': // y축 자전
		rotatingY = !rotatingY;
		selected->SETROTATING(rotatingY);
		break;

	case 't': // 육면체 윗면 회전?
		selected = &objs[0];
		selected->bind(vbo);
		topRotate = !topRotate;
		selected->SETTROTATING(topRotate);
		break;
		
	case 'f': // 앞면을 열고 닫아요?
		selected = &objs[0];
		selected->bind(vbo);
		openFront = !openFront;
		selected->SETOPENF(openFront);
		break;

	case 's': // 옆면을 슬라이드해서 열고 닫기
		selected = &objs[0];
		selected->bind(vbo);
		slideSide = !slideSide;
		selected->SETSLIDES(slideSide);
		break;

	case 'b': // 뒷면 열고 닫기 ㅠㅠ? 크기 변경?
		selected = &objs[0];
		selected->bind(vbo);
		backs = !backs;
		selected->SETBACK(backs);
		break;

	case 'o': // 사각뿔의 모든 면들이 열린다 / 닫힌다
		selected = &objs[1];
		selected->bind(vbo);
		openall = !openall;
		selected->SETOPENALL(openall);
		isOpen[0] = !isOpen[0];
		selected->SETOPEN(0, isOpen[0]);
		isOpen[1] = !isOpen[1];
		selected->SETOPEN(1, isOpen[1]);
		isOpen[4] = !isOpen[4];
		selected->SETOPEN(4, isOpen[4]);
		isOpen[5] = !isOpen[5];
		selected->SETOPEN(5, isOpen[5]);
		break;

	case '0': // 한개씩 열기
		selected = &objs[1];
		selected->bind(vbo);
		isOpen[0] = !isOpen[0];
		selected->SETOPEN(0, isOpen[0]);
		break;
	case '1': // 한개씩 열기
		selected = &objs[1];
		selected->bind(vbo);
		isOpen[1] = !isOpen[1];
		selected->SETOPEN(1, isOpen[1]);
		break;
	case '4': // 한개씩 열기
		selected = &objs[1];
		selected->bind(vbo);
		isOpen[4] = !isOpen[4];
		selected->SETOPEN(4, isOpen[4]);
		break;
	case '5': // 한개씩 열기
		selected = &objs[1];
		selected->bind(vbo);
		isOpen[5] = !isOpen[5];
		selected->SETOPEN(5, isOpen[5]);
		break;

	case 'p': // 직각투영?
		proj = glm::mat4(1.0f);
		proj = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f);
		break;

	case 'P': // 원근투영?
		proj = glm::mat4(1.0f);
		proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f); //--- 투영 공간 설정: fovy, aspect, near, far
		proj = glm::translate(proj, glm::vec3(0.0, 0.0, -5.0));
		break;

	case 'R': // reset
		Reset();
		break;

	case 'q': // 프로그램 종료
		exit(0);
		break;
	}
	//update();
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	for(object& i : objs)
		i.update();

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

void InitBuffer()
{
	glGenVertexArrays(1, &vao); // VAO를 생성하고 할당합니다.
	glBindVertexArray(vao); // VAO를 바인드합니다.
	glGenBuffers(2, vbo); // 2개의 VBO를 생성하고 할당합니다.

	/*glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int), cubeIdx.data(), GL_STATIC_DRAW);*/

	// 첫 번째 VBO에 정점 좌표 데이터를 설정합니다.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * lines.size(), lines.data(), GL_STATIC_DRAW);

	// 두 번째 VBO에 색상 데이터를 설정합니다.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * lineColor.size(), lineColor.data(), GL_STATIC_DRAW);

	// 정점 좌표 데이터를 VAO에 바인딩하고 활성화합니다.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// 색상 데이터를 VAO에 바인딩하고 활성화합니다.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
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