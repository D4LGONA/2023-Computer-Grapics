//#include "stdafx.h"
//
//random_device rd;
//mt19937 dre(rd());
//uniform_real_distribution<float> uidC{ 0.0f, 1.0f }; // 랜덤 컬러 생성
//uniform_int_distribution<int> uid{ 0,5 };
//uniform_int_distribution<int> uid2{ 0,3 };
//
//GLuint vao, vbo[2], ebo;
//
//GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수
//GLuint vertexShader, fragmentShader; //--- 세이더 객체
//GLuint shaderProgramID; //--- 셰이더 프로그램
//
//void make_shaderProgram();
//void make_vertexShaders();
//void make_fragmentShaders();
//GLvoid drawScene();
//GLvoid Reshape(int w, int h);
//GLvoid Keyboard(unsigned char key, int x, int y);
//GLvoid TimerFunction(int value);
//GLvoid Mouse(int button, int state, int x, int y);
//GLvoid Motion(int x, int y);
//void InitBuffer();
//char* filetobuf(const char*);
//
//vector<glm::vec3> v = {
//	{-0.25f, 0.25f,   0.25f}, {-0.25f, -0.25f, 0.25f},  {0.25f, -0.25f, 0.25f},
//	{-0.25f, 0.25f,   0.25f}, {0.25f, -0.25f, 0.25f},   {0.25f, 0.25f, 0.25f}, // 면 1
//	{0.25f, 0.25f,    0.25f}, {0.25f, -0.25f, 0.25f},   {0.25f, -0.25f, -0.25f},
//	{0.25f, 0.25f,    0.25f}, {0.25f, -0.25f, -0.25f},  {0.25f, 0.25f, -0.25f}, // 면 2
//	{0.25f, 0.25f,   -0.25f}, {0.25f, -0.25f, -0.25f},  {-0.25f, -0.25f, -0.25f},
//	{0.25f, 0.25f,   -0.25f}, {-0.25f, -0.25f, -0.25f}, {-0.25f, 0.25f, -0.25f}, // 면 3
//	{-0.25f, 0.25f,  -0.25f}, {-0.25f, -0.25f, -0.25f}, {-0.25f, 0.25f, 0.25f},
//	{-0.25f, -0.25f, -0.25f}, {-0.25f, -0.25f, 0.25f},  {-0.25f, 0.25f, 0.25f}, // 면 4
//	{-0.25f, 0.25f,  -0.25f}, {-0.25f, 0.25f, 0.25f},   {0.25f, 0.25f, 0.25f},
//	{-0.25f, 0.25f,  -0.25f}, {0.25f, 0.25f, 0.25f},    {0.25f, 0.25f, -0.25f}, // 면 5
//	{-0.25f, -0.25f, -0.25f}, {-0.25f, -0.25f, 0.25f},  {0.25f, -0.25f, 0.25f},
//	{-0.25f, -0.25f, -0.25f}, {0.25f, -0.25f, 0.25f},   {0.25f, -0.25f, -0.25f}, // 면 6
//};
//
//vector<glm::vec3> c = {
//	{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
//	{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
//	{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
//	{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
//	{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},
//	{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},
//	{0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f},
//	{0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f},
//	{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f},
//	{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f},
//	{1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f},
//	{1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f},
//};
//
//vector<glm::vec3> v1 = {
//	{-0.25f, -0.25f, -0.25f}, {-0.25f, -0.25f, 0.25f},  {0.25f, -0.25f, 0.25f},
//	{-0.25f, -0.25f, -0.25f}, {0.25f, -0.25f, 0.25f},   {0.25f, -0.25f, -0.25f},
//	{-0.25f, -0.25f, 0.25f}, {0.25f, -0.25f, 0.25f}, {0.0f, 0.5f, 0.0f},
//	{0.25f, -0.25f, 0.25f}, {0.25f, -0.25f, -0.25f}, {0.0f, 0.5f, 0.0f},
//	{0.25f, -0.25f, -0.25f}, {-0.25f, -0.25f, -0.25f}, {0.0f, 0.5f, 0.0f},
//	{-0.25f, -0.25f, -0.25f}, {-0.25f, -0.25f, 0.25f}, {0.0f, 0.5f, 0.0f}
//};
//
//vector<glm::vec3> c1 = {
//	{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},
//	{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},
//	{1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f},
//	{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f},
//	{0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f},
//	{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
//};
//
//POINT mousept;
//
//glm::vec3 transition{ 0.0f,0.0f,0.0f };
//int angleX = 0;
//int angleY = 0;
//bool isX = false;
//bool isY = false;
//bool isMinus = false;
//bool isRect = true;
//bool isHide = true;
//bool isSolid = true;
//float X = 0.0f, Y = 0.0f;
//
//pair<float, float> WintoOpenGL(int x, int y)
//{
//	pair<float, float> a = { x / 400. - 1, 1 - (y / 300.) };
//	return a;
//}
//
//float dist(int x1, int y1, int x2, int y2)
//{
//	return sqrt(powf(x2 - x1, 2) + powf(y2 - y1, 2));
//}
//
//void update()
//{
//
//}
//
//void Reset()
//{
//	angleX = 0;
//	angleY = 0;
//	isX = false;
//	isY = false;
//	isMinus = false;
//	isRect = true;
//	isHide = true;
//	isSolid = true;
//	X = 0.0f;
//	Y = 0.0f;
//
//	v.clear();
//	c.clear();
//	v1.clear();
//	c1.clear();
//
//	v = { 
//	{-0.25f, 0.25f,   0.25f}, {-0.25f, -0.25f, 0.25f},  {0.25f, -0.25f, 0.25f},
//	{-0.25f, 0.25f,   0.25f}, {0.25f, -0.25f, 0.25f},   {0.25f, 0.25f, 0.25f}, // 면 1
//	{0.25f, 0.25f,    0.25f}, {0.25f, -0.25f, 0.25f},   {0.25f, -0.25f, -0.25f},
//	{0.25f, 0.25f,    0.25f}, {0.25f, -0.25f, -0.25f},  {0.25f, 0.25f, -0.25f}, // 면 2
//	{0.25f, 0.25f,   -0.25f}, {0.25f, -0.25f, -0.25f},  {-0.25f, -0.25f, -0.25f},
//	{0.25f, 0.25f,   -0.25f}, {-0.25f, -0.25f, -0.25f}, {-0.25f, 0.25f, -0.25f}, // 면 3
//	{-0.25f, 0.25f,  -0.25f}, {-0.25f, -0.25f, -0.25f}, {-0.25f, 0.25f, 0.25f},
//	{-0.25f, -0.25f, -0.25f}, {-0.25f, -0.25f, 0.25f},  {-0.25f, 0.25f, 0.25f}, // 면 4
//	{-0.25f, 0.25f,  -0.25f}, {-0.25f, 0.25f, 0.25f},   {0.25f, 0.25f, 0.25f},
//	{-0.25f, 0.25f,  -0.25f}, {0.25f, 0.25f, 0.25f},    {0.25f, 0.25f, -0.25f}, // 면 5
//	{-0.25f, -0.25f, -0.25f}, {-0.25f, -0.25f, 0.25f},  {0.25f, -0.25f, 0.25f},
//	{-0.25f, -0.25f, -0.25f}, {0.25f, -0.25f, 0.25f},   {0.25f, -0.25f, -0.25f}, // 면 6
//	};
//
//	c = {
//	{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
//	{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
//	{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
//	{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
//	{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},
//	{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},
//	{0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f},
//	{0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f},
//	{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f},
//	{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f},
//	{1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f},
//	{1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f},
//	};
//
//	v1 = {
//	{-0.25f, -0.25f, -0.25f}, {-0.25f, -0.25f, 0.25f},  {0.25f, -0.25f, 0.25f},
//	{-0.25f, -0.25f, -0.25f}, {0.25f, -0.25f, 0.25f},   {0.25f, -0.25f, -0.25f},
//	{-0.25f, -0.25f, 0.25f}, {0.25f, -0.25f, 0.25f}, {0.0f, 0.5f, 0.0f},
//	{0.25f, -0.25f, 0.25f}, {0.25f, -0.25f, -0.25f}, {0.0f, 0.5f, 0.0f},
//	{0.25f, -0.25f, -0.25f}, {-0.25f, -0.25f, -0.25f}, {0.0f, 0.5f, 0.0f},
//	{-0.25f, -0.25f, -0.25f}, {-0.25f, -0.25f, 0.25f}, {0.0f, 0.5f, 0.0f}
//	};
//
//	c1 = {
//	{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},
//	{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},
//	{1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f},
//	{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f},
//	{0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f},
//	{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
//	};
//}
//
//void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
//{
//	//--- 윈도우 생성하기
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
//	glutInitWindowPosition(100, 100);
//	glutInitWindowSize(800, 800);
//	glutCreateWindow("Example1");
//	//--- GLEW 초기화하기
//	glewExperimental = GL_TRUE;
//	glewInit();
//	make_shaderProgram();
//	InitBuffer();
//	glutDisplayFunc(drawScene);
//	glutReshapeFunc(Reshape);
//	glutMouseFunc(Mouse);
//	glutMotionFunc(Motion);
//	glutKeyboardFunc(Keyboard);
//	Reset();
//	glutTimerFunc(100, TimerFunction, 1);
//	glutMainLoop();
//}
//
//GLvoid drawScene()
//{
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glUseProgram(shaderProgramID);
//	glBindVertexArray(vao);
//	//angle += 10;
//
//	if (isHide)
//		glEnable(GL_DEPTH_TEST);
//	else
//		glDisable(GL_DEPTH_TEST);
//
//	glm::mat4 model = glm::mat4(1.0f);
//	model = glm::translate(model, glm::vec3(X, Y, -0.5f));
//	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 1.0f, 0.0f));
//	model = glm::rotate(model, glm::radians(float(angleX)), glm::vec3(1.0f, 0.0f, 0.0f));
//	model = glm::rotate(model, glm::radians(float(angleY)), glm::vec3(0.0f, 1.0f, 0.0f));
//
//
//	// Location 번호 저장
//	int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position"); //	: 0  Shader의 'layout (location = 0)' 부분
//	int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color"); //	: 1
//	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "transform");
//
//
//	glEnableVertexAttribArray(PosLocation); // Enable 필수! 사용하겠단 의미
//	glEnableVertexAttribArray(ColorLocation);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // 정점 좌표용 VBO 바인딩
//	glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // 색상 데이터용 VBO 바인딩
//	glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
//
//	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
//
//	if (isRect)
//	{
//		if (!isSolid)
//			for (int i = 0; i < v.size(); i += 3)
//				glDrawArrays(GL_LINES, i, i + 3);
//		else
//			for (int i = 0; i < v.size(); i += 3)
//				glDrawArrays(GL_TRIANGLES, i, i + 3);
//	}
//	else
//	{
//		if (!isSolid)
//			for (int i = 0; i < v1.size(); i += 3)
//				glDrawArrays(GL_LINES, i, i + 3);
//		else
//			for(int i = 0; i < v1.size(); i += 3)
//				glDrawArrays(GL_TRIANGLES, i, i + 3);
//	}
//
//	
//
//	glDisableVertexAttribArray(PosLocation); // Disable 필수!
//	glDisableVertexAttribArray(ColorLocation);
//
//	glutSwapBuffers(); //--- 화면에 출력하기
//}
////--- 다시그리기 콜백 함수
//GLvoid Reshape(int w, int h)
//{
//	glViewport(0, 0, w, h);
//}
//
//GLvoid Keyboard(unsigned char key, int x, int y)
//{
//	switch (key)
//	{
//	case 'c': // 육면체 나와라 얍
//		isRect = true;
//		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * v.size(), v.data(), GL_STATIC_DRAW);
//		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * c.size(), c.data(), GL_STATIC_DRAW);
//		break;
//
//	case 'p': // 사각뿔 나와라 얍
//		isRect = false;
//		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * v1.size(), v1.data(), GL_STATIC_DRAW);
//		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * c1.size(), c1.data(), GL_STATIC_DRAW);
//		break;
//
//	case 'h': // 은면제거 적용/해제
//		isHide = !isHide;
//		break;
//
//	case 'w': // 와이어 객체
//		isSolid = false;
//		break;
//
//	case 'W': // 솔리드 객체
//		isSolid = true;
//		break;
//
//	case 'x': // x축기준 양의 방향 회전
//		isX = true;
//		isY = false;
//		isMinus = false;
//		break;
//
//	case 'X': // x축기준 음의 방향 회전
//		isX = true;
//		isY = false;
//		isMinus = true;
//		break;
//
//	case 'y': // y축기준 양의방양 회전
//		isX = false;
//		isY = true;
//		isMinus = false;
//		break;
//
//	case 'Y': // y축기준 음이ㅡ방향 회전
//		isX = false;
//		isY = true;
//		isMinus = true;
//		break;
//
//	case '1': //위로이동
//		Y += 0.05f;
//		break;
//
//	case '2': //아래로 이동
// 		Y -= 0.05f;
//		break;
//
//	case '3': // 오른쪽 이동
//		X += 0.05f;
//		break;
//
//	case '4': // 왼쪽이동
//		X -= 0.05f;
//		break;
//
//	case 's': // 초기화
//		Reset();
//		break;
//
//	case VK_DOWN:
//		X = 0;
//		break;
//
//	case 'q': // 프로그램 종료
//		exit(0);
//		break;
//	}
//	glutPostRedisplay();
//}
//
//GLvoid TimerFunction(int value)
//{
//	if (isX)
//	{
//		if (isMinus) angleX--;
//		else angleX++;
//	}
//	if (isY)
//	{
//		if (isMinus) angleY--;
//		else angleY++;
//	}
//	glutPostRedisplay();
//	glutTimerFunc(100, TimerFunction, 1);
//}
//
//GLvoid Mouse(int button, int state, int x, int y)
//{
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
//	{
//	}
//	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
//	{
//	}
//
//	return GLvoid();
//}
//
//GLvoid Motion(int x, int y)
//{
//
//	glutPostRedisplay();
//}
//
//void InitBuffer()
//{
//	glGenVertexArrays(1, &vao); // VAO를 생성하고 할당합니다.
//	glBindVertexArray(vao); // VAO를 바인드합니다.
//	glGenBuffers(2, vbo); // 2개의 VBO를 생성하고 할당합니다.
//
//	// 첫 번째 VBO에 정점 좌표 데이터를 설정합니다.
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * v.size(), v.data(), GL_STATIC_DRAW);
//
//	// 두 번째 VBO에 색상 데이터를 설정합니다.
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * c.size(), c.data(), GL_STATIC_DRAW);
//
//	// 정점 좌표 데이터를 VAO에 바인딩하고 활성화합니다.
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//	glEnableVertexAttribArray(0);
//
//	// 색상 데이터를 VAO에 바인딩하고 활성화합니다.
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
//	glEnableVertexAttribArray(1);
//}
//
//
//
//void make_shaderProgram()
//{
//	make_vertexShaders(); //--- 버텍스 세이더 만들기
//	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
//	//-- shader Program
//	shaderProgramID = glCreateProgram();
//	glAttachShader(shaderProgramID, vertexShader);
//	glAttachShader(shaderProgramID, fragmentShader);
//	glLinkProgram(shaderProgramID);
//	//--- 세이더 삭제하기
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);
//	//--- Shader Program 사용하기
//	glUseProgram(shaderProgramID);
//}
//
//void make_vertexShaders()
//{
//	vertexSource = filetobuf("vertex.glsl");
//	//--- 버텍스 세이더 객체 만들기
//	vertexShader = glCreateShader(GL_VERTEX_SHADER);
//	//--- 세이더 코드를 세이더 객체에 넣기
//	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
//	//--- 버텍스 세이더 컴파일하기
//	glCompileShader(vertexShader);
//	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
//	GLint result;
//	GLchar errorLog[512];
//	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
//	if (!result)
//	{
//		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
//		std::cout << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
//		return;
//	}
//}
//
//void make_fragmentShaders()
//{
//	fragmentSource = filetobuf("fragment.glsl");
//	//--- 프래그먼트 세이더 객체 만들기
//	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//	//--- 세이더 코드를 세이더 객체에 넣기
//	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
//	//--- 프래그먼트 세이더 컴파일
//	glCompileShader(fragmentShader);
//	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
//	GLint result;
//	GLchar errorLog[512];
//	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
//	if (!result)
//	{
//		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
//		std::cout << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
//		return;
//	}
//}
//
//char* filetobuf(const char* file)
//{
//	FILE* fptr;
//	long length;
//	char* buf;
//	fptr = fopen(file, "rb"); // Open file for reading 
//	if (!fptr) // Return NULL on failure 
//		return NULL;
//	fseek(fptr, 0, SEEK_END); // Seek to the end of the file 
//	length = ftell(fptr); // Find out how many bytes into the file we are 
//	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator 
//	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file 
//	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer 
//	fclose(fptr); // Close the file 
//	buf[length] = 0; // Null terminator 
//	return buf; // Return the buffer 
//}