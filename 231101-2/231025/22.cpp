#include "stdafx.h"
#include "object.h"
#include "Robot.h"
#include "BB.h"

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
vector<Object> stage;
vector<Robot> r;
vector<Object> box; // ������ ���� ��
vector<Object> cyl; 

struct obs
{

};


void cameraRot(int n)
{
	cameraAngle[n] += 1.0f;
}

void Reset()
{
	stage.clear();
	r.clear();

	cameraPos = glm::vec3(0.0f, 0.0f, 120.0f); //--- ī�޶� ��ġ
	cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- ī�޶� �ٶ󺸴� ����
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- ī�޶� ���� ����
	view = glm::mat4(1.0f);
	cameraAngle = { 0.0f, 0.0f, 0.0f };


	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			stage.push_back({ "plane.obj", {5.0f, 0.1f, 5.0f}, {0.0f,0.0f,0.0f}, {(-5.0f * 5) + (5.0f * i) + 2.5f,-20.0f,(-5.0f * 5) + (5.0f * j) + 2.5f}});
		}
	}

	r.push_back({ glm::vec3{ 0.0f, 0.0f ,0.0f }, 1.0f });
	r.push_back({ glm::vec3{ 0.0f, 0.0f, 5.0f }, 0.5f });


	//for(int i = 0; i < 5; ++i)
	box.push_back({ "cube.obj", {5.0f, 5.0f, 5.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, -20.0f, 0.0f} });

	cyl.push_back({ "cylinder.obj", {5.0f, 100.0f, 5.0f}, {0.0f, 0.0f, 0.0f}, {-25.0f + 2.5f, -20.0f, -25.0f + 2.5f} });

	proj = glm::mat4(1.0f);
	proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 200.0f); //--- ���� ���� ����: fovy, aspect, near, far
	proj = glm::translate(proj, glm::vec3(0.0, 0.0, 30.0f));
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
	glutTimerFunc(50, TimerFunction, 1);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	
	glEnable(GL_DEPTH_TEST); 

	for (Object& i : stage)
	{
		i.Render(shaderProgramID);
		i.bb->Render(shaderProgramID);
	}

	for (Robot& i : r)
	{
		i.Render(shaderProgramID);
	}


	for (Object& i : box)
	{
		i.Render(shaderProgramID);
		i.bb->Render(shaderProgramID);
	}


	for (Object& i : cyl)
	{
		i.Render(shaderProgramID);
		i.bb->Render(shaderProgramID);
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
	case 'j': // ����
		r[0].Jump();
		break;

	case 'k':
		for (int i = 1; i < r.size(); ++i)
			r[i].Jump();
		break;

	case 'w':
		for (Robot& i : r)
		{
			i.RotationY(180.0f);
			i.dir.z = -1.0f;
		}
		break;

	case 'a':
		for (Robot& i : r)
		{
			i.RotationY(270.0f);
			i.dir.x = -1.0f;
		}
		break;

	case 's':
		for (Robot& i : r)
		{
			i.RotationY(0.0f);
			i.dir.z = 1.0f;
		}
		break;

	case 'd':
		for (Robot& i : r)
		{
			i.RotationY(90.0f);
			i.dir.x = 1.0f;
		}
		break;

	case 'x':
		cameraPos.x += 0.5f;
		cameraDirection.x += 0.5f;
		break;

	case 'X':
		cameraPos.x -= 0.5f;
		cameraDirection.x -= 0.5f;
		break;

	case 'y':
		break;

	case 'z':
		cameraPos.z += 0.5f;
		cameraDirection.z += 0.5f;
		break;

	case 'Z':
		cameraPos.z -= 0.5f;
		cameraDirection.z -= 0.5f;
		break;

	case '+':
		break;

	case '-':
		break;

	case 'p': // ��������?
		proj = glm::mat4(1.0f);
		proj = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, -50.0f, 50.0f);
		break;

	case 'P': // ��������?
		proj = glm::mat4(1.0f);
		proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 200.0f); //--- ���� ���� ����: fovy, aspect, near, far
		proj = glm::translate(proj, glm::vec3(0.0, 0.0, -10.0f));
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
	case 'w':
		for (Robot& i : r)
			i.dir.z = 0.0f;
		break;

	case 'a':
		for (Robot& i : r)
			i.dir.x = 0.0f;
		break;

	case's':
		for (Robot& i : r)
			i.dir.z = 0.0f;
		break;
		
	case 'd':
		for (Robot& i : r)
			i.dir.x = 0.0f;
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	for (Robot& i : r)
		i.Move();

	for (Robot& i : r)
		i.Update();

	for (Object& i : stage)
		i.Update();

	for (Object& j : box)
		j.Update();

	for (Object& i : cyl)
		i.Update();


	// �浹 üũ - �÷��̾�� ��
	for (Object& j : stage)
	{
		for (Robot& i : r)
		{
			for (int k = 0; k < 7; ++k)
			{
				if (obb(*i.GetBB(k), *j.bb))
				{
					while (obb(*i.GetBB(k), *j.bb))
					{
						i.origin.y += 0.1f;
						i.dir.y = 0.0f;
						i.Update();
					}
				}
			}
		}
	}
	
	bool b = false;
	// �浹 üũ - �÷��̾�� �ڽ�
	for (Object& j : box)
	{
		for (Robot& i : r)
		{
			for (int k = 0; k < 7; ++k)
			{
				if (obb(*i.GetBB(k), *j.bb) && i.origin.y > 0.0f)
				{
					b = true;
					while (obb(*i.GetBB(k), *j.bb))
					{
						i.origin.y += 0.1f;
						i.dir.y = 0.0f;
						i.Update();
					}
				}
			}
			
		}
		if (b)
			j.Move(1, -0.1f);
		else if (!b && j.bb->vCenterPos.y < -17.5f)
			j.Move(1, 0.1f);
	}

	//�浹üũ - �÷��̾�� ���
	for (Object& j : cyl)
	{
		for (Robot& i : r)
		{
			for (int k = 0; k < 7; ++k)
			{
				if (obb(*i.GetBB(k), *j.bb) && i.origin.y > 0.0f)
				{
					while (obb(*i.GetBB(k), *j.bb))
					{
						i.origin.x -= i.dir.x/ 10.0f;
						i.origin.z -= i.dir.z/ 10.0f;
						i.Update();
					}
				}
			}
		}
	}


	glutPostRedisplay();
	glutTimerFunc(50, TimerFunction, 1);
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