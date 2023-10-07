#include "stdafx.h"

random_device rd;
mt19937 dre(rd());
uniform_real_distribution<float> uidC{ 0.0f, 1.0f }; // ���� �÷� ����

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

vertex v;
POINT mousept;

enum types
{
	dot = 1,
	line = 2,
	triangle = 3,
	rectangle = 4,
	pentagon = 5
};

struct shape
{
	int type = 0;
	int idx = 0;
};

vector<POINT> pts;
bool drag = false;

pair<float, float> WintoOpenGL(int x, int y)
{
	pair<float, float> a = { x / 400. - 1, 1 - (y / 300.) };
	return a;
}

float dist(int x1, int y1, int x2, int y2)
{
	return sqrt(powf(x2 - x1, 2) + powf(y2 - y1, 2));
}

void update()
{
	v.pt.clear();
	
		for (int i = 0; i < pts.size(); ++i)
		{
			vec3 tmp = { WintoOpenGL(pts[i].x, pts[i].y).first, WintoOpenGL(pts[i].x, pts[i].y).second, 0.0f };
			v.pt.push_back(tmp);
		}

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * v.pt.size(), v.pt.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color) * v.c.size(), v.c.data(), GL_DYNAMIC_DRAW);
}

void Reset()
{
	pts.push_back({100, 100});
	v.c.push_back({ uidC(dre), uidC(dre), uidC(dre) });
	pts.push_back({100, 500});
	v.c.push_back({ uidC(dre), uidC(dre), uidC(dre) });
	pts.push_back({700, 500});
	v.c.push_back({ uidC(dre), uidC(dre), uidC(dre) });
	pts.push_back({700, 100});
	v.c.push_back({ uidC(dre), uidC(dre), uidC(dre) });
	pts.push_back({100, 100});
	v.c.push_back({ uidC(dre), uidC(dre), uidC(dre) });

	update();
}

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
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
	glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // ���� �����Ϳ� VBO ���ε�
	glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(color), 0);
	
	for (int i = 0; i < pts.size() - 1; ++i)
	{
		glLineWidth(5.0f);
		glDrawArrays(GL_LINES, i, 2);
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
	case 'q': // ���α׷� ����
		exit(0);
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	glutPostRedisplay();
	glutTimerFunc(100, TimerFunction, 1);
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		drag = true;
		mousept = { x, y };
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		drag = false;
	}

	return GLvoid();
}

GLvoid Motion(int x, int y)
{
	if (drag)
	{
		bool isdraged = false;
		for (int i = 0; i < pts.size(); ++i)
		{
			if (dist(pts[i].x, pts[i].y, x, y) < 50)
			{
				pts[i].x += x - mousept.x;
				pts[i].y += y - mousept.y;
				isdraged = true;
			}
		}

		if (!isdraged)
		{
			POINT pt1{ pts[0].x, pts[0].y },pt2{ pts[0].x, pts[0].y };
			// x1 y1 �ּ��� x2y2 �ִ���
			for (int i = 1; i < pts.size() - 1; ++i)
			{
				if (pts[i].x < pt1.x) pt1.x = pts[i].x;
				if (pts[i].y < pt1.y) pt1.y = pts[i].y;
				if (pts[i].x > pt2.x) pt2.x = pts[i].x;
				if (pts[i].y > pt2.y) pt2.y = pts[i].y;

			}
			RECT tmp = { pt1.x, pt1.y, pt2.x, pt2.y };
			if (PtInRect(&tmp, { x, y }))
			{
				for (int i = 0; i < pts.size(); ++i)
				{
					pts[i].x += x - mousept.x;
					pts[i].y += y - mousept.y;
				}
			}
		}


		mousept = { x, y };
		update();
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