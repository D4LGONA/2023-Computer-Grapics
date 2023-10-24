#include "stdafx.h"
#include "object.h"

GLuint vao, vbo[2], ebo;

GLchar* vertexSource, * fragmentSource; //--- �ҽ��ڵ� ���� ����
GLuint vertexShader, fragmentShader; //--- ���̴� ��ü
GLuint shaderProgramID; //--- ���̴� ���α׷�

void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);
void InitBuffer();
char* filetobuf(const char*);

POINT mousept;
object big = { 0.5f, {0.0f, 0.0f, 0.0f}, {0.0f,0.0f,0.0f}, 0.0f, {uidC(dre), uidC(dre), uidC(dre)} };
vector<object> mid;
vector<object> small;
bool isSpinY = false;
bool isSpinZ = false;

vector<glm::vec3> pts;
vector<glm::vec3> c;

void makecircle()
{
	for (int j = 0; j < 6; ++j)
	{
		for (int i = 0; i < 360; ++i)
		{
			pts.push_back({ sin(glm::radians(float(i))), 0.0f,cos(glm::radians(float(i))) });
			c.push_back({ 0.0f,1.0f,0.0f });
		}
	}
}

void Reset()
{
	makecircle();

	big = { 0.5f, {0.0f, 0.0f, 0.0f}, {0.0f,0.0f,0.0f}, 0.0f, {uidC(dre), uidC(dre), uidC(dre)} };

	mid.push_back({ 0.3f, {0.5f, 0.0f, 0.0f} , {0.0f, 0.0f, 0.0f} , 2.0f, {uidC(dre), uidC(dre), uidC(dre)} });
	mid.push_back({ 0.3f, {0.5f, 0.0f, 0.0f} , {0.0f, 0.0f, 45.0f}, 1.0f, {uidC(dre), uidC(dre), uidC(dre)} });
	mid.push_back({ 0.3f, {0.5f, 0.0f, 0.0f} , {0.0f, 0.0f, -45.0f} , 4.0f, {uidC(dre), uidC(dre), uidC(dre)} });

	small.push_back({ 0.1f, {0.3f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} , 10.0f, {uidC(dre), uidC(dre), uidC(dre)} });
	small.push_back({ 0.1f, {0.3f, 0.0f, 0.0f}, {0.0f, 0.0f, -0.0f}, 8.0f , {uidC(dre), uidC(dre), uidC(dre)} });
	small.push_back({ 0.1f, {0.3f, 0.0f, 0.0f}, {0.0f, 0.0f, +0.0f}, 11.0f , {uidC(dre), uidC(dre), uidC(dre)} });

	proj = glm::mat4(1.0f);
	proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f); //--- ���� ���� ����: fovy, aspect, near, far
	proj = glm::translate(proj, glm::vec3(0.0, 0.0, -5.0));
}

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Example1");
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	make_shaderProgram();
	InitBuffer();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(Keyboard);
	Reset();
	glutTimerFunc(100, TimerFunction, 1);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);
	
	
	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_COLOR_MATERIAL);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * pts.size(), pts.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * c.size(), c.data(), GL_STATIC_DRAW);


	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 2.0f); //--- ī�޶� ��ġ
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- ī�޶� �ٶ󺸴� ����
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- ī�޶� ���� ����
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	// Location ��ȣ ����
	unsigned int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position"); //	: 0  Shader�� 'layout (location = 0)' �κ�
	unsigned int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color"); //	: 1
	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "transform");
	unsigned int viewLocation = glGetUniformLocation(shaderProgramID, "view");
	unsigned int projLocation = glGetUniformLocation(shaderProgramID, "projection");
	unsigned int cLocation = glGetUniformLocation(shaderProgramID, "color");

	glEnableVertexAttribArray(PosLocation); // Enable �ʼ�! ����ϰڴ� �ǹ�
	glEnableVertexAttribArray(ColorLocation);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // ���� ��ǥ�� VBO ���ε�
	glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // ���� �����Ϳ� VBO ���ε�
	glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	
	for (int i = 0; i < 6; ++i)
	{
		
		if (i < 3)
		{
			glm::mat4 m = glm::mat4(1.0f);

			m = glm::translate(m, big.transition);
			m = glm::rotate(m, glm::radians(rotZ), {0.0f, 0.0f, 1.0f});
			m = glm::rotate(m, glm::radians(rotY), {0.0f, 1.0f, 0.0f});
			m = glm::rotate(m, glm::radians(mid[i].orbit), { 0.0f, 0.0f, 1.0f });
			m = glm::translate(m, -big.transition);

			m = glm::translate(m, big.transition);

			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(m));
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
			glUniformMatrix4fv(projLocation, 1, GL_FALSE, &proj[0][0]);
			glUniform3f(cLocation, 0.0f, 0.0f, 0.0f);
			glDrawArrays(GL_LINE_LOOP, i * 360, 360);
		}
		else
		{
			glm::mat4 m = glm::mat4(1.0f);

			/*m = glm::translate(m, big.transition);
			m = glm::rotate(m, glm::radians(mid[i - 3].orbit), { 0.0f, 0.0f, 1.0f });
			m = glm::translate(m, -big.transition)*/

			m = glm::translate(m, { mid[i - 3].matrix[3][0], mid[i - 3].matrix[3][1], mid[i - 3].matrix[3][2] });
			m = glm::rotate(m, glm::radians(rotY), {0.0f, 1.0f, 0.0f});
			m = glm::rotate(m, glm::radians(rotZ), { 0.0f, 0.0f, 1.0f });
			//m = glm::rotate(m, glm::radians(mid[i - 3].orbit), { 0.0f, 0.0f, 1.0f });
			m = glm::translate(m, { -mid[i - 3].matrix[3][0], -mid[i - 3].matrix[3][1], -mid[i - 3].matrix[3][2] });

			//m = glm::rotate(m, glm::radians(rotZ), { 0.0f, 0.0f, 1.0f });
			m = glm::translate(m, { mid[i - 3].matrix[3][0], mid[i - 3].matrix[3][1], mid[i - 3].matrix[3][2] });
			m = glm::scale(m, {0.6f, 0.6f, 0.6f });

			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(m));
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
			glUniformMatrix4fv(projLocation, 1, GL_FALSE, &proj[0][0]);
			glUniform3f(cLocation, 0.0f, 0.0f, 0.0f);
			glDrawArrays(GL_LINE_LOOP, i * 360, 360);
		}

		
	}



	big.render(vbo, ebo, shaderProgramID);
	mid[0].render(vbo, ebo, shaderProgramID);
	mid[1].render(vbo, ebo, shaderProgramID);
	mid[2].render(vbo, ebo, shaderProgramID);
	small[0].render(vbo, ebo, shaderProgramID);
	small[1].render(vbo, ebo, shaderProgramID);
	small[2].render(vbo, ebo, shaderProgramID);

	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}
