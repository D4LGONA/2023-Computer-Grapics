#include "stdafx.h"
#include "object.h"

GLuint vao, vbo[2];

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

int cnt = 0;
bool isDrag = false;
vector<object> v;

POINT mousept;
glm::vec3 dragLine[2];
POINT linePt[2];
glm::vec3 lineColor[2] = { {0.0f, 1.0f, 1.0f }, {0.0f, 1.0f, 1.0f} };

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
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

	glutTimerFunc(50, TimerFunction, 1);

	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);
	
	// Location ��ȣ ����
	int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position"); //	: 0  Shader�� 'layout (location = 0)' �κ�
	int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color"); //	: 1

	
	glEnableVertexAttribArray(PosLocation); // Enable �ʼ�! ����ϰڴ� �ǹ�
	glEnableVertexAttribArray(ColorLocation);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // ���� ��ǥ�� VBO ���ε�
	glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // ���� �����Ϳ� VBO ���ε�
	glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	
	for (object& i : v)
		i.render(vbo);

	if (isDrag)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 2, dragLine, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 2, lineColor, GL_DYNAMIC_DRAW);

		glLineWidth(3.0f);
		glDrawArrays(GL_LINES, 0, 2);
	}

	glDisableVertexAttribArray(PosLocation); // Disable �ʼ�!
	glDisableVertexAttribArray(ColorLocation);

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
	case 'c':
		v.push_back({5, 30 });
		break;

	case 'w':
		isSolid = false;
		break;

	case 's':
		isSolid = true;
		break;

	case 'q': // ���α׷� ����
		for (int i = 0; i < v.size(); ++i)
			v[i].remove();

		exit(0);
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	/*cnt++;

	if (cnt >= 10)
	{
		v.push_back({ uidS(dre), 30 });
		cnt = 0;
	}*/

	for (object& i : v)
		i.move();

	for (object& i : v)
		i.update();
	

	glutPostRedisplay();
	glutTimerFunc(50, TimerFunction, 1);
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		isDrag = true;
		pair<float, float> tmp = WintoOpenGL({ x,y });
		dragLine[0] = { tmp.first, tmp.second, 0.0f };
		dragLine[1] = { tmp.first, tmp.second, 0.0f };
		linePt[0] = { x,y };
		linePt[1] = { x,y };
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		isDrag = false;
		vector<int> arr;
		int max = v.size();
		for(int count = 0; count < max; ++count)
		{
			pair<bool, pair<vector<POINT>, vector<POINT>>> tmp = v[count].isCross(linePt[0], linePt[1]);
			if (tmp.first)
			{
				for (int j = 0; j < tmp.second.first.size(); ++j)
					cout << tmp.second.first[j].x << ", " << tmp.second.first[j].y << endl;
				for (int j = 0; j < tmp.second.second.size(); ++j)
					cout << tmp.second.second[j].x << ", " << tmp.second.second[j].y << endl;


				v.push_back({ tmp.second.first });
				v.back().sliceMove(-1);
				v.push_back({ tmp.second.second });
				v.back().sliceMove(1);
				
				arr.push_back(count);
			}
		}

		for (int i = 0; i < arr.size(); ++i)
		{
			v.erase(v.begin() + (arr[i] - i));
		}
	}
	return GLvoid();
}

GLvoid Motion(int x, int y)
{
	if (isDrag)
	{
		pair<float, float> tmp = WintoOpenGL({ x,y });
		dragLine[1] = { tmp.first, tmp.second, 0.0f };
		linePt[1] = { x,y };
	}
	glutPostRedisplay();
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�
	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
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
	if(!result)
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