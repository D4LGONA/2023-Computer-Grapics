#include "stdafx.h"

random_device rd;
mt19937 dre(rd());
uniform_real_distribution<float> uidC{ 0.0f, 1.0f }; // 랜덤 컬러 생성
uniform_int_distribution uidS{ 0,3 };

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
void ReadObj(FILE* path);

struct object
{
	GLUquadric* obj;
	glm::mat4 matrix{ 1.0f };
	int angleX = 0;
	int angleY = 0;
	glm::vec3 transition{0.0f, 0.0f, 0.0f};
	int state = 0;
	float scale = 0.2f;
};

object objs[4];

POINT mousept;

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

vector<glm::vec3> cube;
vector<glm::vec3> cubeColor;
vector<unsigned int> cubeIdx;

glm::vec3 transitionL{ 0.5f, 0.0f, 0.0f };
glm::vec3 transitionR{ -0.5f, 0.0f, 0.0f };
int angleY = 0;
int angleZ = 0;

float Gscale = 1.0f;

object* Right = nullptr;
object* Left = nullptr;

bool spiral = false;

bool MoveOrigin = false;
bool ToOrigin = true;

bool Swap = false;

bool swapXZ = false;

bool swapYZ = false;

pair<float, float> tmp;

POINT dir = { -1, 1 };

float dist(int x1, int y1, int x2, int y2)
{
	return sqrt(powf(x2 - x1, 2) + powf(y2 - y1, 2));
}

int length = 0;

void update() // 매트릭스 초기화하고 다시 넘겨 줄 것임
{
	for (int i = 0; i < 4; ++i)
	{
		glm::mat4 tmp{ 1.0f };
		objs[i].matrix = tmp;

		objs[i].matrix = glm::scale(objs[i].matrix, glm::vec3(Gscale, Gscale, Gscale));
		objs[i].matrix = glm::rotate(objs[i].matrix, glm::radians(-45.0f), glm::vec3(1.0f, 1.0f, 0.0f)); // 전체 돌린 것
		//if(angleY % 180 != 0 || angleZ % 180 != 0)
		//	objs[i].matrix = glm::translate(objs[i].matrix, -objs[i].transition); // 이동
		objs[i].matrix = glm::rotate(objs[i].matrix, glm::radians(float(angleY)), glm::vec3(0.0f, 1.0f, 0.0f)); // 공전인가?
		objs[i].matrix = glm::rotate(objs[i].matrix, glm::radians(float(angleZ)), glm::vec3(0.0f, 0.0f, 1.0f)); // 공전인가?
		//objs[i].matrix = glm::translate(objs[i].matrix, objs[i].transition); // 이동
		objs[i].matrix = glm::translate(objs[i].matrix, objs[i].transition); // 이동
		if(&objs[i] == Left)
			objs[i].matrix = glm::translate(objs[i].matrix, transitionL); // 이동
		if (&objs[i] == Right)
			objs[i].matrix = glm::translate(objs[i].matrix, transitionR); // 이동
		objs[i].matrix = glm::rotate(objs[i].matrix, glm::radians(float(objs[i].angleY)), glm::vec3(0.0f, 1.0f, 0.0f)); // 자전
		objs[i].matrix = glm::rotate(objs[i].matrix, glm::radians(float(objs[i].angleX)), glm::vec3(1.0f, 0.0f, 0.0f)); //
		objs[i].matrix = glm::scale(objs[i].matrix, glm::vec3(objs[i].scale, objs[i].scale, objs[i].scale));
	}
}

void bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * cubeIdx.size(), cubeIdx.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * cube.size(), cube.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * cubeColor.size(), cubeColor.data(), GL_DYNAMIC_DRAW);
}

