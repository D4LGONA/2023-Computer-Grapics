#include "stdafx.h"
#include "object.h"

GLchar* vertexSource, * fragmentSource; //--- �ҽ��ڵ� ���� ����
GLuint vertexShader, fragmentShader; //--- ���̴� ��ü
GLuint shaderProgramID; //--- ���̴� ���α׷�

uniform_real_distribution<float> uidp{-20.0f, 20.0f};

void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboarddown(unsigned char key, int x, int y);
GLvoid Keyboardup(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);
char* filetobuf(const char*);

POINT mousept;
vector<vector<Object*>> v;

bool spinLight = false;
int w_count = 0;
int h_count = 0;

bool _1 = false;
bool _2 = false;
bool _3 = false;
int timertime = 50;

bool drag = false;

// ��ü ������� ������ 50*50���� �����մϴ�. �׷��� x�� z scale�� �޶��� �� �ֽ��ϴ�.
void Reset()
{
	if (v.size() != 0)
	{
		for (auto& i : v)
		{
			for (auto& j : i)
				delete j;
			i.clear();
		}
		v.clear();
	}

	cout << "����, ���� ���� ���� �Է��ϼ���: ";
	cin >> w_count >> h_count;

	float s_X = 50.0f / float(w_count);
	float s_Z = 50.0f / float(h_count);

	for (int i = 0; i < h_count; ++i)
	{
		v.push_back(vector<Object*>());
		for (int j = 0; j < w_count; ++j)
		{
			v[i].push_back(new Object("cube.obj", shaderProgramID, { s_X, 1.0f, s_Z }, { 0.0f, 0.0f, 0.0f }, {-25.0f + (j * s_X) + s_X / 2.0f, 0.0f, 25.0f - (i * s_Z) - s_Z / 2.0f}, {1.0f, 0.0f, 1.0f}));
		}
	}


	cameraPos = glm::vec3(0.0f, 100.0f, 100.0f); //--- ī�޶� ��ġ
	cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- ī�޶� �ٶ󺸴� ����
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- ī�޶� ���� ����
	view = glm::mat4(1.0f);
	cameraAngle = { 0.0f, 0.0f, 0.0f };

	proj = glm::mat4(1.0f);
	proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 200.0f); //--- ���� ���� ����: fovy, aspect, near, far
	proj = glm::translate(proj, glm::vec3(0.0, 0.0, 30.0f));

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	view = glm::rotate(view, glm::radians(cameraAngle.z), { 0.0f, 0.0f, 1.0f });
	view = glm::rotate(view, glm::radians(cameraAngle.y), { 0.0f, 1.0f, 0.0f });
	view = glm::rotate(view, glm::radians(cameraAngle.x), { 1.0f, 0.0f, 0.0f });
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
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(Keyboarddown);
	glutKeyboardUpFunc(Keyboardup);
	Reset();
	glutTimerFunc(timertime, TimerFunction, 1);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	
	glEnable(GL_DEPTH_TEST); 


	for (auto& i : v)
	{
		for (auto& j : i)
			j->Render();
	}

	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}
//--- �ٽñ׸��� �ݹ� �Լ�
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboarddown(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		_1 = !_1;
		_2 = false;
		_3 = false;
		break;

	case '2':
		break;

	case '3':
		break;

	case 't': // ���� �¿���
		light_onf = !light_onf;
		break;

	case 'y': // ī�޶� ���ǹ��� ����
		break;

	case 'Y': // ī�޶� ���ǹ��� ����
		break;

	case '+': // �ӵ�����
		timertime = max(timertime - 1, 0);
		break;

	case '-': // �ӵ�����
		timertime = min(timertime + 1, 200);
		break;

	case 'c': // ���� �� �ٲٱ�
		lightColor = { uidC(dre), uidC(dre), uidC(dre) };
		break;

	case 'r':
		Reset();
		break;
		
	case 'p': // ��������?
		proj = glm::mat4(1.0f);
		proj = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, -50.0f, 50.0f);
		break;

	case 'q': // ���α׷� ����
		exit(0);
		break;
	}
	glutPostRedisplay();
}

GLvoid Keyboardup(unsigned char key, int x, int y)
{
	switch (key)
	{
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	if (_1)
	{
		for (auto& i : v)
			for (auto& j : i)
				j->ani1();
	}

	for (auto& i : v)
		for (auto& j : i)
			j->Update();

	glutPostRedisplay();
	glutTimerFunc(timertime, TimerFunction, 1);
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		drag = true;

		glm::vec3 ray_origin = glm::unProject(glm::vec3(x, HEIGHT - y, 0.0f), view, proj, glm::vec4(0, 0, WIDTH, HEIGHT));
		glm::vec3 ray_direction = glm::normalize(glm::unProject(glm::vec3(x, HEIGHT - y, 1.0f), view, proj, glm::vec4(0, 0, WIDTH, HEIGHT)) - ray_origin);

		for (auto& i : v)
		{
			for (auto& j : i)
			{
				if (obb_ray(*j, ray_origin, ray_direction))
				{
					j->isSelected = true;
					break;
				}
			}
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		drag = false;
		for (auto& i : v)
		{
			for (auto& j : i)
			{
				j->isSelected = false;
			}
		}
	}
	mousept = { x, y };
	return GLvoid();
}

GLvoid Motion(int x, int y)
{
	if (drag)
	{
		//glm::vec3 ray_origin = glm::unProject(glm::vec3(x, HEIGHT - y, 0.0f), view, proj, glm::vec4(0, 0, WIDTH, HEIGHT));
		//glm::vec3 ray_direction = glm::normalize(glm::unProject(glm::vec3(x, HEIGHT - y, 1.0f), view, proj, glm::vec4(0, 0, WIDTH, HEIGHT)) - ray_origin);

		for (auto& i : v)
		{
			for (auto& j : i)
			{
				if (j->isSelected)
				{
					//pair<float, float> glpt = { WintoOpenGL({ x, y }).first - WintoOpenGL(mousept).first, WintoOpenGL({ x, y }).second - WintoOpenGL(mousept).second };
					j->Move(1, x - mousept.x);
				}
			}	
		}
		mousept = { x, y };
	}

	glutPostRedisplay();
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