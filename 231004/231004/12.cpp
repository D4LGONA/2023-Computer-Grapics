//#include "stdafx.h"
//
//random_device rd;
//mt19937 dre(rd());
//uniform_real_distribution<float> uidC{ 0.0f, 1.0f }; // ���� �÷� ����
//uniform_real_distribution<float> uidD{ -1.0f, 1.0f }; // ���� ���� / �ӵ�
//uniform_int_distribution<int> uid{ 40, 760 };
//uniform_int_distribution<int> uidA{ 1,5 };
//
//GLuint vao, vbo[2];
//
//GLchar* vertexSource, * fragmentSource; //--- �ҽ��ڵ� ���� ����
//GLuint vertexShader, fragmentShader; //--- ���̴� ��ü
//GLuint shaderProgramID; //--- ���̴� ���α׷�
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
//vertex v; // vbo�� �����ϴ� ��
//vector<pair<POINT,color>> CENTER; // ��� ��
//vector<POINT> pts; // winapi ��ǥ��� �� �༮
//vector<shape> obj; // � �༮���� �˾ƺ��� ģ��
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
//	// ������ �ϴ� ��: �������� �� �� ������(pts) �Ѵٶ� center�� �ִ� 
//	// pt�� �ִ� �༮���� idx�� �κк��� anglecnt ���� ��ġ����
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
//void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
//{
//	//--- ������ �����ϱ�
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
//	glutInitWindowPosition(100, 100);
//	glutInitWindowSize(800, 800);
//	glutCreateWindow("Example1");
//	//--- GLEW �ʱ�ȭ�ϱ�
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
//	// Location ��ȣ ����
//	int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position"); //	: 0  Shader�� 'layout (location = 0)' �κ�
//	int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color"); //	: 1
//
//	
//	glEnableVertexAttribArray(PosLocation); // Enable �ʼ�! ����ϰڴ� �ǹ�
//	glEnableVertexAttribArray(ColorLocation);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // ���� ��ǥ�� VBO ���ε�
//	glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // ���� �����Ϳ� VBO ���ε�
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
//	glDisableVertexAttribArray(PosLocation); // Disable �ʼ�!
//	glDisableVertexAttribArray(ColorLocation);
//
//	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
//}
////--- �ٽñ׸��� �ݹ� �Լ�
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
//	case 'q': // ���α׷� ����
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
//				// pts �ǵ��
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
//				// �� ���� �߰�
//				shape s{ slices, pts.size() };
//				obj.push_back(s);
//
//				color c = { uidC(dre), uidC(dre), uidC(dre) };
//				makePolyObject(tmp.x, tmp.y, slices, obj.back().idx);
//				CENTER.push_back({ { tmp.x, tmp.y }, c });
//
//				cout << "�����" << endl;
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
//	glGenVertexArrays(1, &vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
//	glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�
//	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
//}
//
//void make_shaderProgram()
//{
//	make_vertexShaders(); //--- ���ؽ� ���̴� �����
//	make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����
//	//-- shader Program
//	shaderProgramID = glCreateProgram();
//	glAttachShader(shaderProgramID, vertexShader);
//	glAttachShader(shaderProgramID, fragmentShader);
//	glLinkProgram(shaderProgramID);
//	//--- ���̴� �����ϱ�
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);
//	//--- Shader Program ����ϱ�
//	glUseProgram(shaderProgramID);
//}
//
//void make_vertexShaders()
//{
//	vertexSource = filetobuf("vertex.glsl");
//		//--- ���ؽ� ���̴� ��ü �����
//		vertexShader = glCreateShader(GL_VERTEX_SHADER);
//	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
//	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
//	//--- ���ؽ� ���̴� �������ϱ�
//	glCompileShader(vertexShader);
//	//--- �������� ����� ���� ���� ���: ���� üũ
//	GLint result;
//	GLchar errorLog[512];
//	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
//	if(!result)
//	{
//		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
//		std::cout << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
//		return;
//	}
//}
//
//void make_fragmentShaders()
//{
//	fragmentSource = filetobuf("fragment.glsl");
//	//--- �����׸�Ʈ ���̴� ��ü �����
//	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
//	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
//	//--- �����׸�Ʈ ���̴� ������
//	glCompileShader(fragmentShader);
//	//--- �������� ����� ���� ���� ���: ������ ���� üũ
//	GLint result;
//	GLchar errorLog[512];
//	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
//	if (!result)
//	{
//		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
//		std::cout << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
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