void Reset()
{
	angleY = 0;

	for (int i = 0; i < 4; ++i)
	{
		objs[i].obj = gluNewQuadric();
		gluQuadricDrawStyle(objs[i].obj, GLU_LINE);
		gluQuadricNormals(objs[i].obj, GLU_SMOOTH);
		gluQuadricOrientation(objs[i].obj, GLU_OUTSIDE);
		objs[i].transition = { 0.0f, 0.0f, 0.0f };
		objs[i].state = i;
	}

	Left = &objs[0];
	Right = &objs[1];

	/*Left->transition = transitionL;
	Right->transition = transitionR;*/
}

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
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
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);
	
	glEnable(GL_DEPTH_TEST);

	// Location 번호 저장
	int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position"); //	: 0  Shader의 'layout (location = 0)' 부분
	int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color"); //	: 1
	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "transform");


	glEnableVertexAttribArray(PosLocation); // Enable 필수! 사용하겠단 의미
	glEnableVertexAttribArray(ColorLocation);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // 정점 좌표용 VBO 바인딩
	glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // 색상 데이터용 VBO 바인딩
	glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);


	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Left->matrix));
	switch (Left->state)
	{
	case 0:
		gluSphere(Left->obj, 1.0f, 20, 20);
		break;
	case 1:
		gluCylinder(Left->obj, 1.0f, 0.0f, 2.0f, 20, 8);
		break;
	case 2:
		gluCylinder(Left->obj, 1.0f, 1.0f, 2.0f, 20, 8);
		break;
	case 3:
		gluSphere(Left->obj, 1.0f, 4, 2);
		break;
	}

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Right->matrix));
	switch (Right->state)
	{
	case 0:
		gluSphere(Right->obj, 1.0f, 20, 20);
		break;
	case 1:
		gluCylinder(Right->obj, 1.0f, 0.0f, 2.0f, 20, 8);
		break;
	case 2:
		gluCylinder(Right->obj, 1.0f, 1.0f, 2.0f, 20, 8);
		break;
	case 3:
		gluSphere(Right->obj, 1.0f, 4, 2);
		break;
	}

	glm::mat4 axis{ 1.0f };
	axis = glm::rotate(axis, glm::radians(-45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
	//axis = glm::rotate(axis, glm::radians(float(angleY)), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(axis));

	for (int i = 0; i < lines.size(); i += 2)
	{
		glDrawArrays(GL_LINES, i, i + 2);
	}

	// 그리기 // 
	
	glDisableVertexAttribArray(PosLocation); // Disable 필수!
	glDisableVertexAttribArray(ColorLocation);

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
	case 'x':
		Left->transition.x += 0.1f;
		Right->transition.x += 0.1f;
		break;
	case 'X':
		Left->transition.x -= 0.1f;
		Right->transition.x -= 0.1f;
		break;

	case 'y':
		Left->transition.y += 0.1f;
		Right->transition.y += 0.1f;
		break;
	case 'Y':
		break;

	case 'z':
		break;
	case 'Z':
		break;

	case 's':
		if(Left->scale < 2.0f)
			Left->scale += 0.05f;
		if(Right->scale < 2.0f)
			Right->scale += 0.05f;
		break;
		
	case 'S':
		if(Left->scale > 0.05f)
			Left->scale -= 0.05f;
		if(Right->scale > 0.05f)
			Right->scale -= 0.05f;
		break;

	case '+':
		Gscale += 0.1f;
		break;
		
	case '-':
		Gscale -= 0.1f;
		break;

	case 'r': // 스파이럴
	{
		/*pt = 
		while (true)
		{

		}*/
		spiral = true;
		break;

	}

	case 't': // 원점으로 이동했다 제자리
		tmp.first = Right->transition.x + transitionR.x;
		tmp.second = Left->transition.x + transitionL.x;
		MoveOrigin = true;
		break;

	case '1': // 두 도형이 중심점을 통과하며 상대 자리로 이동
		Swap = true;
		tmp.first = Right->transition.x + transitionR.x;
		tmp.second = Left->transition.x + transitionL.x;

		break;
	case '2': // 뭔소리지
		swapXZ = true;
		break;
	case '3': // 이건 뭔소리지
		swapYZ = true;
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
	if (MoveOrigin)
	{

		if (ToOrigin)
		{
			Right->transition.x += 0.1f;
			Left->transition.x -= 0.1f;
			if (Right->transition.x + transitionR.x >= 0.0f) 
				ToOrigin = false;
		}
		else
		{
			Right->transition.x -= 0.1f;
			Left->transition.x += 0.1f;
			if (Right->transition.x + transitionR.x <= tmp.first)
			{
				/*Right->transition.x += 0.1f;
				Left->transition.x -= 0.1f;*/
				/*Right->transition.x = 0.0f;
				Left->transition.x = 0.0f;*/
				MoveOrigin = false;
				ToOrigin = true;
			}
		}/*
		if (ToOrigin)
		{
			Right->transition.x += 0.1f;
			Left->transition.x -= 0.1f;
			if (Right->transition.x >= Left->transition.x)
				ToOrigin = false;
		}
		else
		{
			Right->transition.x -= 0.1f;
			Left->transition.x += 0.1f;
			if (Right->transition.x <= -1.0f && Left->transition.x >= 1.0f)
				ToOrigin = true;
		}*/
	}

	if (spiral)
	{
		Right->transition.x += 0.01f * dir.x;
		Left->transition.x += 0.01f * dir.y;
		angleY += dir.x * 5;
		if (Right->transition.x >= 1.0f || Left->transition.x >= 1.0f)
			dir = { dir.x * -1, dir.y * -1 };
	}

	if (swapXZ)
	{
		angleY += 5;
		if (angleY % 180 == 0) 
			swapXZ = false;
	}

	if (swapYZ)
	{
		angleZ += 5;
		if (angleZ % 180 == 0)
			swapYZ = false;
	}

	if (Swap)
	{
		if (Right->transition.x + transitionR.x < tmp.second)
		{
			Right->transition.x += 0.1f;
			Left->transition.x -= 0.1f;
			if (Right->transition.x + transitionR.x >= tmp.second) Swap = false;
		}
		else
		{
			Right->transition.x -= 0.1f;
			Left->transition.x += 0.1f;
			if (Right->transition.x + transitionR.x < tmp.second)
			{
				Right->transition.x += 0.1f;
				Left->transition.x -= 0.1f;
				/*Right->transition.x = 0.0f;
				Left->transition.x = 0.0f;*/
				Swap = false;
			}
		}
	}
	update();

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

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int), cubeIdx.data(), GL_STATIC_DRAW);

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

