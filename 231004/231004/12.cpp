//#include "stdafx.h"
//
//random_device rd;
//mt19937 dre(rd());
//uniform_real_distribution<float> uidC{ 0.0f, 1.0f }; // 랜덤 컬러 생성
//uniform_real_distribution<float> uidD{ -1.0f, 1.0f }; // 랜덤 방향 / 속도
//uniform_int_distribution<int> uid{ 40, 760 };
//uniform_int_distribution<int> uidA{ 1,5 };
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
//void makeObject(int, int, int);
//
//struct shape
//{
//	int angleCnt = 0;
//	int idx = 0;
//	pair<float, float> dir = { uidD(dre), uidD(dre) };
//};
//
//vertex v; // vbo에 연결하는 애
//vector<pair<POINT,color>> CENTER; // 가운데 점
//vector<POINT> pts; // winapi 좌표계로 된 녀석
//vector<shape> obj; // 어떤 녀석인지 알아보는 친구
//POINT mousept;
//int selected = -1;
//bool isDrag = false;
//
//pair<float, float> WintoOpenGL(int x, int y)
//{
//	pair<float, float> a = { x / 400. - 1, 1 - (y / 400.) };
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
//	v.pt.clear();
//	v.c.clear();
//
//	for (int i = 0; i < pts.size(); ++i)
//	{
//		vec3 tmp = { WintoOpenGL(pts[i].x, pts[i].y).first, WintoOpenGL(pts[i].x, pts[i].y).second, 0.0f };
//		v.pt.push_back(tmp);
//	}
//	
//	for (int i = 0; i < obj.size(); ++i)
//	{
//		for (int j = 0; j < obj[i].angleCnt; ++j)
//			v.c.push_back(CENTER[i].second);
//	}
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * v.pt.size(), v.pt.data(), GL_DYNAMIC_DRAW);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(color) * v.c.size(), v.c.data(), GL_DYNAMIC_DRAW);
//}
//
//void deleteVertex(int idx, int i)
//{
//	// 지워야 하는 것: 합쳐지는 두 놈 데이터(pts) 둘다랑 center에 있는 
//	// pt에 있는 녀석들중 idx인 부분부터 anglecnt 더한 위치까지
// 	v.pt.erase(v.pt.begin() + idx, v.pt.begin() + idx + obj[i].angleCnt);
//	v.c.erase(v.c.begin() + idx, v.c.begin() + idx + obj[i].angleCnt);
//	//CENTER.erase()
//}
//
//void makePolyObject(int x, int y, int sides, int idx)
//{
//	for (int i = 0; i < sides; ++i)
//	{
//		GLfloat angle = 2.0 * 3.14f * i / sides;
//		int ax = x + 20 * cos(angle);
//		int ay = y + 20 * sin(angle);
//		pts.insert(pts.begin() + idx, {ax, ay});
//	}
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
//	for (int i = 0; i < 15; ++i)
//	{
//		makeObject(uid(dre), uid(dre), uidA(dre));
//	}
//
//	glutMainLoop();
//}
//
//void makeObject(int x, int y, int state) 
//{
//	switch (state)
//	{
//	case 1:
//	{
//		shape s{ 1, pts.size() };
//		obj.push_back(s);
//
//		color c = { uidC(dre), uidC(dre), uidC(dre) };
//		pts.push_back({ x, y });
//		CENTER.push_back({ {x, y}, c });
//		break;
//	}
//	case 2:
//	{
//		shape s{ 2, pts.size() };
//		obj.push_back(s);
//
//		color c = { uidC(dre), uidC(dre), uidC(dre) };
//		makePolyObject(x, y, 2, obj.back().idx);
//		CENTER.push_back({ {x, y}, c });
//		break;
//	}
//	case 3: 
//	{
//		shape s{ 3, pts.size() };
//		obj.push_back(s);
//
//		color c = { uidC(dre), uidC(dre), uidC(dre) };
//		makePolyObject(x, y, 3, obj.back().idx);
//		CENTER.push_back({ { x,y }, c });
//		break;
//	}
//	case 4:
//	{
//		shape s{ 4, pts.size() };
//		obj.push_back(s);
//
//		color c = { uidC(dre), uidC(dre), uidC(dre) };
//		makePolyObject(x, y, 4, obj.back().idx);
//		CENTER.push_back({ { x,y }, c });
//		break;
//	}
//	case 5:
//	{
//		shape s{ 5, pts.size() };
//		obj.push_back(s);
//
//		color c = { uidC(dre), uidC(dre), uidC(dre) };
//		makePolyObject(x, y, 5, obj.back().idx);
//		CENTER.push_back({ { x,y }, c });
//		break;
//	}
//	}
//	
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * v.pt.size(), v.pt.data(), GL_DYNAMIC_DRAW);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(color) * v.c.size(), v.c.data(), GL_DYNAMIC_DRAW);
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
//		if (obj[i].angleCnt == 1)
//		{
//			glPointSize(4.0f);
//			glDrawArrays(GL_POINTS, obj[i].idx, obj[i].angleCnt);
//		}
//		else if (obj[i].angleCnt == 2)
//		{
//			glLineWidth(4.0f);
//			glDrawArrays(GL_LINES, obj[i].idx, obj[i].angleCnt);
//		}
//		else
//			glDrawArrays(GL_POLYGON, obj[i].idx, obj[i].angleCnt);
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
//	case 'c':
//		v.pt.clear();
//		v.c.clear();
//
//		obj.clear();
//		break;
//
//	case 'q': // 프로그램 종료
//		exit(0);
//		break;
//	}
//	update();
//	glutPostRedisplay();
//}
//
//GLvoid TimerFunction(int value)
//{
//	for (int i = 0; i < obj.size(); ++i)
//	{
//		if (i == selected) continue;
//		bool ischangedX = false;
//		bool ischangedY = false;
//		CENTER[i].first.x += obj[i].dir.first * 10;
//		CENTER[i].first.y += obj[i].dir.second * 10;
//		for (int j = obj[i].idx; j < obj[i].idx + obj[i].angleCnt; ++j)
//		{
//			pts[j].x += obj[i].dir.first * 10;
//			pts[j].y += obj[i].dir.second * 10;
//		}
//		for (int j = obj[i].idx; j < obj[i].idx + obj[i].angleCnt; ++j)
//		{
//			if (!ischangedX && (pts[j].x - 15 <= 0 || pts[j].x + 15 >= 800))
//			{
//				obj[i].dir.first *= -1;
//				ischangedX = true;
//			}
//			if (!ischangedY && (pts[j].y - 15 <= 0 || pts[j].y + 15 >= 800))
//			{
//				obj[i].dir.second *= -1;
//				ischangedY = true;
//			}
//		}
//	}
//	
//	cout << selected << endl;
//
//	update();
//	glutPostRedisplay();
//	glutTimerFunc(100, TimerFunction, 1);
//}
//
//GLvoid Mouse(int button, int state, int x, int y)
//{
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
//	{
//		mousept = { x,y };
//		isDrag = true;
//		for (int i = 0; i < obj.size(); ++i)
//		{
//			if (dist(CENTER[i].first.x, CENTER[i].first.y, x, y) < 50)
//			{
//				selected = i;
//				break;
//			}
//		}
//	}
//	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && selected != -1)
//	{
//		for (int i = 0; i < obj.size(); ++i)
//		{
//			if (i == selected) continue;
//			if (dist(CENTER[selected].first.x, CENTER[selected].first.y, CENTER[i].first.x, CENTER[i].first.y) < 20)
//			{
//				int slices = obj[i].angleCnt + obj[selected].angleCnt;
//				POINT tmp = { CENTER[selected].first.x, CENTER[selected].first.y };
//				// pts 건들고
//				if (selected < i)
//				{
//					pts.erase(pts.begin() + obj[selected].idx, pts.begin() + obj[selected].idx + obj[selected].angleCnt);
//					pts.erase(pts.begin() + (obj[i].idx - obj[selected].angleCnt), pts.begin() + (obj[i].idx + obj[i].angleCnt - obj[selected].angleCnt));
//
//					CENTER.erase(CENTER.begin() + selected);
//					CENTER.erase(CENTER.begin() + i - 1);
//
//					for (int j = selected + 1; j < obj.size(); ++j)
//						obj[j].idx -= obj[selected].angleCnt;
//					for (int j = i + 1; j < obj.size(); ++j)
//						obj[j].idx -= obj[i].angleCnt;
//
//					obj.erase(obj.begin() + selected);
//					obj.erase(obj.begin() + i - 1);
//				}
//				else
//				{
//					pts.erase(pts.begin() + obj[i].idx, pts.begin() + obj[i].idx + obj[i].angleCnt);
//					pts.erase(pts.begin() + (obj[selected].idx - obj[i].angleCnt), pts.begin() + (obj[selected].idx + obj[selected].angleCnt - obj[i].angleCnt));
//
//					CENTER.erase(CENTER.begin() + i);
//					CENTER.erase(CENTER.begin() + selected - 1);
//
//					for (int j = selected + 1; j < obj.size(); ++j)
//						obj[j].idx -= obj[selected].angleCnt;
//					for (int j = i + 1; j < obj.size(); ++j)
//						obj[j].idx -= obj[i].angleCnt;
//
//					obj.erase(obj.begin() + i);
//					obj.erase(obj.begin() + selected - 1);
//				}
//
//				// 새 도형 추가
//				shape s{ slices, pts.size() };
//				obj.push_back(s);
//
//				color c = { uidC(dre), uidC(dre), uidC(dre) };
//				makePolyObject(tmp.x, tmp.y, slices, obj.back().idx);
//				CENTER.push_back({ { tmp.x, tmp.y }, c });
//
//				cout << "닿았음" << endl;
//			}
//		}
//
//
//
//		selected = -1;
//		isDrag = false;
//	}
//
//	update();
//	return GLvoid();
//}
//
//GLvoid Motion(int x, int y)
//{
//	if (isDrag && selected != -1)
//	{
//		for (int i = obj[selected].idx; i < obj[selected].idx + obj[selected].angleCnt; ++i)
//		{
//			pts[i].x += x - mousept.x;
//			pts[i].y += y - mousept.y;
//		}
//		CENTER[selected].first.x += x - mousept.x;
//		CENTER[selected].first.y += y - mousept.y;
//
//		mousept = { x, y };
//	}
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
//		//--- 버텍스 세이더 객체 만들기
//		vertexShader = glCreateShader(GL_VERTEX_SHADER);
//	//--- 세이더 코드를 세이더 객체에 넣기
//	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
//	//--- 버텍스 세이더 컴파일하기
//	glCompileShader(vertexShader);
//	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
//	GLint result;
//	GLchar errorLog[512];
//	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
//	if(!result)
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