//--- �ٽñ׸��� �ݹ� �Լ�
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'm':
		isWire = !isWire;
		break;

	case 'y':
		isSpinY = !isSpinY;
		big.isSpinPlus = true;
		big.isSpinY = isSpinY;
		//big.isSpinPlus = true;
		mid[0].isSpinY = isSpinY;
		mid[1].isSpinY = isSpinY;
		mid[2].isSpinY = isSpinY;
		small[0].isSpinY = isSpinY;
		small[1].isSpinY = isSpinY;
		small[2].isSpinY = isSpinY;
		mid[0].  isSpinPlus = true;
		mid[1].  isSpinPlus = true;
		mid[2].  isSpinPlus = true;
		small[0].isSpinPlus = true;
		small[1].isSpinPlus = true;
		small[2].isSpinPlus = true;
		break;

	case 'Y':
		isSpinY = !isSpinY;
		big.isSpinPlus = false;
		big.isSpinY = isSpinY;
		mid[0].isSpinY = isSpinY;
		mid[1].isSpinY = isSpinY;
		mid[2].isSpinY = isSpinY;
		small[0].isSpinY = isSpinY;
		small[1].isSpinY = isSpinY;
		small[2].isSpinY = isSpinY;
		mid[0].isSpinPlus =   false;
		mid[1].isSpinPlus =   false;
		mid[2].isSpinPlus =   false;
		small[0].isSpinPlus = false;
		small[1].isSpinPlus = false;
		small[2].isSpinPlus = false;
		break;

	case 'w':
		big.transition.y += 0.2f;
		/*mid[0].transition.y += 0.2f;
		mid[1].transition.y += 0.2f;
		mid[2].transition.y += 0.2f;*/
		/*small[0].transition.y += 0.2f;
		small[1].transition.y += 0.2f;
		small[2].transition.y += 0.2f;*/
		
		break;

	case 'a':
		big.transition.x -= 0.2f;
		/*mid[0].transition.x -= 0.2f;
		mid[1].transition.x -= 0.2f;
		mid[2].transition.x -= 0.2f;*/
		/*small[0].transition.x -= 0.2f;
		small[1].transition.x -= 0.2f;
		small[2].transition.x -= 0.2f;*/
		break;

	case 's':
		big.transition.y -= 0.2f;
		/*mid[0].transition.y -= 0.2f;
		mid[1].transition.y -= 0.2f;
		mid[2].transition.y -= 0.2f;*/
		/*small[0].transition.y -= 0.2f;
		small[1].transition.y -= 0.2f;
		small[2].transition.y -= 0.2f;*/
		break;

	case 'd':
		big.transition.x += 0.2f;
		/*mid[0].transition.x += 0.2f;
		mid[1].transition.x += 0.2f;
		mid[2].transition.x += 0.2f;*/
		/*small[0].transition.x += 0.2f;
		small[1].transition.x += 0.2f;
		small[2].transition.x += 0.2f;*/
		break;
	case '+':
		big.transition.z += 0.2f;
		/*mid[0].transition.z += 0.2f;
		mid[1].transition.z += 0.2f;
		mid[2].transition.z += 0.2f;*/
		break;

	case'-':
		big.transition.z -= 0.2f;
		/*mid[0].transition.z -= 0.2f;
		mid[1].transition.z -= 0.2f;
		mid[2].transition.z -= 0.2f;*/
		break;

	case'z':
		isSpinZ = !isSpinZ;
		big.isSpinZ = isSpinZ;
		break;

	case 'p': // ��������?
		proj = glm::mat4(1.0f);
		proj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f);
		break;

	case 'P': // ��������?
		proj = glm::mat4(1.0f);
		proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f); //--- ���� ���� ����: fovy, aspect, near, far
		proj = glm::translate(proj, glm::vec3(0.0, 0.0, -5.0));
		break;


	case 'q': // ���α׷� ����
		exit(0);
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	if(isSpinZ)
		rotZ += 1.0f;

	if (isSpinY)
		rotY += 5.0f;

	big.update();
	mid[0].update();
	mid[1].update();
	mid[2].update();

	small[0].update();
	small[1].update();
	small[2].update();

	big.setmatrix();

	/*mid[0].setmatrix(big.rotation, big.transition, big.scale);
	mid[1].setmatrix(big.rotation, big.transition, big.scale);
	mid[2].setmatrix(big.rotation, big.transition, big.scale);*/

	mid[0].setmatrix(big.matrix);
	mid[1].setmatrix(big.matrix);
	mid[2].setmatrix(big.matrix);

	/*small[0].setmatrix(mid[0].rotation, mid[0].transition, mid[0].scale, big.rotation, big.transition, big.scale, mid[0].orbit);
	small[1].setmatrix(mid[1].rotation, mid[1].transition, mid[1].scale, big.rotation, big.transition, big.scale,mid[1].orbit);
	small[2].setmatrix(mid[2].rotation, mid[2].transition, mid[2].scale, big.rotation, big.transition, big.scale,mid[2].orbit);*/

	small[0].setmatrix(mid[0].matrix);
	small[1].setmatrix(mid[1].matrix);
	small[2].setmatrix(mid[2].matrix);

	glutPostRedisplay();
	glutTimerFunc(100, TimerFunction, 1);
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
	}

	return GLvoid();
}

