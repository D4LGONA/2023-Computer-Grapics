//#include "stdafx.h"
//
//random_device rd;
//mt19937 dre(rd());
//uniform_real_distribution<float> uidC{ 0.0f, 1.0f }; // 랜덤 컬러 생성
//uniform_real_distribution<float> uidD{ -1.0f, 1.0f }; // 랜덤 방향 생성
//uniform_int_distribution<int> uid{ 0,800 };
//
//GLuint vao, vbo[2];
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
//color c;
//vertex v; // 정점이 저장되어있는 녀석
//vector<POINT> pts;
//vector<POINT> pt;
//vector<POINT> movept;
//float angle = 0.0f;
//float Radius = 0.0f;
//bool start = false;
//bool isbigger = true;
//bool isline = true;
//int n = 1;
//int dotcount = 0;
//vector<int> insertcount;
//
//pair<float, float> WintoOpenGL(int x, int y)
//{
//	pair<float, float> a = { x / 400. - 1, 1 - (y / 400.) };
//	return a;
//}
//
//void update()
//{
//	v.pt.clear();
//	v.c.clear();
//	
//	for (int i = 0; i < pts.size(); ++i)
//	{
//		vec3 tmp = { WintoOpenGL(pts[i].x, pts[i].y).first, WintoOpenGL(pts[i].x, pts[i].y).second, 0.0f };
//		v.pt.push_back(tmp);
//		v.c.push_back({ 1.0f, 1.0f, 1.0f });
//	}
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * v.pt.size(), v.pt.data(), GL_DYNAMIC_DRAW);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(color) * v.c.size(), v.c.data(), GL_DYNAMIC_DRAW);
//}
//
//void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
//{
//	//--- 윈도우 생성하기
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
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
//	glutTimerFunc(100, TimerFunction, 1);
//	glutMainLoop();
//}
//
//GLvoid drawScene()
//{
//	glClearColor(c.r, c.g, c.b, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glUseProgram(shaderProgramID);
//	glBindVertexArray(vao);
//
//	// Location 번호 저장
//	int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position"); //	: 0  Shader의 'layout (location = 0)' 부분
//	int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color"); //	: 1
//
//	glEnableVertexAttribArray(PosLocation); // Enable 필수! 사용하겠단 의미
//	glEnableVertexAttribArray(ColorLocation);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // 정점 좌표용 VBO 바인딩
//	glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // 색상 데이터용 VBO 바인딩
//	glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(color), 0);
//
//	for (int i = 0; i < pts.size(); ++i)
//	{
//		if (isline)
//		{
//			if (i == pts.size() - 1 || i == (pts.size() / pt.size()) - 1 || i == (pts.size() * 2 / pt.size()) - 1 || i == (pts.size() * 3 / pt.size()) - 1 || i == (pts.size() * 4 / pt.size()) - 1)
//				continue;
//			glLineWidth(2.0f);
//			glDrawArrays(GL_LINES, i, 2);
//		}
//		else
//		{
//			glPointSize(2.0f);
//			glDrawArrays(GL_POINTS, i, 1);
//		}
//	}
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
//	case 'p':
//		isline = false;
//		break;
//
//	case 'l':
//		isline = true;
//		break;
//
//	case '1':
//		dotcount = 0;
//		Radius = 0.0f;
//		angle = 0.0f;
//		pt.clear();
//		movept.clear();
//		pts.clear();
//		insertcount.clear();
//		isbigger = true;
//		pt.push_back({ uid(dre), uid(dre) });
//		start = true;
//		insertcount.push_back(0);
//		break;
//	case '2':
//		dotcount = 0;
//		Radius = 0.0f;
//		angle = 0.0f;
//		pt.clear();
//		movept.clear();
//		pts.clear();
//		insertcount.clear();
//		isbigger = true;
//		pt.push_back({ uid(dre), uid(dre) });
//		pt.push_back({ uid(dre), uid(dre) });
//		start = true;
//		insertcount.push_back(0);
//		insertcount.push_back(1);
//		break;
//	case '3':
//		dotcount = 0;
//		Radius = 0.0f;
//		angle = 0.0f;
//		pt.clear();
//		movept.clear();
//		pts.clear();
//		insertcount.clear();
//		isbigger = true;
//		pt.push_back({ uid(dre), uid(dre) });
//		pt.push_back({ uid(dre), uid(dre) });
//		pt.push_back({ uid(dre), uid(dre) });
//		start = true;
//		insertcount.push_back(0);
//		insertcount.push_back(1);
//		insertcount.push_back(2);
//		break;
//	case '4':
//		dotcount = 0;
//		Radius = 0.0f;
//		angle = 0.0f;
//		pt.clear();
//		movept.clear();
//		pts.clear();
//		insertcount.clear();
//		isbigger = true;
//		pt.push_back({ uid(dre), uid(dre) });
//		pt.push_back({ uid(dre), uid(dre) });
//		pt.push_back({ uid(dre), uid(dre) });
//		pt.push_back({ uid(dre), uid(dre) });
//		start = true;
//		insertcount.push_back(0);
//		insertcount.push_back(1);
//		insertcount.push_back(2);
//		insertcount.push_back(3);
//		break;
//	case '5':
//		dotcount = 0;
//		Radius = 0.0f;
//		angle = 0.0f;
//		pt.clear();
//		movept.clear();
//		pts.clear();
//		insertcount.clear();
//		isbigger = true;
//		pt.push_back({ uid(dre), uid(dre) });
//		pt.push_back({ uid(dre), uid(dre) });
//		pt.push_back({ uid(dre), uid(dre) });
//		pt.push_back({ uid(dre), uid(dre) });
//		pt.push_back({ uid(dre), uid(dre) });
//		start = true;
//		insertcount.push_back(0);
//		insertcount.push_back(1);
//		insertcount.push_back(2);
//		insertcount.push_back(3);
//		insertcount.push_back(4);
//		break;
//	}
//	glutPostRedisplay();
//}
//
//GLvoid TimerFunction(int value)
//{
//	if (start)
//	{
//		if (isbigger)
//		{
//			angle += 10;
//			Radius += 0.5f;
//			for (int i = 0; i < pt.size(); ++i) 
//			{
//				pts.insert(pts.begin() + insertcount[i], {int(pt[i].x + Radius * cos(angle * 3.14f / 180)), int(pt[i].y + Radius * sin(angle * 3.14f / 180))});
//				insertcount[i] += (i + 1);
//			}
//			if (angle > 360 * 3)
//			{
//				isbigger = false;
//				angle += 180;
//				for (int i = 0; i < pt.size(); ++i)
//					movept.push_back({ int(pt[i].x + Radius * 2), pt[i].y });
//			}
//		}
//		else
//		{
//			angle -= 10;
//			Radius -= 0.5f;
//			for (int i = 0; i < pt.size(); ++i)
//			{
//				pts.insert(pts.begin() + insertcount[i], { int(movept[i].x + Radius * cos(angle * 3.14f / 180)), int(movept[i].y + Radius * sin(angle * 3.14f / 180)) });
//				insertcount[i] += (i + 1);
//			}
//			if (angle < 0 || Radius < 0)
//				start = false;
//		}
//	}
//
//	update();
//
//	glutPostRedisplay();
//	glutTimerFunc(100, TimerFunction, 1);
//}
//
//GLvoid Mouse(int button, int state, int x, int y)
//{
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
//	{
//		c = { uidC(dre), uidC(dre), uidC(dre) };
//		Radius = 0.0f;
//		angle = 0.0f;
//		start = true;
//		pt.clear();
//		movept.clear();
//		pts.clear();
//		insertcount.clear();
//		isbigger = true;
//		pt.push_back({ x, y });
//		insertcount.push_back(0);
//	}
//
//	return GLvoid();
//}
//
//GLvoid Motion(int x, int y)
//{
//	glutPostRedisplay();
//}
//
//void InitBuffer()
//{
//	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
//	glBindVertexArray(vao); //--- VAO를 바인드하기
//	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기
//}
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