void ReadObj(FILE* path)
{
	char count[128];
	int vertexnum = 0;
	int facenum = 0;
	int uvnum = 0;
	//--- 1. 전체 버텍스 개수 및 삼각형 개수 세기
	while (!feof(path))
	{
		fscanf(path, "%s", count);
		if (count[0] == 'v' && count[1] == '\0')
			vertexnum++;
		else if (count[0] == 'f' && count[1] == '\0')
			facenum++;
		else if (count[0] == 'v' && count[1] == 't' && count[3] == '\0')
			uvnum++;
		memset(count, '\0', sizeof(count));
	}
	rewind(path);
	int vertIndex = 0;
	int faceIndex = 0;
	int uvIndex = 0;
	//--- 2. 메모리 할당
	glm::vec3* vertex = new glm::vec3[vertexnum];
	glm::vec3* face = new glm::vec3[facenum];
	glm::vec3* uvdata = new glm::vec3[facenum];
	glm::vec2* uv = new glm::vec2[uvnum];
	char bind[128];

	while (!feof(path)) {
		fscanf(path, "%s", bind);
		if (bind[0] == 'v' && bind[1] == '\0') {
			fscanf(path, "%f %f %f\n",
				&vertex[vertIndex].x, &vertex[vertIndex].y, &vertex[vertIndex].z);
			vertIndex++;
		}
		else if (bind[0] == 'f' && bind[1] == '\0') {
			unsigned int temp_face[3], temp_uv[3], temp_normal[3];
			fscanf(path, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&temp_face[0], &temp_uv[0], &temp_normal[0],
				&temp_face[1], &temp_uv[1], &temp_normal[1],
				&temp_face[2], &temp_uv[2], &temp_normal[2]);
			face[faceIndex].x = temp_face[0]-1;
			face[faceIndex].y = temp_face[1]-1;
			face[faceIndex].z = temp_face[2]-1;
			uvdata[faceIndex].x = temp_uv[0];
			uvdata[faceIndex].y = temp_uv[1];
			uvdata[faceIndex].z = temp_uv[2];
			faceIndex++;
		}
		else if (bind[0] == 'v' && bind[1] == 't' && bind[2] == '\0') {
			fscanf(path, "%f %f\n", &uv[uvIndex].x, &uv[uvIndex].y);
			uvIndex++;
		}
	}


	for (int i = 0; i < vertexnum; ++i)
	{
		cube.push_back(vertex[i]);
		cubeColor.push_back({ uidC(dre),uidC(dre),uidC(dre) });
	}

	for (int i = 0; i < facenum; ++i)
	{
		cubeIdx.push_back(face[i].x);
		cubeIdx.push_back(face[i].y);
		cubeIdx.push_back(face[i].z);
	}
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