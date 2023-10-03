//#include "stdafx.h"
//
//random_device rd;
//mt19937 dre(rd());
//uniform_real_distribution<float> uidC{ 0.0f, 1.0f }; // ���� �÷� ����
//uniform_real_distribution<float> uidD{ -1.0f, 1.0f }; // ���� ���� ����
//uniform_int_distribution<int> uid{ 0,800 };
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
//color c;
//vertex v; // ������ ����Ǿ��ִ� �༮
//vector<POINT> pts;
//vector<POINT> pt;
//vector<POINT> movept;
//float angle = 0.0f;
//float Radius = 0.0f;
//bool start = false;
//bool isbigger = true;
//bool isline = false;
//int n = 1;
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
//	// Location ��ȣ ����
//	int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position"); //	: 0  Shader�� 'layout (location = 0)' �κ�
//	int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color"); //	: 1
//
//	glEnableVertexAttribArray(PosLocation); // Enable �ʼ�! ����ϰڴ� �ǹ�
//	glEnableVertexAttribArray(ColorLocation);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // ���� ��ǥ�� VBO ���ε�
//	glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // ���� �����Ϳ� VBO ���ε�
//	glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(color), 0);
//
//	for (int i = 0; i < pts.size(); ++i)
//	{
//		if (isline)
//		{
//			if (i == pts.size() - 1) continue;
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
//	case 'p':
//		isline = false;
//		break;
//
//	case 'l':
//		isline = true;
//		break;
//
//	case '1':
//		Radius = 0.0f;
//		angle = 0.0f;
//		pt.clear();
//		movept.clear();
//		pts.clear();
//		isbigger = true;
//		pt.push_back({ uid(dre), uid(dre) });
//		start = true;
//		break;
//	case '2':
//		Radius = 0.0f;
//		angle = 0.0f;
//		pt.clear();
//		movept.clear();
//		pts.clear();
//		isbigger = true;
//		pt.push_back({ uid(dre), uid(dre) });
//		pt.push_back({ uid(dre), uid(dre) });
//		start = true;
//		break;
//	case '3':
//		Radius = 0.0f;
//		angle = 0.0f;
//		pt.clear();
//		movept.clear();
//		pts.clear();
//		isbigger = true;
//		pt.push_back({ uid(dre), uid(dre) });
//		pt.push_back({ uid(dre), uid(dre) });
//		pt.push_back({ uid(dre), uid(dre) });
//		start = true;
//		break;
//	case '4':
//		Radius = 0.0f;
//		angle = 0.0f;
//		pt.clear();
//		movept.clear();
//		pts.clear();
//		isbigger = true;
//		pt.push_back({ uid(dre), uid(dre) });
//		pt.push_back({ uid(dre), uid(dre) });
//		pt.push_back({ uid(dre), uid(dre) });
//		pt.push_back({ uid(dre), uid(dre) });
//		start = true;
//		break;
//	case '5':
//		Radius = 0.0f;
//		angle = 0.0f;
//		pt.clear();
//		movept.clear();
//		pts.clear();
//		isbigger = true;
//		pt.push_back({ uid(dre), uid(dre) });
//		pt.push_back({ uid(dre), uid(dre) });
//		pt.push_back({ uid(dre), uid(dre) });
//		pt.push_back({ uid(dre), uid(dre) });
//		pt.push_back({ uid(dre), uid(dre) });
//		start = true;
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
//			for(int i = 0; i < pt.size(); ++i)
//				pts.push_back({ int(pt[i].x + Radius * cos(angle * 3.14f / 180)), int(pt[i].y + Radius * sin(angle * 3.14f / 180))});
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
//				pts.push_back({ int(movept[i].x + Radius * cos(angle * 3.14f / 180)), int(movept[i].y + Radius * sin(angle * 3.14f / 180)) });
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
//		isbigger = true;
//		pt.push_back({ x, y });
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
//	//--- ���ؽ� ���̴� ��ü �����
//	vertexShader = glCreateShader(GL_VERTEX_SHADER);
//	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
//	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
//	//--- ���ؽ� ���̴� �������ϱ�
//	glCompileShader(vertexShader);
//	//--- �������� ����� ���� ���� ���: ���� üũ
//	GLint result;
//	GLchar errorLog[512];
//	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
//	if (!result)
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