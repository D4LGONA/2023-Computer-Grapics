#include "stdafx.h"

random_device rd;
mt19937 dre(rd());
uniform_real_distribution<float> uidC{ 0.0f, 1.0f }; // 랜덤 컬러 생성

GLuint vao, vbo[2];

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

vertex v; // 정점이 저장되어있는 녀석
vector<POINT> pts;
bool isSmaller[4] = { false };
int state = 0;
int biggernumber = -1;

pair<float, float> WintoOpenGL(int x, int y)
{
	pair<float, float> a = { x / 400. - 1, 1 - (y / 400.) };
	return a;
}

void RESET()
{
	// 선
	pts.push_back({ 100, 300 });
	pts.push_back({ 200, 100 });
	pts.push_back({ 200, 100 });
	color c = { uidC(dre), uidC(dre), uidC(dre) };

	v.c.push_back(c);
	v.c.push_back(c);
	v.c.push_back(c);

	//삼각형
	pts.push_back({ 500, 300 });
	pts.push_back({ 700, 300 });
	pts.push_back({ 600, 100 });
	pts.push_back({ 600, 100 });
	pts.push_back({ 700, 300 });
	pts.push_back({ 600, 100 });
	c = { uidC(dre), uidC(dre), uidC(dre) };

	v.c.push_back(c);
	v.c.push_back(c);
	v.c.push_back(c);
	v.c.push_back(c);
	v.c.push_back(c);
	v.c.push_back(c);

	//사각형
	pts.push_back({ 100, 500 });
	pts.push_back({ 100, 700 });
	pts.push_back({ 200, 500 });
	pts.push_back({ 300, 500 });
	pts.push_back({ 200, 500 });
	pts.push_back({ 300, 700 });
	pts.push_back({ 100, 700 });
	pts.push_back({ 300, 700 });
	pts.push_back({ 200, 500 });
	c = { uidC(dre), uidC(dre), uidC(dre) };

	v.c.push_back(c);
	v.c.push_back(c);
	v.c.push_back(c);
	v.c.push_back(c);
	v.c.push_back(c);
	v.c.push_back(c);
	v.c.push_back(c);
	v.c.push_back(c);
	v.c.push_back(c);

	//오각형
	pts.push_back({ 500, 550 });
	pts.push_back({ 550, 700 });
	pts.push_back({ 600, 500 });
	pts.push_back({ 550, 700 });
	pts.push_back({ 650, 700 });
	pts.push_back({ 600, 500 });
	pts.push_back({ 650, 700 });
	pts.push_back({ 700, 550 });
	pts.push_back({ 600, 500 });
	c = { uidC(dre), uidC(dre), uidC(dre) };

	v.c.push_back(c);
	v.c.push_back(c);
	v.c.push_back(c);
	v.c.push_back(c);
	v.c.push_back(c);
	v.c.push_back(c);
	v.c.push_back(c);
	v.c.push_back(c);
	v.c.push_back(c);
}

void update()
{
	v.pt.clear();

	for (int i = 0; i < pts.size(); ++i)
	{
		vec3 tmp = { WintoOpenGL(pts[i].x, pts[i].y).first, WintoOpenGL(pts[i].x, pts[i].y).second, 0.0f };
		v.pt.push_back(tmp);
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * v.pt.size(), v.pt.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color) * v.c.size(), v.c.data(), GL_DYNAMIC_DRAW);
}

void tobigger(int n)
{
	switch (n)
	{
	case 0:
		pts[0] = { pts[0].x * 2, pts[0].y * 2};
		pts[1] = { pts[1].x * 2, pts[1].y * 2};
		pts[2] = { pts[2].x * 2, pts[2].y * 2};
		break;
	case 1:
		pts[3] = { (pts[3].x - 400) * 2, pts[3].y * 2 };
		pts[4] = { (pts[4].x - 400) * 2, pts[4].y * 2 };
		pts[5] = { (pts[5].x - 400) * 2, pts[5].y * 2 };
		pts[6] = { (pts[6].x - 400) * 2, pts[6].y * 2 };
		pts[7] = { (pts[7].x - 400) * 2, pts[7].y * 2 };
		pts[8] = { (pts[8].x - 400) * 2, pts[8].y * 2 };
		break;
	case 2:
		pts[9] = { pts[9].x * 2, (pts[9].y - 400) * 2 };
		pts[10] = { pts[10].x * 2, (pts[10].y - 400) * 2 };
		pts[11] = { pts[11].x * 2, (pts[11].y - 400) * 2 };
		pts[12] = { pts[12].x * 2, (pts[12].y - 400) * 2 };
		pts[13] = { pts[13].x * 2, (pts[13].y - 400) * 2 };
		pts[14] = { pts[14].x * 2, (pts[14].y - 400) * 2 };
		pts[15] = { pts[15].x * 2, (pts[15].y - 400) * 2 };
		pts[16] = { pts[16].x * 2, (pts[16].y - 400) * 2 };
		pts[17] = { pts[17].x * 2, (pts[17].y - 400) * 2 };
		break;
	case 3:
		pts[18] = { (pts[18].x - 400) * 2, (pts[18].y - 400) * 2 };
		pts[19] = { (pts[19].x - 400) * 2, (pts[19].y - 400) * 2 };
		pts[20] = { (pts[20].x - 400) * 2, (pts[20].y - 400) * 2 };
		pts[21] = { (pts[21].x - 400) * 2, (pts[21].y - 400) * 2 };
		pts[22] = { (pts[22].x - 400) * 2, (pts[22].y - 400) * 2 };
		pts[23] = { (pts[23].x - 400) * 2, (pts[23].y - 400) * 2 };
		pts[24] = { (pts[24].x - 400) * 2, (pts[24].y - 400) * 2 };
		pts[25] = { (pts[25].x - 400) * 2, (pts[25].y - 400) * 2 };
		pts[26] = { (pts[26].x - 400) * 2, (pts[26].y - 400) * 2 };
		break;
	default:
		break;
	}

	update();
}

