//#include "stdafx.h"
//
//random_device rd;
//mt19937 dre(rd());
//uniform_real_distribution<float> uidC{ 0.0f, 1.0f }; // 랜덤 컬러 생성
//uniform_real_distribution<float> uidD{ -1.0f, 1.0f }; // 랜덤 방향 생성
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
//vertex v; // 정점이 저장되어있는 녀석
//vector<POINT> pts;
//vector<pair<float, float>> dirs;
//int counts[4] = { 0 };
//int state = 0;
//int cts[4] = { 0 };
//int radiuss[4];
//float angles[4];
//
//pair<float, float> WintoOpenGL(int x, int y)
//{
//	pair<float, float> a = { x / 400. - 1, 1 - (y / 400.) };
//	return a;
//}
//
//void makeObject(int x, int y, int size)
//{
//	pts.push_back({ x - size, y + size });
//	pts.push_back({ x + size, y + size });
//	pts.push_back({ x, y - size * 2 });
//
//	color c = { uidC(dre), uidC(dre), uidC(dre) };
//
//	v.c.push_back(c);
//	v.c.push_back(c);
//	v.c.push_back(c);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * v.pt.size(), v.pt.data(), GL_DYNAMIC_DRAW);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(color) * v.c.size(), v.c.data(), GL_DYNAMIC_DRAW);
//}
//
//void RESET()
//{
//	makeObject(200, 200, 20);
//	makeObject(600, 200, 20);
//	makeObject(200, 600, 20);
//	makeObject(600, 600, 20);
//}
//
//void update()
//{
//	v.pt.clear();
//
//	for (int i = 0; i < pts.size(); ++i)
//	{
//		vec3 tmp = { WintoOpenGL(pts[i].x, pts[i].y).first, WintoOpenGL(pts[i].x, pts[i].y).second, 0.0f};
//		v.pt.push_back(tmp);
//	}
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)* v.pt.size(), v.pt.data(), GL_DYNAMIC_DRAW);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(color)* v.c.size(), v.c.data(), GL_DYNAMIC_DRAW);
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
//	RESET();
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
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
//
//	for (int i = 0; i < 4; ++i)
//		glDrawArrays(GL_TRIANGLES, i * 3, 3);
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
//	case '1': // 튕기기
//		if (dirs.size() == 0)
//			for (int i = 0; i < 4; ++i)
//				dirs.push_back({ uidD(dre), uidD(dre) });
//		state = 1;
//		break;
//
//	case '2': // 좌우로 지그재그
//		if (dirs.size() == 0)
//			for (int i = 0; i < 4; ++i)
//				dirs.push_back({ uidD(dre), uidD(dre) });
//		state = 2;
//		break;
//		
//	case '3': // 사각 스파이럴
//		dirs.clear();
//		for (int i = 0; i < 4; ++i)
//			cts[i] = 0;
//		state = 3;
//		break;
//		
//	case '4': // 원 스파이럴
//		dirs.clear();
//		for (int i = 0; i < 4; ++i)
//		{
//			angles[i] = (atan2(pts[i * 3].y - 400, pts[i * 3].x - 400)) * 180 / 3.14f;
//			radiuss[i] = sqrt(pow(pts[i * 3].x - 400, 2) + pow(pts[i * 3].y - 400, 2));
//		}
//
//		state = 4;
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
//	switch (state)
//	{
//	case 1:
//	{
//		for (int i = 0; i < 4; ++i)
//		{
//			pts[i*3].x += dirs[i].first * 15;
//			pts[i*3].y += dirs[i].second * 15;
//			pts[i*3+1].x += dirs[i].first * 15;
//			pts[i*3+1].y += dirs[i].second * 15;
//			pts[i*3+2].x += dirs[i].first * 15;
//			pts[i*3+2].y += dirs[i].second * 15;
//		}
//
//		// 충돌 체크
//		for (int i = 0; i < 4; ++i)
//		{
//			bool width = false;
//			bool height = false;
//			// 위 아래랑 체크
//			if (pts[i * 3].y < 0) // 1번이 위로 나갔음
//			{
//				pts[i * 3].y = 0;
//				pts[i * 3 + 1].y = 0;
//				pts[i * 3 + 1].x = pts[i * 3].x - 40;
//				pts[i * 3 + 2].x = pts[i * 3].x - 20;
//				pts[i * 3 + 2].y = pts[i * 3].y + 60;
//				height = true;
//			}
//			else if (pts[i * 3].y > 800) // 1번이 밑으로 나갔음
//			{
//				pts[i * 3].y = 800;
//				pts[i * 3 + 1].y = 800;
//				pts[i * 3 + 1].x = pts[i * 3].x + 40;
//				pts[i * 3 + 2].x = pts[i * 3].x + 20;
//				pts[i * 3 + 2].y = pts[i * 3].y - 60;
//				height = true;
//			}
//			else if (pts[i * 3 + 1].y < 0) // 2번이 위로 나갔음
//			{
//				pts[i * 3 + 1].y = 0;
//				pts[i * 3].y = 0;
//				pts[i * 3].x = pts[i * 3 + 1].x + 40;
//				pts[i * 3 + 2].x = pts[i * 3].x - 20;
//				pts[i * 3 + 2].y = pts[i * 3].y + 60;
//				height = true;
//			}
//			else if (pts[i * 3 + 1].y > 800) // 2번이 밑으로 나갔음
//			{
//				pts[i * 3 + 1].y = 800;
//				pts[i * 3].y = 800;
//				pts[i * 3].x = pts[i * 3 + 1].x - 40;
//				pts[i * 3 + 2].x = pts[i * 3].x + 20;
//				pts[i * 3 + 2].y = pts[i * 3].y - 60;
//				height = true;
//			}
//			else if (pts[i * 3 + 2].y < 0) // 3번이 위로
//			{
//				pts[i * 3].y = 0;
//				pts[i * 3 + 1].y = 0;
//				pts[i * 3 + 2].y = pts[i * 3].y + 60;
//				height = true;
//			}
//			else if (pts[i * 3 + 2].y > 800) // 3번이 밑으로
//			{
//				pts[i * 3].y = 800;
//				pts[i * 3 + 1].y = 800;
//				pts[i * 3 + 2].y = pts[i * 3].y - 60;
//				height = true;
//			}
//
//			// 좌우충돌
//			if (pts[i * 3].x < 0) // 1번이 왼쪽으로 나갔음
//			{
//				pts[i * 3].x = 0;
//				pts[i * 3 + 1].x = 0;
//				pts[i * 3 + 1].y = pts[i * 3].y + 40;
//				pts[i * 3 + 2].x = pts[i * 3].x + 60;
//				pts[i * 3 + 2].y = pts[i * 3].y + 20;
//				width = true;
//			}
//			else if (pts[i * 3].x > 800) // 1번이 오른쪽으로 나갔음
//			{
//				pts[i * 3].x = 800;
//				pts[i * 3 + 1].x = 800;
//				pts[i * 3 + 1].y = pts[i * 3].y - 40;
//				pts[i * 3 + 2].x = pts[i * 3].x - 60;
//				pts[i * 3 + 2].y = pts[i * 3].y - 20;
//				width = true;
//			}
//			else if (pts[i * 3 + 1].x < 0) // 2번이 왼쪽으로 나갔음
//			{
//				pts[i * 3 + 1].x = 0;
//				pts[i * 3].x = 0;
//				pts[i * 3].y = pts[i * 3 + 1].y - 40;
//				pts[i * 3 + 2].x = pts[i * 3].x + 60;
//				pts[i * 3 + 2].y = pts[i * 3].y + 20;
//				width = true;
//			}
//			else if (pts[i * 3 + 1].x > 800) // 2번이 오른쪽으로 나갔음
//			{
//				pts[i * 3 + 1].x = 800;
//				pts[i * 3].x = 800;
//				pts[i * 3].y = pts[i * 3 + 1].y + 40;
//				pts[i * 3 + 2].x = pts[i * 3].x - 60;
//				pts[i * 3 + 2].y = pts[i * 3].y - 20;
//				width = true;
//			}
//			else if (pts[i * 3 + 2].x < 0) // 3번이 왼쪽
//			{
//				pts[i * 3].x = 0;
//				pts[i * 3 + 1].x = 0;
//				pts[i * 3 + 2].x = pts[i * 3].x + 60;
//				width = true;
//			}
//			else if (pts[i * 3 + 2].x > 800) // 3번이 오른쪽
//			{
//				pts[i * 3].x = 800;
//				pts[i * 3 + 1].x = 800;
//				pts[i * 3 + 2].x = pts[i * 3].x - 60;
//				width = true;
//			}
//
//			if (width)
//				dirs[i].first *= -1;
//			if (height)
//				dirs[i].second *= -1;
//		}
//		break;
//	}
//	case 2: // 지그재그
//	{
//		for (int i = 0; i < 4; ++i)
//		{
//			if (counts[i] == 0)
//			{
//				pts[i * 3].x += dirs[i].first * 15;
//				pts[i * 3 + 1].x += dirs[i].first * 15;
//				pts[i * 3 + 2].x += dirs[i].first * 15;
//			}
//			else
//			{
//				pts[i * 3].y += dirs[i].second * 15;
//				pts[i * 3 + 1].y += dirs[i].second * 15;
//				pts[i * 3 + 2].y += dirs[i].second * 15;
//				counts[i]++;
//			}
//		}
//
//		for (int i = 0; i < 4; ++i)
//		{
//			if (counts[i] == 5) 
//				counts[i] = 0;
//			if (pts[i * 3].x < 0 || pts[i * 3 + 1].x < 0 || pts[i * 3 + 2].x < 0) // 먼가 왼쪽으로 나가면
//			{
//				pts[i * 3].x = 0;
//				pts[i * 3 + 1].x = 0;
//				pts[i * 3 + 2].x = 60;
//				pts[i * 3 + 1].y = pts[i * 3].y + 40;
//				pts[i * 3 + 2].y = pts[i * 3].y + 20;
//				dirs[i].first *= -1;
//				counts[i]++;
//			}
//			else if (pts[i * 3].x > 800 || pts[i * 3 + 1].x > 800 || pts[i * 3 + 2].x > 800) // 먼가 왼쪽으로 나가면
//			{
//				pts[i * 3].x = 800;
//				pts[i * 3 + 1].x = 800;
//				pts[i * 3 + 2].x = 800 - 60;
//				pts[i * 3 + 1].y = pts[i * 3].y + 40;
//				pts[i * 3 + 2].y = pts[i * 3].y + 20;
//				dirs[i].first *= -1;
//				counts[i]++;
//			}
//
//			if (pts[i * 3].y > 800 || pts[i * 3 + 1].y > 800 || pts[i * 3 + 2].y > 800
//				|| pts[i * 3].y < 0 || pts[i * 3 + 1].y < 0 || pts[i * 3 + 2].y < 0)
//				dirs[i].second *= -1;
//
//		}
//		break;
//	}
//	case 3: // 사각 스파이럴
//	{
//		for (int i = 0; i < 4; ++i)
//		{
//			if (cts[i] == 0)
//			{
//				pts[i * 3].x -= 10;
//				pts[i * 3 + 1].x -= 10;
//				pts[i * 3 + 2].x -= 10;
//				if (pts[i * 3].x < pts[i * 3].y)
//					cts[i]++;
//			}
//			else if (cts[i] == 1)
//			{
//				pts[i * 3].y += 10;
//				pts[i * 3 + 1].y += 10;
//				pts[i * 3 + 2].y += 10;
//				if (pts[i * 3].y > 800 - pts[i * 3].x)
//					cts[i]++;
//			}
//			else if (cts[i] == 2)
//			{
//				pts[i * 3].x += 10;
//				pts[i * 3 + 1].x += 10;
//				pts[i * 3 + 2].x += 10;
//				if (pts[i * 3].x > pts[i * 3].y)
//					cts[i]++;
//			}
//			else if (cts[i] == 3)
//			{
//				pts[i * 3].y -= 10;
//				pts[i * 3 + 1].y -= 10;
//				pts[i * 3 + 2].y -= 10;
//				if (pts[i * 3].y < (800-pts[i * 3].x) * 2)
//					cts[i] = 0;
//			}
//		}
//
//		break;
//	}
//	case 4:
//	{
//		for (int i = 0; i < 4; ++i)
//		{
//			angles[i] -= 10;
//			radiuss[i] -= 1;
//
//			pts[i * 3] = { int(400 + radiuss[i] * cos(angles[i] * 3.14 / 180.)) , int(400 + radiuss[i] * sin(angles[i] * 3.14 / 180.)) };
//			pts[i * 3 + 1] = { pts[i * 3].x + 40, pts[i * 3].y};
//			pts[i * 3 + 2] = { pts[i * 3].x + 20, pts[i * 3].y - 60 };
//		}
//
//		break;
//	}
//	default:
//		break;
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