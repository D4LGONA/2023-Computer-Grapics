#include "stdafx.h"
#include "Object.h"
#include "Rect.h"
#include "BB.h"
#include "Sphere.h"

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
bool drag = false;
vector<Object> stage;
vector<Sphere> spheres;
vector<Rect> rects;
Object* ground;
Object* Left = nullptr;
Object* Right = nullptr;

bool _y = false;

glm::vec3 origin = { 0.0f, 0.0f, 0.0f };

void cameraRot(int n)
{
	cameraAngle[n] += 1.0f;
}

void Reset()
{
	stage.clear();

	cameraPos = glm::vec3(0.0f, 0.0f, 120.0f); //--- ī�޶� ��ġ
	cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- ī�޶� �ٶ󺸴� ����
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- ī�޶� ���� ����
	view = glm::mat4(1.0f);
	cameraAngle = { 0.0f, 0.0f, 0.0f };

	stage.push_back({ "plane.obj", {40.0f, 0.1f, 40.0f}, {0.0f,0.0f,0.0f}, {0.0f,-20.0f,0.0f} });
	stage.push_back({ "plane.obj", {40.0f, 0.1f, 40.0f}, {0.0f,0.0f,180.0f}, {0.0f,20.0f,0.0f} });
	stage.push_back({ "plane.obj", {40.0f, 0.1f, 40.0f}, {0.0f,0.0f,90.0f}, {20.0f,0.0f,0.0f} });
	stage.push_back({ "plane.obj", {40.0f, 0.1f, 40.0f}, {0.0f,0.0f,-90.0f}, {-20.0f,0.0f,0.0f} });
	stage.push_back({ "plane.obj", {40.0f, 0.1f, 40.0f}, {90.0f,0.0f,0.0f}, {0.0f,0.0f,-20.0f} });

	rects.push_back({ "cube.obj" , { 2.0f, 2.0f, 2.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 10.0f } });
	rects.push_back({ "cube.obj" , { 4.0f, 4.0f, 4.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } });
	rects.push_back({ "cube.obj" , { 6.0f, 6.0f, 6.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -10.0f } });

	for(int i = 0; i < 20; ++i)
		spheres.push_back({ "sphere.obj", {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {uidC(dre), uidC(dre), uidC(dre)} });

	proj = glm::mat4(1.0f);
	proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 200.0f); //--- ���� ���� ����: fovy, aspect, near, far
	proj = glm::translate(proj, glm::vec3(0.0, 0.0, 30.0f));

	ground = &stage[0];
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

	for (Rect& i : rects)
	{
		i.Render(shaderProgramID);
		i.bb->Render(shaderProgramID);
	}

	for (Sphere& i : spheres)
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
	case'y':
		_y = !_y;
		
		break;

	case 'z':
		cameraPos.z += 1.0f;
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
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	if(_y)
	{
		for (Object& i : stage)
		{
			i.origin = { 0.0f, 0.0f, 0.0f };
			i.rotBy = { 0.0f, 0.0f, 0.0f };
			i.rotByAngle.z += 1.0f;
		}
		for (Rect& i : rects)
		{
			if (Left != nullptr)
				i.Move(0, 0.5f);
			if (Right != nullptr)
				i.Move(0, -0.5f);
		}
	}


	vector<glm::vec3> tmp;
	for (Object& j : stage)
		tmp.push_back(j.bb->vCenterPos);
	auto t = min_element(stage.begin(), stage.end(), [](const Object& a, const Object& b) {return a.bb->vCenterPos.y < b.bb->vCenterPos.y; });
	
	//if (obb(*rects[0].bb, *(*t).bb))
		ground = &(*t);

	for (Rect& i : rects)
	{
		i.MoveRect();
	}

	for (Object& i : stage)
		i.Update();

	for (Rect& i : rects)
		i.Update();

	for (Sphere& i : spheres)
		i.Update();

	for (Object& i : stage)
	{
		for (Rect& j : rects)
		{
			if (obb(*i.bb, *j.bb))
			{
				if (&i == ground) // ���̶� �ε�������
				{
					while (obb(*i.bb, *j.bb))
					{
						j.Move(1, 0.1f);
						j.speed.y = 0.0f;
						j.Update();
					}
					j.SetRot(2, (i.rotByAngle.z + i.GetRot().z));
				}
				else
				{
					if (i.bb->vCenterPos.x < j.bb->vCenterPos.x)
					{
						Left = &i;
						Right = nullptr;
					}
					else
					{
						Right = &i;
						Left = nullptr;
					}

					j.speed.x *= -0.5f;
					if (abs(j.speed.x) <= 0.01f)
					{
						j.speed.x = 0.0f;
						j.isStop = true;
					}
				}
			}
		}

		for (Sphere& j : spheres)
		{
			if (j.ischecked)
			{
				j.ischecked = !j.ischecked;
				continue;
			}
			if (obb(*i.bb, *j.bb) && !j.ischecked)
			{
				j.ischecked = true;
				j.dir = (glm::vec3{ 2 * i.bb->vAxisDir[1].x, 2 * i.bb->vAxisDir[1].y, 2 * i.bb->vAxisDir[1].z } - j.dir) / 1.1f;
				j.dir /= glm::length(j.dir);
				j.dir.z *= -1.0f;
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
		for (Object& i : stage)
		{
			i.origin = { 0.0f, 0.0f, 0.0f };
			i.rotBy = { 0.0f, 0.0f, 0.0f };
			i.rotByAngle.z += ((x - mousept.x)/ 10.0f);
			for (Rect& j : rects)
			{
				//j.SetRot(2, (i.rotByAngle.z + i.GetRot().z));

				if (obb(*i.bb, *j.bb))
				{
					if (&i == ground) // ���̶� �ε�������
					{
						while (obb(*i.bb, *j.bb))
						{
							j.Move(1, 0.1f);
							j.speed.y = 0.0f;
							j.Update();
						}
						j.SetRot(2, (i.rotByAngle.z + i.GetRot().z));
					}
				}
			}
		}


		mousept = { x,y };

		for (Rect& i : rects)
			i.isStop = false;
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