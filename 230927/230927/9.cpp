//#include "stdafx.h"
//
//random_device rd;
//mt19937 dre(rd());
//uniform_real_distribution<float> uidC{ 0.0f, 1.0f }; // ���� �÷� ����
//uniform_real_distribution<float> uidD{ -1.0f, 1.0f }; // ���� ���� ����
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
//vertex v; // ������ ����Ǿ��ִ� �༮
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
//
//	for (int i = 0; i < 4; ++i)
//		glDrawArrays(GL_TRIANGLES, i * 3, 3);
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
//	case '1': // ƨ���
//		if (dirs.size() == 0)
//			for (int i = 0; i < 4; ++i)
//				dirs.push_back({ uidD(dre), uidD(dre) });
//		state = 1;
//		break;
//
//	case '2': // �¿�� �������
//		if (dirs.size() == 0)
//			for (int i = 0; i < 4; ++i)
//				dirs.push_back({ uidD(dre), uidD(dre) });
//		state = 2;
//		break;
//		
//	case '3': // �簢 �����̷�
//		dirs.clear();
//		for (int i = 0; i < 4; ++i)
//			cts[i] = 0;
//		state = 3;
//		break;
//		
//	case '4': // �� �����̷�
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
//	case 'q': // ���α׷� ����
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
//		// �浹 üũ
//		for (int i = 0; i < 4; ++i)
//		{
//			bool width = false;
//			bool height = false;
//			// �� �Ʒ��� üũ
//			if (pts[i * 3].y < 0) // 1���� ���� ������
//			{
//				pts[i * 3].y = 0;
//				pts[i * 3 + 1].y = 0;
//				pts[i * 3 + 1].x = pts[i * 3].x - 40;
//				pts[i * 3 + 2].x = pts[i * 3].x - 20;
//				pts[i * 3 + 2].y = pts[i * 3].y + 60;
//				height = true;
//			}
//			else if (pts[i * 3].y > 800) // 1���� ������ ������
//			{
//				pts[i * 3].y = 800;
//				pts[i * 3 + 1].y = 800;
//				pts[i * 3 + 1].x = pts[i * 3].x + 40;
//				pts[i * 3 + 2].x = pts[i * 3].x + 20;
//				pts[i * 3 + 2].y = pts[i * 3].y - 60;
//				height = true;
//			}
//			else if (pts[i * 3 + 1].y < 0) // 2���� ���� ������
//			{
//				pts[i * 3 + 1].y = 0;
//				pts[i * 3].y = 0;
//				pts[i * 3].x = pts[i * 3 + 1].x + 40;
//				pts[i * 3 + 2].x = pts[i * 3].x - 20;
//				pts[i * 3 + 2].y = pts[i * 3].y + 60;
//				height = true;
//			}
//			else if (pts[i * 3 + 1].y > 800) // 2���� ������ ������
//			{
//				pts[i * 3 + 1].y = 800;
//				pts[i * 3].y = 800;
//				pts[i * 3].x = pts[i * 3 + 1].x - 40;
//				pts[i * 3 + 2].x = pts[i * 3].x + 20;
//				pts[i * 3 + 2].y = pts[i * 3].y - 60;
//				height = true;
//			}
//			else if (pts[i * 3 + 2].y < 0) // 3���� ����
//			{
//				pts[i * 3].y = 0;
//				pts[i * 3 + 1].y = 0;
//				pts[i * 3 + 2].y = pts[i * 3].y + 60;
//				height = true;
//			}
//			else if (pts[i * 3 + 2].y > 800) // 3���� ������
//			{
//				pts[i * 3].y = 800;
//				pts[i * 3 + 1].y = 800;
//				pts[i * 3 + 2].y = pts[i * 3].y - 60;
//				height = true;
//			}
//
//			// �¿��浹
//			if (pts[i * 3].x < 0) // 1���� �������� ������
//			{
//				pts[i * 3].x = 0;
//				pts[i * 3 + 1].x = 0;
//				pts[i * 3 + 1].y = pts[i * 3].y + 40;
//				pts[i * 3 + 2].x = pts[i * 3].x + 60;
//				pts[i * 3 + 2].y = pts[i * 3].y + 20;
//				width = true;
//			}
//			else if (pts[i * 3].x > 800) // 1���� ���������� ������
//			{
//				pts[i * 3].x = 800;
//				pts[i * 3 + 1].x = 800;
//				pts[i * 3 + 1].y = pts[i * 3].y - 40;
//				pts[i * 3 + 2].x = pts[i * 3].x - 60;
//				pts[i * 3 + 2].y = pts[i * 3].y - 20;
//				width = true;
//			}
//			else if (pts[i * 3 + 1].x < 0) // 2���� �������� ������
//			{
//				pts[i * 3 + 1].x = 0;
//				pts[i * 3].x = 0;
//				pts[i * 3].y = pts[i * 3 + 1].y - 40;
//				pts[i * 3 + 2].x = pts[i * 3].x + 60;
//				pts[i * 3 + 2].y = pts[i * 3].y + 20;
//				width = true;
//			}
//			else if (pts[i * 3 + 1].x > 800) // 2���� ���������� ������
//			{
//				pts[i * 3 + 1].x = 800;
//				pts[i * 3].x = 800;
//				pts[i * 3].y = pts[i * 3 + 1].y + 40;
//				pts[i * 3 + 2].x = pts[i * 3].x - 60;
//				pts[i * 3 + 2].y = pts[i * 3].y - 20;
//				width = true;
//			}
//			else if (pts[i * 3 + 2].x < 0) // 3���� ����
//			{
//				pts[i * 3].x = 0;
//				pts[i * 3 + 1].x = 0;
//				pts[i * 3 + 2].x = pts[i * 3].x + 60;
//				width = true;
//			}
//			else if (pts[i * 3 + 2].x > 800) // 3���� ������
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
//	case 2: // �������
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
//			if (pts[i * 3].x < 0 || pts[i * 3 + 1].x < 0 || pts[i * 3 + 2].x < 0) // �հ� �������� ������
//			{
//				pts[i * 3].x = 0;
//				pts[i * 3 + 1].x = 0;
//				pts[i * 3 + 2].x = 60;
//				pts[i * 3 + 1].y = pts[i * 3].y + 40;
//				pts[i * 3 + 2].y = pts[i * 3].y + 20;
//				dirs[i].first *= -1;
//				counts[i]++;
//			}
//			else if (pts[i * 3].x > 800 || pts[i * 3 + 1].x > 800 || pts[i * 3 + 2].x > 800) // �հ� �������� ������
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
//	case 3: // �簢 �����̷�
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