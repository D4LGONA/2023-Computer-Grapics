//#include "stdafx.h"
//
//random_device rd;
//mt19937 dre(rd());
//uniform_real_distribution<float> uidC{ 0.0f, 1.0f }; // ���� �÷� ����
//uniform_int_distribution<int> uid{ 0,5 };
//uniform_int_distribution<int> uid2{ 0,3 };
//
//GLuint vao, vbo[2], ebo;
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
//vector<glm::vec3> v = {
//	{-0.25f, 0.25f,   0.25f}, {-0.25f, -0.25f, 0.25f},  {0.25f, -0.25f, 0.25f},
//	{-0.25f, 0.25f,   0.25f}, {0.25f, -0.25f, 0.25f},   {0.25f, 0.25f, 0.25f}, // �� 1
//	{0.25f, 0.25f,    0.25f}, {0.25f, -0.25f, 0.25f},   {0.25f, -0.25f, -0.25f},
//	{0.25f, 0.25f,    0.25f}, {0.25f, -0.25f, -0.25f},  {0.25f, 0.25f, -0.25f}, // �� 2
//	{0.25f, 0.25f,   -0.25f}, {0.25f, -0.25f, -0.25f},  {-0.25f, -0.25f, -0.25f},
//	{0.25f, 0.25f,   -0.25f}, {-0.25f, -0.25f, -0.25f}, {-0.25f, 0.25f, -0.25f}, // �� 3
//	{-0.25f, 0.25f,  -0.25f}, {-0.25f, -0.25f, -0.25f}, {-0.25f, 0.25f, 0.25f},
//	{-0.25f, -0.25f, -0.25f}, {-0.25f, -0.25f, 0.25f},  {-0.25f, 0.25f, 0.25f}, // �� 4
//	{-0.25f, 0.25f,  -0.25f}, {-0.25f, 0.25f, 0.25f},   {0.25f, 0.25f, 0.25f},
//	{-0.25f, 0.25f,  -0.25f}, {0.25f, 0.25f, 0.25f},    {0.25f, 0.25f, -0.25f}, // �� 5
//	{-0.25f, -0.25f, -0.25f}, {-0.25f, -0.25f, 0.25f},  {0.25f, -0.25f, 0.25f},
//	{-0.25f, -0.25f, -0.25f}, {0.25f, -0.25f, 0.25f},   {0.25f, -0.25f, -0.25f}, // �� 6
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
//	{-0.25f, 0.25f,   0.25f}, {0.25f, -0.25f, 0.25f},   {0.25f, 0.25f, 0.25f}, // �� 1
//	{0.25f, 0.25f,    0.25f}, {0.25f, -0.25f, 0.25f},   {0.25f, -0.25f, -0.25f},
//	{0.25f, 0.25f,    0.25f}, {0.25f, -0.25f, -0.25f},  {0.25f, 0.25f, -0.25f}, // �� 2
//	{0.25f, 0.25f,   -0.25f}, {0.25f, -0.25f, -0.25f},  {-0.25f, -0.25f, -0.25f},
//	{0.25f, 0.25f,   -0.25f}, {-0.25f, -0.25f, -0.25f}, {-0.25f, 0.25f, -0.25f}, // �� 3
//	{-0.25f, 0.25f,  -0.25f}, {-0.25f, -0.25f, -0.25f}, {-0.25f, 0.25f, 0.25f},
//	{-0.25f, -0.25f, -0.25f}, {-0.25f, -0.25f, 0.25f},  {-0.25f, 0.25f, 0.25f}, // �� 4
//	{-0.25f, 0.25f,  -0.25f}, {-0.25f, 0.25f, 0.25f},   {0.25f, 0.25f, 0.25f},
//	{-0.25f, 0.25f,  -0.25f}, {0.25f, 0.25f, 0.25f},    {0.25f, 0.25f, -0.25f}, // �� 5
//	{-0.25f, -0.25f, -0.25f}, {-0.25f, -0.25f, 0.25f},  {0.25f, -0.25f, 0.25f},
//	{-0.25f, -0.25f, -0.25f}, {0.25f, -0.25f, 0.25f},   {0.25f, -0.25f, -0.25f}, // �� 6
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
//void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
//{
//	//--- ������ �����ϱ�
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
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
//	// Location ��ȣ ����
//	int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position"); //	: 0  Shader�� 'layout (location = 0)' �κ�
//	int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color"); //	: 1
//	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "transform");
//
//
//	glEnableVertexAttribArray(PosLocation); // Enable �ʼ�! ����ϰڴ� �ǹ�
//	glEnableVertexAttribArray(ColorLocation);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // ���� ��ǥ�� VBO ���ε�
//	glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // ���� �����Ϳ� VBO ���ε�
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
//	case 'c': // ����ü ���Ͷ� ��
//		isRect = true;
//		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * v.size(), v.data(), GL_STATIC_DRAW);
//		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * c.size(), c.data(), GL_STATIC_DRAW);
//		break;
//
//	case 'p': // �簢�� ���Ͷ� ��
//		isRect = false;
//		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * v1.size(), v1.data(), GL_STATIC_DRAW);
//		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * c1.size(), c1.data(), GL_STATIC_DRAW);
//		break;
//
//	case 'h': // �������� ����/����
//		isHide = !isHide;
//		break;
//
//	case 'w': // ���̾� ��ü
//		isSolid = false;
//		break;
//
//	case 'W': // �ָ��� ��ü
//		isSolid = true;
//		break;
//
//	case 'x': // x����� ���� ���� ȸ��
//		isX = true;
//		isY = false;
//		isMinus = false;
//		break;
//
//	case 'X': // x����� ���� ���� ȸ��
//		isX = true;
//		isY = false;
//		isMinus = true;
//		break;
//
//	case 'y': // y����� ���ǹ�� ȸ��
//		isX = false;
//		isY = true;
//		isMinus = false;
//		break;
//
//	case 'Y': // y����� ���̤ѹ��� ȸ��
//		isX = false;
//		isY = true;
//		isMinus = true;
//		break;
//
//	case '1': //�����̵�
//		Y += 0.05f;
//		break;
//
//	case '2': //�Ʒ��� �̵�
// 		Y -= 0.05f;
//		break;
//
//	case '3': // ������ �̵�
//		X += 0.05f;
//		break;
//
//	case '4': // �����̵�
//		X -= 0.05f;
//		break;
//
//	case 's': // �ʱ�ȭ
//		Reset();
//		break;
//
//	case VK_DOWN:
//		X = 0;
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
//	glGenVertexArrays(1, &vao); // VAO�� �����ϰ� �Ҵ��մϴ�.
//	glBindVertexArray(vao); // VAO�� ���ε��մϴ�.
//	glGenBuffers(2, vbo); // 2���� VBO�� �����ϰ� �Ҵ��մϴ�.
//
//	// ù ��° VBO�� ���� ��ǥ �����͸� �����մϴ�.
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * v.size(), v.data(), GL_STATIC_DRAW);
//
//	// �� ��° VBO�� ���� �����͸� �����մϴ�.
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * c.size(), c.data(), GL_STATIC_DRAW);
//
//	// ���� ��ǥ �����͸� VAO�� ���ε��ϰ� Ȱ��ȭ�մϴ�.
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//	glEnableVertexAttribArray(0);
//
//	// ���� �����͸� VAO�� ���ε��ϰ� Ȱ��ȭ�մϴ�.
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
//	glEnableVertexAttribArray(1);
//}
//
//
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