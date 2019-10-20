#include <GL/glew.h> // �ʿ��� ������� include
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include"basic.h"

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

using namespace std;

int WIN;
GLuint ShaderProgramID;
GLuint triangleShaderProgramID;


vector<Vector3D> positions;
vector<Vector3D> colors;

int triangleIndex = 0;

char* filetobuf(const char *file) {
	FILE *fptr; long length; char *buf;
	fptr = fopen(file, "rb"); // Open file for reading 
	if (!fptr) // Return NULL on failure 
		return NULL; fseek(fptr, 0, SEEK_END); // Seek to the end of the file 
	length = ftell(fptr); // Find out how many bytes into the file we are 
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator 
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file 
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer 
	fclose(fptr); // Close the file
	buf[length] = 0; // Null terminator 
	return buf; // Return the buffer 
}

void InitTriangle()
{
	positions.push_back({ 0.6,0.6,0 });
	positions.push_back({ 0.3,0.0,0.0 });
	positions.push_back({ 0.9,0.0,0.0 });

	positions.push_back({ -0.6,0.6,0 });
	positions.push_back({ -0.9,0.0,0.0 });
	positions.push_back({ -0.3,0.0,0.0 });

	positions.push_back({ 0.5,-0.5,0 });
	positions.push_back({ 0.25,-1.0,0 });
	positions.push_back({ 0.75,-1.0,0 });

	positions.push_back({ -0.5,-0.5,0 });
	positions.push_back({ -0.75,-1.0,0 });
	positions.push_back({ -0.25,-1.0,0 });

	colors.push_back({ 1.0,0.0,0.0 });
	colors.push_back({ 1.0,0.0,0.0 });
	colors.push_back({ 1.0,0.0,0.0 });

	colors.push_back({ 1.0,0.0,0.0 });
	colors.push_back({ 1.0,0.0,0.0 });
	colors.push_back({ 1.0,0.0,0.0 });

	colors.push_back({ 1.0,1.0,0.0 });
	colors.push_back({ 1.0,1.0,0.0 });
	colors.push_back({ 1.0,1.0,0.0 });

	colors.push_back({ 0.0,1.0,1.0 });
	colors.push_back({ 0.0,1.0,1.0 });
	colors.push_back({ 0.0,1.0,1.0 });
}
void Mouse(int button, int state, int x, int y);

GLuint complie_shaders() {

	auto vertexsource = filetobuf("vertex.glsl");
	auto fragmentsource = filetobuf("pixel.glsl");

	//---���ؽ����̴��о������ϰ��������ϱ� 
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexsource, NULL);
	glCompileShader(vertexShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader �����Ͻ���\n" << errorLog << endl;
		return false;
	}

	//---�����׸�Ʈ���̴��о������ϰ��������ϱ�
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentsource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader �����Ͻ���\n" << errorLog << endl;
		return false;
	}
	ShaderProgramID = glCreateProgram(); //---���̴����α׷������
	glAttachShader(ShaderProgramID, vertexShader); // ���̴����α׷������ؽ����̴����̱� 
	glAttachShader(ShaderProgramID, fragmentShader); // ���̴����α׷��������׸�Ʈ���̴����̱�

	glBindAttribLocation(ShaderProgramID, 0, "in_Position");
	glBindAttribLocation(ShaderProgramID, 1, "in_Color");


	glLinkProgram(ShaderProgramID); // ���̴����α׷���ũ�ϱ�
	glDeleteShader(vertexShader); // ���̴����α׷�����ũ�Ͽ����̴���ü��ü�»������� 
	glDeleteShader(fragmentShader);
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result); // ���̴����߿���Ǿ�����üũ�ϱ� 
	if (!result) {
		glGetProgramInfoLog(triangleShaderProgramID, 512, NULL, errorLog);
		cerr << "ERROR: shader program �������\n" << errorLog << endl;
		return false;
	}
	glUseProgram(ShaderProgramID); //---����������̴����α׷�����ϱ� // �����������α׷�������ְ�, Ư�����α׷�������Ϸ��� // glUseProgram�Լ���ȣ���Ͽ������Ư�����α׷��������Ѵ�. // ����ϱ�������ȣ���Ҽ��ִ�. return ShaderProgramID;
}

void main(int argc, char** argv) // ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�

	srand(time(NULL));

	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(WIDTH, HEIGHT); // �������� ũ�� ����
	WIN = glutCreateWindow("Example1"); // ������ ����(������ �̸�)

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	complie_shaders();
	InitTriangle();
	glutMouseFunc(Mouse);
	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����


	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}

GLvoid drawScene() // �ݹ� �Լ�: ��� 
{

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // �⺻ ���
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	GLuint vao, vbo[2]; 
	GLchar *vertexsource, *fragmentsource; // �ҽ��ڵ����庯��
	GLuint vertexshader, fragmentshader; // ���̴�
	GLuint shaderprogram; // ���̴����α׷�
	// VAO �������ϰ��Ҵ��ϱ� 
	glGenVertexArrays (1, &vao);
	// VAO�����ε��ϱ� 
	glBindVertexArray(vao);
	// 2���� VBO�������ϰ��Ҵ��ϱ� 
	glGenBuffers(2, vbo);

	//--- 1��° VBO��Ȱ��ȭ�Ͽ����ε��ϰ�, ���ؽ��Ӽ� (��ǥ��)������ 
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	// ���� diamond �������ؽ������Ͱ������ۿ������Ѵ�.
	// triShape�迭�ǻ�����: 9 * float

	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(Vector3D), &positions[0], GL_STATIC_DRAW);

	// ��ǥ���� attribute �ε��� 0��������Ѵ�: ���ؽ��� 3* float 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// attribute �ε��� 0������밡���ϰ��� 
	glEnableVertexAttribArray(0);

	//---2��° VBO��Ȱ��ȭ�Ͽ����ε��ϰ�, ���ؽ��Ӽ� (����)������ 
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	// ���� colors�������ؽ������������Ѵ�. 
	// colors �迭�ǻ�����: 9 *float 
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(Vector3D), &colors[0], GL_STATIC_DRAW);

	// ������ attribute �ε��� 1��������Ѵ�: ���ؽ���3*float 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//cout << "test" << endl;
	// attribute �ε��� 1������밡���ϰ���. 
	glEnableVertexAttribArray(1);

	glUseProgram(ShaderProgramID);
	glDrawArrays(GL_TRIANGLES, 0,positions.size());
	glutSwapBuffers();
	glutPostRedisplay();
}

GLvoid Reshape(int w, int h) // �ݹ� �Լ�: �ٽ� �׸��� 
{
	glViewport(0, 0, w, h);
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		float ox;
		float oy;
		convertDeviceXYOpneglXY(x, y, &ox, &oy);

		cout << x<<"   "<<y << endl;
		cout << ox << "   " << oy << endl;

		positions[triangleIndex*3] = { ox,oy,0 };
		positions[triangleIndex*3+1] = { ox - 0.25f,oy-0.25f,0 };
		positions[triangleIndex*3+2] = { ox +0.25f , oy -0.25f,0 };

		triangleIndex += 1;
		triangleIndex %= 4;
	}
}