GLvoid Motion(int x, int y)
{

	glutPostRedisplay();
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao); // VAO�� �����ϰ� �Ҵ��մϴ�.
	glBindVertexArray(vao); // VAO�� ���ε��մϴ�.
	glGenBuffers(2, vbo); // 2���� VBO�� �����ϰ� �Ҵ��մϴ�.

	/*glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int), cubeIdx.data(), GL_STATIC_DRAW);*/

	// ù ��° VBO�� ���� ��ǥ �����͸� �����մϴ�.
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * lines.size(), lines.data(), GL_STATIC_DRAW);

	//// �� ��° VBO�� ���� �����͸� �����մϴ�.
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * lineColor.size(), lineColor.data(), GL_STATIC_DRAW);

	// ���� ��ǥ �����͸� VAO�� ���ε��ϰ� Ȱ��ȭ�մϴ�.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// ���� �����͸� VAO�� ���ε��ϰ� Ȱ��ȭ�մϴ�.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

void make_shaderProgram()
{
	make_vertexShaders(); //--- ���ؽ� ���̴� �����
	make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program ����ϱ�
	glUseProgram(shaderProgramID);
}

void make_vertexShaders()
{
	vertexSource = filetobuf("vertex.glsl");
	//--- ���ؽ� ���̴� ��ü �����
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexShader);
	//--- �������� ����� ���� ���� ���: ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cout << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	fragmentSource = filetobuf("fragment.glsl");
	//--- �����׸�Ʈ ���̴� ��ü �����
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentShader);
	//--- �������� ����� ���� ���� ���: ������ ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cout << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading 
	if (!fptr) // Return NULL on failure 
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file 
	length = ftell(fptr); // Find out how many bytes into the file we are 
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator 
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file 
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer 
	fclose(fptr); // Close the file 
	buf[length] = 0; // Null terminator 
	return buf; // Return the buffer 
}