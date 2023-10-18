//#include "stdafx.h"
//
//random_device rd;
//mt19937 dre(rd());
//uniform_real_distribution<float> uidC{ 0.0f, 1.0f }; // ���� �÷� ����
//uniform_int_distribution uidS{ 0,3 };
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
//void ReadObj(FILE* path);
//
//struct object
//{
//	GLUquadric* obj;
//	glm::mat4 matrix{ 1.0f };
//	int angleX = 0;
//	int angleY = 0;
//	glm::vec3 transition{ 0.0f, 0.0f, 0.0f };
//	int state = 0;
//};
//
//object objs[4];
//
//POINT mousept;
//
//vector<glm::vec3> lines = {
//	{1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},
//	{0.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f},
//	{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}
//};
//
//vector<glm::vec3> lineColor = {
//	{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
//	{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
//	{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}
//};
//
//vector<glm::vec3> cube;
//vector<glm::vec3> cubeColor;
//vector<unsigned int> cubeIdx;
//
//glm::vec3 transitionL{ 0.5f, 0.0f, 0.0f };
//glm::vec3 transitionR{ -0.5f, 0.0f, 0.0f };
//int angleY = 0;
//
//bool bigTurn = false;
//
//bool bigMinus = false;
//
//bool TurnX = false;
//bool TurnY = false;
//
//bool TurnMinus = false;
//
//bool turnR = false;
//bool turnL = false;
//
//object* Right = nullptr;
//object* Left = nullptr;
//
//float dist(int x1, int y1, int x2, int y2)
//{
//	return sqrt(powf(x2 - x1, 2) + powf(y2 - y1, 2));
//}
//
//int length = 0;
//
//void update() // ��Ʈ���� �ʱ�ȭ�ϰ� �ٽ� �Ѱ� �� ����
//{
//	for (int i = 0; i < 4; ++i)
//	{
//		glm::mat4 tmp{ 1.0f };
//		objs[i].matrix = tmp;
//
//		objs[i].matrix = glm::rotate(objs[i].matrix, glm::radians(-45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
//		objs[i].matrix = glm::rotate(objs[i].matrix, glm::radians(float(angleY)), glm::vec3(0.0f, 1.0f, 0.0f));
//		objs[i].matrix = glm::translate(objs[i].matrix, objs[i].transition);
//		objs[i].matrix = glm::rotate(objs[i].matrix, glm::radians(float(objs[i].angleY)), glm::vec3(0.0f, 1.0f, 0.0f));
//		objs[i].matrix = glm::rotate(objs[i].matrix, glm::radians(float(objs[i].angleX)), glm::vec3(1.0f, 0.0f, 0.0f));
//		objs[i].matrix = glm::scale(objs[i].matrix, glm::vec3(0.2f, 0.2f, 0.2f));
//	}
//}
//
//void bind()
//{
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * cubeIdx.size(), cubeIdx.data(), GL_STATIC_DRAW);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * cube.size(), cube.data(), GL_DYNAMIC_DRAW);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * cubeColor.size(), cubeColor.data(), GL_DYNAMIC_DRAW);
//}
//
//void Reset()
//{
//	angleY = 0;
//
//	bigTurn = false;
//
//	bigMinus = false;
//
//	TurnX = false;
//	TurnY = false;
//
//	TurnMinus = false;
//
//	turnR = false;
//	turnL = false;
//
//	for (int i = 0; i < 4; ++i)
//	{
//		objs[i].obj = gluNewQuadric();
//		gluQuadricDrawStyle(objs[i].obj, GLU_LINE);
//		gluQuadricNormals(objs[i].obj, GLU_SMOOTH);
//		gluQuadricOrientation(objs[i].obj, GLU_OUTSIDE);
//		objs[i].transition = { 0.0f, 0.0f, 0.0f };
//		objs[i].state = i;
//	}
//
//	Left = &objs[0];
//	Right = &objs[1];
//
//	Left->transition = transitionL;
//	Right->transition = transitionR;
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
//	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glUseProgram(shaderProgramID);
//	glBindVertexArray(vao);
//
//	glEnable(GL_DEPTH_TEST);
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
//
//	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Left->matrix));
//	switch (Left->state)
//	{
//	case 0:
//		gluSphere(Left->obj, 1.0f, 20, 20);
//		break;
//	case 1:
//		gluCylinder(Left->obj, 1.0f, 0.0f, 2.0f, 20, 8);
//		break;
//	case 2:
//		gluCylinder(Left->obj, 1.0f, 1.0f, 2.0f, 20, 8);
//		break;
//	case 3:
//		gluSphere(Left->obj, 1.0f, 4, 2);
//		break;
//	}
//
//	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Right->matrix));
//	switch (Right->state)
//	{
//	case 0:
//		gluSphere(Right->obj, 1.0f, 20, 20);
//		break;
//	case 1:
//		gluCylinder(Right->obj, 1.0f, 0.0f, 2.0f, 20, 8);
//		break;
//	case 2:
//		gluCylinder(Right->obj, 1.0f, 1.0f, 2.0f, 20, 8);
//		break;
//	case 3:
//		gluSphere(Right->obj, 1.0f, 4, 2);
//		break;
//	}
//
//	glm::mat4 axis{ 1.0f };
//	axis = glm::rotate(axis, glm::radians(-45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
//	axis = glm::rotate(axis, glm::radians(float(angleY)), glm::vec3(0.0f, 1.0f, 0.0f));
//	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(axis));
//
//	for (int i = 0; i < lines.size(); i += 2)
//	{
//		glDrawArrays(GL_LINES, i, i + 2);
//	}
//
//	// �׸��� // 
//
//	/*for (int i = 0; i < cubeIdx.size(); i+= 3)
//		glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, (void*)(i * sizeof(GLuint)));*/
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
//	case 'o':
//	{
//		// obj ���Ͽ��� �о���� �ڵ�
//		FILE* file = fopen("cube2.obj", "r");
//		if (file == NULL)
//		{
//			exit(0);
//		}
//		else
//		{
//			ReadObj(file);
//			fclose(file);
//		}
//		break;
//
//	}
//
//	case 'x': // x�� ���ǹ�������
//		turnR = true;
//		turnL = true;
//		TurnX = true;
//		TurnY = false;
//		TurnMinus = false;
//		break;
//
//	case 'X': //x�� ���ǹ��� ����
//		turnR = true;
//		turnL = true;
//		TurnX = true;
//		TurnY = false;
//		TurnMinus = true;
//		break;
//
//	case 'y': // y�� ���ǹ��� ����
//		turnR = true;
//		turnL = true;
//		TurnX = false;
//		TurnY = true;
//		TurnMinus = false;
//		break;
//
//	case 'Y': // y�� ���ǹ��� ����
//		turnR = true;
//		turnL = true;
//		TurnX = false;
//		TurnY = true;
//		TurnMinus = true;
//		break;
//
//	case '1':
//		turnR = true;
//		turnL = false;
//		break;
//	case '2':
//		turnL = true;
//		turnR = false;
//		break;
//	case '3':
//		turnL = true;
//		turnR = true;
//		break;
//
//	case 'r': // y�࿡ ���� ���ǹ��� ����
//		bigTurn = true;
//		bigMinus = false;
//		break;
//
//	case 'R': // y�࿡ ���� ���ǹ��� ����
//		bigTurn = true;
//		bigMinus = true;
//		break;
//
//	case 'c': // ���� �ٲٱ� ����
//		Right = &objs[uidS(dre)];
//		Right->transition = transitionR;
//		while (true)
//		{
//			int tmp = uidS(dre);
//			if (Right != &objs[tmp])
//			{
//				Left = &objs[tmp];
//				Left->transition = transitionL;
//				break;
//			}
//		}
//		break;
//
//	case 's': // reset
//		Reset();
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
//	if (TurnX)
//	{
//		if (TurnMinus)
//		{
//			if (turnR)
//				Right->angleX -= 5;
//			if (turnL)
//				Left->angleX -= 5;
//		}
//		else
//		{
//			if (turnR)
//				Right->angleX += 5;
//			if (turnL)
//				Left->angleX += 5;
//		}
//	}
//	if (TurnY)
//	{
//		if (TurnMinus)
//		{
//			if (turnR)
//				Right->angleY -= 5;
//			if (turnL)
//				Left->angleY -= 5;
//		}
//		else
//		{
//			if (turnR)
//				Right->angleY += 5;
//			if (turnL)
//				Left->angleY += 5;
//		}
//	}
//
//	if (bigTurn)
//	{
//		if (bigMinus)
//			angleY -= 5;
//		else
//			angleY += 5;
//	}
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
//	glGenBuffers(1, &ebo);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int), cubeIdx.data(), GL_STATIC_DRAW);
//
//	// ù ��° VBO�� ���� ��ǥ �����͸� �����մϴ�.
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * lines.size(), lines.data(), GL_STATIC_DRAW);
//
//	// �� ��° VBO�� ���� �����͸� �����մϴ�.
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * lineColor.size(), lineColor.data(), GL_STATIC_DRAW);
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
//void ReadObj(FILE* path)
//{
//	char count[128];
//	int vertexnum = 0;
//	int facenum = 0;
//	int uvnum = 0;
//	//--- 1. ��ü ���ؽ� ���� �� �ﰢ�� ���� ����
//	while (!feof(path))
//	{
//		fscanf(path, "%s", count);
//		if (count[0] == 'v' && count[1] == '\0')
//			vertexnum++;
//		else if (count[0] == 'f' && count[1] == '\0')
//			facenum++;
//		else if (count[0] == 'v' && count[1] == 't' && count[3] == '\0')
//			uvnum++;
//		memset(count, '\0', sizeof(count));
//	}
//	rewind(path);
//	int vertIndex = 0;
//	int faceIndex = 0;
//	int uvIndex = 0;
//	//--- 2. �޸� �Ҵ�
//	glm::vec3* vertex = new glm::vec3[vertexnum];
//	glm::vec3* face = new glm::vec3[facenum];
//	glm::vec3* uvdata = new glm::vec3[facenum];
//	glm::vec2* uv = new glm::vec2[uvnum];
//	char bind[128];
//
//	while (!feof(path)) {
//		fscanf(path, "%s", bind);
//		if (bind[0] == 'v' && bind[1] == '\0') {
//			fscanf(path, "%f %f %f\n",
//				&vertex[vertIndex].x, &vertex[vertIndex].y, &vertex[vertIndex].z);
//			vertIndex++;
//		}
//		else if (bind[0] == 'f' && bind[1] == '\0') {
//			unsigned int temp_face[3], temp_uv[3], temp_normal[3];
//			fscanf(path, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
//				&temp_face[0], &temp_uv[0], &temp_normal[0],
//				&temp_face[1], &temp_uv[1], &temp_normal[1],
//				&temp_face[2], &temp_uv[2], &temp_normal[2]);
//			face[faceIndex].x = temp_face[0] - 1;
//			face[faceIndex].y = temp_face[1] - 1;
//			face[faceIndex].z = temp_face[2] - 1;
//			uvdata[faceIndex].x = temp_uv[0];
//			uvdata[faceIndex].y = temp_uv[1];
//			uvdata[faceIndex].z = temp_uv[2];
//			faceIndex++;
//		}
//		else if (bind[0] == 'v' && bind[1] == 't' && bind[2] == '\0') {
//			fscanf(path, "%f %f\n", &uv[uvIndex].x, &uv[uvIndex].y);
//			uvIndex++;
//		}
//	}
//
//
//	for (int i = 0; i < vertexnum; ++i)
//	{
//		cube.push_back(vertex[i]);
//		cubeColor.push_back({ uidC(dre),uidC(dre),uidC(dre) });
//	}
//
//	for (int i = 0; i < facenum; ++i)
//	{
//		cubeIdx.push_back(face[i].x);
//		cubeIdx.push_back(face[i].y);
//		cubeIdx.push_back(face[i].z);
//	}
//}
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