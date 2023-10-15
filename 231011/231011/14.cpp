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
//vector<glm::vec3> v = { // ���峲 �ٵ� ����
//	{-0.25f, 0.25f, -0.25f}, {-0.25f, -0.25f, -0.25f}, {0.25f, -0.25f, -0.25f},
//	{-0.25f, 0.25f, -0.25f}, {0.25f, -0.25f, -0.25f}, {0.25f, 0.25f, -0.25f}, // �� 1
//	{0.25f, 0.25f, -0.25f}, {0.25f, -0.25f, -0.25f}, {0.25f, -0.25f, -0.75f},
//	{0.25f, 0.25f, -0.25f}, {0.25f, -0.25f, -0.75f}, {0.25f, 0.25f, -0.75f}, // �� 2
//	{0.25f, 0.25f, -0.75f}, {0.25f, -0.25f, -0.75f}, {-0.25f, -0.25f, -0.75f},
//	{0.25f, 0.25f, -0.75f}, {-0.25f, -0.25f, -0.75f}, {-0.25f, 0.25f, -0.75f}, // �� 3
//	{-0.25f, 0.25f, -0.75f}, {-0.25f, -0.25f, -0.75f}, {-0.25f, 0.25f, -0.25f},
//	{-0.25f, -0.25f, -0.75f}, {-0.25f, -0.25f, -0.25f}, {-0.25f, 0.25f, -0.25f}, // �� 4
//	{-0.25f, 0.25f, -0.75f}, {-0.25f, 0.25f, -0.25f}, {0.25f, 0.25f, -0.25f},
//	{-0.25f, 0.25f, -0.75f}, {0.25f, 0.25f, -0.25f}, {0.25f, 0.25f, -0.75f}, // �� 5
//	{-0.25f, -0.25f, -0.75f}, {-0.25f, -0.25f, -0.25f}, {0.25f, -0.25f, -0.25f},
//	{-0.25f, -0.25f, -0.75f}, {0.25f, -0.25f, -0.25f}, {0.25f, -0.25f, -0.75f}, // �� 6
//	// ���ü
//	{-0.25f, 0.25f, -0.25f}, {-0.25f, -0.25f, -0.25f}, {0.25f, -0.25f, -0.25f},
//	{-0.25f, -0.25f, -0.75f}, {-0.25f, -0.25f, -0.25f}, {-0.25f, 0.25f, -0.25f},
//	{-0.25f, -0.25f, -0.75f}, {-0.25f, -0.25f, -0.25f}, {0.25f, -0.25f, -0.25f},
//	{-0.25f, 0.25f, -0.25f}, {-0.25f, -0.25f, -0.75f}, {0.25f, -0.25f, -0.25f}
//};
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
//
//	{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f},
//	{1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f},
//	{0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f},
//	{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
//};
//POINT mousept;
//
//int angle = 30;
//glm::vec3 transition{ 0.0f,0.0f,0.0f };
//bool arr[6]{ true, true, true, true, true, true };
//bool isRect = true;
//bool arr2[4]{ true, true, true, true };
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
//	glEnable(GL_DEPTH_TEST);
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
//	glm::mat4 model = glm::mat4(1.0f);
//	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
//	model = glm::rotate(model, glm::radians(float(angle)), glm::vec3(1.0f, 1.0f, 0.0f));
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
//	for (int i = 0; i < v.size(); i += 3)
//	{
//		if (isRect) // ������ü �׸���
//		{
//			if (i >= 36) break;
//			if(arr[i / 6])
//				glDrawArrays(GL_TRIANGLES, i, 3);
//
//		}
//		else
//		{
//			if (i < 36) continue;
//			if (arr2[(i - 36) / 3])
//				glDrawArrays(GL_TRIANGLES, i, 3);
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
//	case '1':
//		isRect = true;
//		for (int i = 0; i < 6; ++i)
//			arr[i] = false;
//		arr[0] = true;
//		break;
//	case '2':
//		isRect = true;
//		for (int i = 0; i < 6; ++i)
//			arr[i] = false;
//		arr[1] = true;
//		break;
//	case '3':
//		isRect = true;
//		for (int i = 0; i < 6; ++i)
//			arr[i] = false;
//		arr[2] = true;
//		break;
//	case '4':
//		isRect = true;
//		for (int i = 0; i < 6; ++i)
//			arr[i] = false;
//		arr[3] = true;
//		break;
//	case '5':
//		isRect = true;
//		for (int i = 0; i < 6; ++i)
//			arr[i] = false;
//		arr[4] = true;
//		break;
//	case '6':
//		isRect = true;
//		for (int i = 0; i < 6; ++i)
//			arr[i] = false;
//		arr[5] = true;
//		break;
//
//
//	case '7':
//		isRect = false;
//		for (int i = 0; i < 4; ++i)
//			arr2[i] = false;
//		arr2[0] = true;
//		break;
//	case '8':
//		isRect = false;
//		for (int i = 0; i < 4; ++i)
//			arr2[i] = false;
//		arr2[1] = true;
//		break;
//	case '9':
//		isRect = false;
//		for (int i = 0; i < 4; ++i)
//			arr2[i] = false;
//		arr2[2] = true;
//		break;
//	case '0':
//		isRect = false;
//		for (int i = 0; i < 4; ++i)
//			arr2[i] = false;
//		arr2[3] = true;
//		break;
//
//	case 't':
//		isRect = true;
//		for (int i = 0; i < 6; ++i)
//			arr[i] = false;
//		arr[uid(dre)] = true;
//		while (true)
//		{
//			int a = uid(dre);
//			if (!arr[a])
//			{
//				arr[a] = true;
//				break;
//			}
//		}
//		break;
//
//	case 'c':
//		isRect = false;
//		for (int i = 0; i < 4; ++i)
//			arr2[i] = false;
//		arr2[uid2(dre)] = true;
//		while (true)
//		{
//			int a = uid2(dre);
//			if (!arr2[a])
//			{
//				arr2[a] = true;
//				break;
//			}
//		}
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