void tosmaller(int n)
{
	switch (n)
	{
	case 0:
		pts[0] = { pts[0].x / 2, pts[0].y / 2 };
		pts[1] = { pts[1].x / 2, pts[1].y / 2 };
		pts[2] = { pts[2].x / 2, pts[2].y / 2 };
		break;
	case 1:
		pts[3] = { pts[3].x / 2 + 400, pts[3].y / 2 };
		pts[4] = { pts[4].x / 2 + 400, pts[4].y / 2 };
		pts[5] = { pts[5].x / 2 + 400, pts[5].y / 2 };
		pts[6] = { pts[6].x / 2 + 400, pts[6].y / 2 };
		pts[7] = { pts[7].x / 2 + 400, pts[7].y / 2 };
		pts[8] = { pts[8].x / 2 + 400, pts[8].y / 2 };
		break;
	case 2:
		pts[9] =  { pts[9].x  / 2, pts[9].y / 2  + 400};
		pts[10] = { pts[10].x / 2, pts[10].y / 2 + 400};
		pts[11] = { pts[11].x / 2, pts[11].y / 2 + 400};
		pts[12] = { pts[12].x / 2, pts[12].y / 2 + 400};
		pts[13] = { pts[13].x / 2, pts[13].y / 2 + 400};
		pts[14] = { pts[14].x / 2, pts[14].y / 2 + 400};
		pts[15] = { pts[15].x / 2, pts[15].y / 2 + 400};
		pts[16] = { pts[16].x / 2, pts[16].y / 2 + 400};
		pts[17] = { pts[17].x / 2, pts[17].y / 2 + 400};
		break;
	case 3:
		pts[18] = { pts[18].x / 2 + 400, pts[18].y / 2 + 400 };
		pts[19] = { pts[19].x / 2 + 400, pts[19].y / 2 + 400 };
		pts[20] = { pts[20].x / 2 + 400, pts[20].y / 2 + 400 };
		pts[21] = { pts[21].x / 2 + 400, pts[21].y / 2 + 400 };
		pts[22] = { pts[22].x / 2 + 400, pts[22].y / 2 + 400 };
		pts[23] = { pts[23].x / 2 + 400, pts[23].y / 2 + 400 };
		pts[24] = { pts[24].x / 2 + 400, pts[24].y / 2 + 400 };
		pts[25] = { pts[25].x / 2 + 400, pts[25].y / 2 + 400 };
		pts[26] = { pts[26].x / 2 + 400, pts[26].y / 2 + 400 };
		break;
	default:
		break;
	}
	
	update();
}

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Example1");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	make_shaderProgram();
	InitBuffer();
	RESET();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(100, TimerFunction, 1);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);

	// Location 번호 저장
	int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position"); //	: 0  Shader의 'layout (location = 0)' 부분
	int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color"); //	: 1

	glEnableVertexAttribArray(PosLocation); // Enable 필수! 사용하겠단 의미
	glEnableVertexAttribArray(ColorLocation);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // 정점 좌표용 VBO 바인딩
	glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // 색상 데이터용 VBO 바인딩
	glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(color), 0);

	tobigger(biggernumber);

	if (biggernumber == -1)
		for (int i = 0; i < pts.size(); ++i)
			glDrawArrays(GL_TRIANGLES, i * 3, 3);
	else if (biggernumber == 0)
		glDrawArrays(GL_TRIANGLES, 0, 3);
	else if (biggernumber == 1)
	{
		glDrawArrays(GL_TRIANGLES, 3, 3);
		glDrawArrays(GL_TRIANGLES, 6, 3);
	}
	else if (biggernumber == 2)
	{
		glDrawArrays(GL_TRIANGLES, 9, 3);
		glDrawArrays(GL_TRIANGLES, 12, 3);
		glDrawArrays(GL_TRIANGLES, 15, 3);
	}
	else if (biggernumber == 3)
	{
		glDrawArrays(GL_TRIANGLES, 18, 3);
		glDrawArrays(GL_TRIANGLES, 21, 3);
		glDrawArrays(GL_TRIANGLES, 24, 3);
	}
	
	tosmaller(biggernumber);

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
	case 'l':
		biggernumber = 0;
		break;
	case 't':
		biggernumber = 1;
		break;
	case 'r':
		biggernumber = 2;
		break;
	case 'p':
		biggernumber = 3;
		break;
	case 'a':
		biggernumber = -1;
		break;
	case 'q': // 프로그램 종료
		exit(0);
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	for (int i = 0; i < 4; ++i)
	{
		if (isSmaller[i])
		{
			switch (i)
			{
			case 0: // 1번 애니메이션
				if (pts[2].x > 200) pts[2].x--;
				if (pts[2].y > 100) pts[2].y--;
				if (pts[2].x <= 200 && pts[2].y <= 100)
					isSmaller[i] = false;
				break;
			case 1: // 2번 애니메이션
				if (pts[5].x < 600) pts[5].x++;
				pts[6].x = pts[5].x;
				if (pts[8].x > 600) pts[8].x--;
				if (pts[5].x >= 600 && pts[8].x <= 600)
					isSmaller[i] = false;
				break;
			case 2: // 3번 애니메이션
				if (pts[9].y > 500) pts[9].y--;
				if (pts[10].x > 100) pts[10].x--;
				pts[12].y = pts[9].y;
				pts[14].x = 200 + (200 - pts[10].x);
				pts[15].x = pts[10].x;
				pts[16].x = pts[14].x;
				if (pts[9].y <= 500 && pts[10].x <= 100)
					isSmaller[i] = false;
				break;
			case 3: // 4번 애니메이션
				if (pts[18].x > 500) pts[18].x--;
				if (pts[18].y > 550) pts[18].y--;
				if (pts[19].x > 550) pts[19].x--;
				if (pts[19].y < 700) pts[19].y++;
				if (pts[20].y > 500) pts[20].y--;
				pts[22] = { 600 - pts[19].x + 600, pts[19].y };
				pts[25] = { 600 - pts[18].x + 600, pts[18].y };
				pts[23] = pts[20];
				pts[26] = pts[20];
				pts[21] = pts[19];
				pts[24] = pts[22];
				if (pts[18].x <= 500 && pts[18].y <= 550 && pts[19].x <= 550 && pts[19].y >= 700 && pts[20].y <= 500)
					isSmaller[i] = false;
				break;
			}
		}
		else
		{
			switch (i)
			{
			case 0: // 1번 애니메이션
				if (pts[2].x < 300)pts[2].x++;
				if (pts[2].y < 300)pts[2].y++;
				if (pts[2].x >= 300 && pts[2].y >= 300)
					isSmaller[i] = true;
				break;
			case 1: // 2번 애니메이션
				if (pts[5].x > 500) pts[5].x--;
				pts[6].x = pts[5].x;
				if (pts[8].x < 700) pts[8].x++;
				if (pts[5].x <= 500 && pts[8].x >= 700)
					isSmaller[i] = true;
				break;
			case 2: // 3번 애니메이션
				if (pts[9].y < 550) pts[9].y++;
				if (pts[10].x < 150) pts[10].x++;
				pts[12].y = pts[9].y;
				pts[14].x = 200 + (200 - pts[10].x);
				pts[15].x = pts[10].x;
				pts[16].x = pts[14].x;
				if (pts[9].y >= 550 && pts[10].x >= 150)
					isSmaller[i] = true;
				break;
			case 3: // 4번 애니메이션
				if (pts[18].x < 600) pts[18].x++;
				if (pts[18].y < 600) pts[18].y++;
				if (pts[19].x < 600) pts[19].x++;
				if (pts[19].y > 600) pts[19].y--;
				if (pts[20].y < 600) pts[20].y++;
				pts[22] = { 600 - pts[19].x + 600, pts[19].y };
				pts[25] = { 600 - pts[18].x + 600, pts[18].y };
				pts[23] = pts[20];
				pts[26] = pts[20];
				pts[21] = pts[19];
				pts[24] = pts[22];
				if(pts[18].x >= 600 && pts[18].y >= 600 && pts[19].x >= 600 && pts[19].y <= 600 && pts[20].y >= 600)
					isSmaller[i] = true;
				break;
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

	return GLvoid();
}

GLvoid Motion(int x, int y)
{
	glutPostRedisplay();
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(vao); //--- VAO를 바인드하기
	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기
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