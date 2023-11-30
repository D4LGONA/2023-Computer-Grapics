#include "stdafx.h"
#include "object.h"

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

int cnt = 0;
bool isDrag = false;
vector<object> v;
vector<object> lines;

int timertime = 50;
POINT mousept;
glm::vec3 dragLine[2];
POINT linePt[2];
glm::vec3 lineColor[2] = { {0.0f, 1.0f, 1.0f }, {0.0f, 1.0f, 1.0f} };

POINT basketPt[4] = { {400, 760}, {400, 800}, {600, 800}, {600, 760} };
glm::vec3 basket[4];
glm::vec3 basketColor[4] = { {1.0f, 1.0f, 0.0}, {1.0f, 1.0f, 0.0}, {1.0f, 1.0f, 0.0}, {1.0f, 1.0f, 0.0} };
glm::vec2 basketDir = {1.0f, 0.0f};

bool drawLine = true;

void render()
{
	glBindVertexArray(vao);

	glm::mat4 matrix = glm::mat4(1.0f);

	// Location 번호 저장
	unsigned int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position"); //	: 0  Shader의 'layout (location = 0)' 부분
	unsigned int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color"); //	: 1
	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "transform");

	glEnableVertexAttribArray(PosLocation); // Enable 필수! 사용하겠단 의미
	glEnableVertexAttribArray(ColorLocation);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // 정점 좌표용 VBO 바인딩
	glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // 색상 데이터용 VBO 바인딩
	glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(matrix));

	glLineWidth(2.0f);
	if (isDrag)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 2, dragLine, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 2, lineColor, GL_DYNAMIC_DRAW);

		glDrawArrays(GL_LINES, 0, 2);
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 4, basket, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 4, basketColor, GL_DYNAMIC_DRAW);

	glDrawArrays(GL_POLYGON, 0, 4);

	glDisableVertexAttribArray(PosLocation); // Disable 필수!
	glDisableVertexAttribArray(ColorLocation);
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
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(Keyboard);

	glutTimerFunc(timertime, TimerFunction, 1);

	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	
	if(drawLine)
		for (object& i : lines)
			if(i.isrender)
				i.render(shaderProgramID, true);

	for (object& i : v)
		i.render(shaderProgramID, false);

	render();


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
	case 'w':
		isSolid = false;
		break;

	case 's':
		isSolid = true;
		break;

	case 'l':
		drawLine = !drawLine;
		break;

	case '+':
		if(timertime > 0)
			timertime-= 5;
		break;

	case '-':
		if (timertime < 100)
			timertime+=5;
		break;

	case 'q': // 프로그램 종료
		for (int i = 0; i < v.size(); ++i)
			v[i].remove();

		exit(0);
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	cnt++;

	if (cnt >= 10)
	{
		v.push_back({ uidS(dre), 30 /400. });
		lines.push_back(v.back().getRoute());
		cnt = 0;
	}

	for (object& i : v)
	{
		if (i.isput)
			i.move(basketDir);
		else
			i.move();

	}

	for (object& i : v)
		i.update();
	
	// 바구니 위치 조정
	for (int i = 0; i < 4; ++i)
	{
		basketPt[i].x += basketDir.x * 10;
	}

	// 바구니 충돌 체크
	auto a = min_element(basketPt, basketPt + 4, [](const POINT& a, const POINT& b) {return a.x < b.x; });
	auto b = max_element(basketPt, basketPt + 4, [](const POINT& a, const POINT& b) {return a.x < b.x; });
	if (a->x <= 0)
	{
		int dx = 0 - a->x;
		for (int i = 0; i < 4; ++i)
			basketPt[i].x -= dx;
		basketDir.x *= -1.0f;
	}
	if (b->x >= 800)
	{
		int dx = 800 - b->x;
		for (int i = 0; i < 4; ++i)
			basketPt[i].x += dx;
		basketDir.x *= -1.0f;
	}

	for (int i = 0; i < 4; ++i)
	{
		pair<float, float> tmp = WintoOpenGL(basketPt[i]);
		basket[i].x = tmp.first;
		basket[i].y = tmp.second;
		basket[i].z = 0.0f;
	}

	for (int i = 0; i < v.size(); ++i)
	{
		if (v[i].isRemove())
		{
			i--;
			v[i+1].remove();
			v.erase(v.begin() + (i + 1));
			lines.erase(lines.begin() + (i + 1));
		}
	}

	for (int i = 0; i < v.size(); ++i)
	{
		if (v[i].isIntersect(basket[0], basketDir))
			lines[i].isrender = false;
	}
	

	

	glutPostRedisplay();
	glutTimerFunc(timertime, TimerFunction, 1);
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		isDrag = true;
		pair<float, float> tmp = WintoOpenGL({ x,y });
		dragLine[0] = { tmp.first, tmp.second, 0.0f };
		dragLine[1] = { tmp.first, tmp.second, 0.0f };
		linePt[0] = { x,y };
		linePt[1] = { x,y };
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		isDrag = false;
		vector<int> arr;
		int max = v.size();
		for(int count = 0; count < max; ++count)
		{
			pair<bool, pair<vector<glm::vec3>, vector<glm::vec3>>> tmp = v[count].isCross(dragLine[0], dragLine[1]);
			if (tmp.first)
			{
				glm::vec2 tmpdir = v[count].GETDIR();

				if (tmpdir.x < 0.0f)
				{
					v.push_back({ tmp.second.first, {tmpdir.x / 2.0f, tmpdir.y}, v[count].S, v[count].R, v[count].T});
					v.back().isSliced = true;
					lines.push_back({ v.back().getRoute()});
					v.push_back({ tmp.second.second, {tmpdir.x / -2.0f, tmpdir.y} , v[count].S, v[count].R, v[count].T });
					v.back().isSliced = true;
					lines.push_back({ v.back().getRoute() });
				}
				else
				{
					v.push_back({ tmp.second.first, {tmpdir.x / -2.0f, tmpdir.y} , v[count].S, v[count].R, v[count].T });
					v.back().isSliced = true;
					lines.push_back({ v.back().getRoute() });
					v.push_back({ tmp.second.second, {tmpdir.x / 2.0f, tmpdir.y} , v[count].S, v[count].R, v[count].T });
					v.back().isSliced = true;
					lines.push_back({ v.back().getRoute() });
				}
				
				arr.push_back(count);
			}
		}

		for (int i = 0; i < arr.size(); ++i)
		{
			v.erase(v.begin() + (arr[i] - i));
			lines.erase(lines.begin() + (arr[i] - i));
		}
	}
	return GLvoid();
}

GLvoid Motion(int x, int y)
{
	if (isDrag)
	{
		pair<float, float> tmp = WintoOpenGL({ x,y });
		dragLine[1] = { tmp.first, tmp.second, 0.0f };
		linePt[1] = { x,y };
	}
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
	if(!result)
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