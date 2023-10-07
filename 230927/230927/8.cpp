//#include "stdafx.h"
//
//random_device rd;
//mt19937 dre(rd());
//uniform_real_distribution<float> uidC{ 0.0f, 1.0f }; // 랜덤 컬러 생성
//uniform_int_distribution<int> uidS{ 20, 100 };
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
//enum types
//{
//	dot = 0,
//	line = 1,
//	triangle = 2,
//	rectangle = 3
//};
//
//struct shape
//{
//	int type = 0;
//	int idx = 0;
//};
//
//vertex v;
//vector<shape> obj;
//int state = triangle;
//bool isFill = true;
//int scale = 20;
//
//pair<float, float> WintoOpenGL(int x, int y)
//{
//	pair<float, float> a = { x / 400. - 1, 1 - (y / 400.) };
//	return a;
//}
//
//void makeObject(int x, int y, int size) // 오픈지엘 좌표로 변환해서 받아오기
//{
//	switch (state)
//	{
//	case dot:
//	{
//		shape s{ dot, v.pt.size() };
//		obj.push_back(s);
//
//		vec3 pt1 = { WintoOpenGL(x, y).first, WintoOpenGL(x, y).second, 0.0f };
//		color c = { uidC(dre), uidC(dre), uidC(dre) };
//
//		v.pt.push_back(pt1);
//
//		v.c.push_back(c);
//		break;
//	}
//	case line: // 여기선 테두리 그리는 녀석으로 만들어버렸음
//	{
//		shape s{ line, v.pt.size() };
//		obj.push_back(s);
//
//		vec3 pt1 = { WintoOpenGL(400, 0).first, WintoOpenGL(400, 0).second, 0.0f };
//		vec3 pt2 = { WintoOpenGL(400, 800).first, WintoOpenGL(400, 800).second, 0.0f };
//		color c = { 1.0f, 1.0f, 1.0f };
//
//		v.pt.push_back(pt1);
//		v.pt.push_back(pt2);
//
//		v.c.push_back(c);
//		v.c.push_back(c);
//		///
//
//		s = { line, int(v.pt.size()) };
//		obj.push_back(s);
//
//		pt1 = { WintoOpenGL(0, 400).first, WintoOpenGL(0, 400).second, 0.0f };
//		pt2 = { WintoOpenGL(800, 400).first, WintoOpenGL(800, 400).second, 0.0f };
//
//		v.pt.push_back(pt1);
//		v.pt.push_back(pt2);
//
//		v.c.push_back(c);
//		v.c.push_back(c);
//		break;
//	}
//	case triangle:
//	{
//		shape s{ triangle, v.pt.size() };
//		obj.push_back(s);
//
//		vec3 pt1 = { WintoOpenGL(x - size, y + size).first, WintoOpenGL(x - size, y + size).second };
//		vec3 pt2 = { WintoOpenGL(x + size, y + size).first, WintoOpenGL(x + size, y + size).second };
//		vec3 pt3 = { WintoOpenGL(x, y - size * 2).first, WintoOpenGL(x, y - size * 2).second };
//		color c = { uidC(dre), uidC(dre), uidC(dre) };
//
//		v.pt.push_back(pt1);
//		v.pt.push_back(pt2);
//		v.pt.push_back(pt3);
//		v.pt.push_back(pt1);
//		v.pt.push_back(pt2);
//		v.pt.push_back(pt3);
//
//		v.c.push_back(c);
//		v.c.push_back(c);
//		v.c.push_back(c);
//		v.c.push_back(c);
//		v.c.push_back(c);
//		v.c.push_back(c);
//		break;
//	}
//	case rectangle:
//	{
//		shape s{ rectangle, v.pt.size() };
//		obj.push_back(s);
//
//		vec3 pt1 = { WintoOpenGL(x - 20, y + 20).first, WintoOpenGL(x - 20, y + 20).second, 0.0f };
//		vec3 pt2 = { WintoOpenGL(x + 20, y + 20).first, WintoOpenGL(x + 20, y + 20).second, 0.0f };
//		vec3 pt4 = { WintoOpenGL(x - 20, y - 20).first, WintoOpenGL(x - 20, y - 20).second, 0.0f };
//		vec3 pt3 = { WintoOpenGL(x + 20, y - 20).first, WintoOpenGL(x + 20, y - 20).second, 0.0f };
//		color c = { uidC(dre), uidC(dre), uidC(dre) };
//
//		v.pt.push_back(pt1);
//		v.pt.push_back(pt2);
//		v.pt.push_back(pt4);
//		v.pt.push_back(pt4);
//		v.pt.push_back(pt2);
//		v.pt.push_back(pt3);
//
//		v.c.push_back(c);
//		v.c.push_back(c);
//		v.c.push_back(c);
//		v.c.push_back(c);
//		v.c.push_back(c);
//		v.c.push_back(c);
//
//		break;
//	}
//	}
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * v.pt.size(), v.pt.data(), GL_DYNAMIC_DRAW);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(color) * v.c.size(), v.c.data(), GL_DYNAMIC_DRAW);
//}
//
//void RESET()
//{
//	makeObject(200, 200, scale); // 1사분면
//	makeObject(600, 200, scale); // 2사분면
//	makeObject(200, 600, scale); // 3사분면
//	makeObject(600, 600, scale); // 4사분면
//
//	state = line;
//
//	makeObject(0,0,0); // 선 그리려고 그냥 한거임
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
//
//	glutMainLoop();
//}
//
//
//GLvoid drawScene()
//{
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glUseProgram(shaderProgramID);
//	glBindVertexArray(vao);
//
//	 //Location 번호 저장
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
//	for (int i = 0; i < obj.size(); ++i)
//	{
//		switch (obj[i].type)
//		{
//		case dot:
//			glPointSize(5.0f);
//			glDrawArrays(GL_POINTS, obj[i].idx, 1);
//			break;
//		case line:
//			glLineWidth(5.0f); // 라인 두께를 5.0f로 설정
//			glDrawArrays(GL_LINES, obj[i].idx, 2);
//			break;
//		case triangle:
//			if(isFill)
//				glDrawArrays(GL_TRIANGLES, obj[i].idx, 3);
//			else
//			{
//				glDrawArrays(GL_LINES, obj[i].idx, 2);
//				glDrawArrays(GL_LINES, obj[i].idx + 2, 2);
//				glDrawArrays(GL_LINES, obj[i].idx + 4, 2);
//			}
//			break;
//		case rectangle:
//			glDrawArrays(GL_TRIANGLES, obj[i].idx, 3);
//			glDrawArrays(GL_TRIANGLES, obj[i].idx + 3, 3);
//			break;
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
//	case 'a': // 면으로 그린다
//		isFill = true;
//		break;
//
//	case 'A': // 선으로 그린다
//		isFill = false;
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
//	glutPostRedisplay();
//	glutTimerFunc(100, TimerFunction, 1);
//}
//
//GLvoid Mouse(int button, int state, int x, int y)
//{
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
//	{
//		if (x < 400 && y < 400) // 1사분면
//		{
//			scale = uidS(dre);
//			v.pt[0] = { WintoOpenGL(x - scale, y + scale).first, WintoOpenGL(x - scale, y + scale).second };
//			v.pt[1] = { WintoOpenGL(x + scale, y + scale).first, WintoOpenGL(x + scale, y + scale).second };
//			v.pt[2] = { WintoOpenGL(x, y - scale * 2).first, WintoOpenGL(x, y - scale * 2).second };
//			v.pt[3] = { WintoOpenGL(x - scale, y + scale).first, WintoOpenGL(x - scale, y + scale).second };
//			v.pt[4] = { WintoOpenGL(x + scale, y + scale).first, WintoOpenGL(x + scale, y + scale).second };
//			v.pt[5] = { WintoOpenGL(x, y - scale * 2).first, WintoOpenGL(x, y - scale * 2).second };
//		}
//		else if (x > 400 && y < 400) // 2사분면
//		{
//			scale = uidS(dre);
//			v.pt[6] = { WintoOpenGL(x - scale, y + scale).first, WintoOpenGL(x - scale, y + scale).second };
//			v.pt[7] = { WintoOpenGL(x + scale, y + scale).first, WintoOpenGL(x + scale, y + scale).second };
//			v.pt[8] = { WintoOpenGL(x, y - scale * 2).first, WintoOpenGL(x, y - scale * 2).second };
//			v.pt[9] = { WintoOpenGL(x - scale, y + scale).first, WintoOpenGL(x - scale, y + scale).second };
//			v.pt[10] = { WintoOpenGL(x + scale, y + scale).first, WintoOpenGL(x + scale, y + scale).second };
//			v.pt[11] = { WintoOpenGL(x, y - scale * 2).first, WintoOpenGL(x, y - scale * 2).second };
//		}
//		else if (x < 400 && y < 800) // 3사분면
//		{
//			scale = uidS(dre);
//			v.pt[12] = { WintoOpenGL(x - scale, y + scale).first, WintoOpenGL(x - scale, y + scale).second };
//			v.pt[13] = { WintoOpenGL(x + scale, y + scale).first, WintoOpenGL(x + scale, y + scale).second };
//			v.pt[14] = { WintoOpenGL(x, y - scale * 2).first, WintoOpenGL(x, y - scale * 2).second };
//			v.pt[15] = { WintoOpenGL(x - scale, y + scale).first, WintoOpenGL(x - scale, y + scale).second };
//			v.pt[16] = { WintoOpenGL(x + scale, y + scale).first, WintoOpenGL(x + scale, y + scale).second };
//			v.pt[17] = { WintoOpenGL(x, y - scale * 2).first, WintoOpenGL(x, y - scale * 2).second };
//		}
//		else
//		{
//			scale = uidS(dre);
//			v.pt[18] = { WintoOpenGL(x - scale, y + scale).first, WintoOpenGL(x - scale, y + scale).second };
//			v.pt[19] = { WintoOpenGL(x + scale, y + scale).first, WintoOpenGL(x + scale, y + scale).second };
//			v.pt[20] = { WintoOpenGL(x, y - scale * 2).first, WintoOpenGL(x, y - scale * 2).second };
//			v.pt[21] = { WintoOpenGL(x - scale, y + scale).first, WintoOpenGL(x - scale, y + scale).second };
//			v.pt[22] = { WintoOpenGL(x + scale, y + scale).first, WintoOpenGL(x + scale, y + scale).second };
//			v.pt[23] = { WintoOpenGL(x, y - scale * 2).first, WintoOpenGL(x, y - scale * 2).second };
//		}
//
//		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * v.pt.size(), v.pt.data(), GL_DYNAMIC_DRAW);
//		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(color) * v.c.size(), v.c.data(), GL_DYNAMIC_DRAW);
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