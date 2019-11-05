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

void Mouse(int button, int state, int x, int y);
void Keyboard(unsigned char key, int x, int y);

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


bool click = false;
double mouseX, mouseY;
bool drawmode = false; // ��
double increase = 0;
int click_count = 0;

vector<Vector3D> v;

void main(int argc, char** argv) // ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�

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
	glutMouseFunc(Mouse);
	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutKeyboardFunc(Keyboard);

	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}
GLvoid drawScene() // �ݹ� �Լ�: ��� 
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // �⺻ ���
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	float rad = 0.1;
	if(click==true){
		for (int i =0; i <36; i++)
		{
			v.push_back({ (float)((rad * cos((i * 10) * 3.141592 / 180)) + mouseX) ,(float)((rad * sin((i * 10) * 3.141592 / 180)) + mouseY) ,0});
		}
		for (int j = 0; j < click_count; ++j) {

			GLuint vao, vbo[1];
			GLchar *vertexsource, *fragmentsource; // �ҽ��ڵ����庯��
			GLuint vertexshader, fragmentshader; // ���̴�
			GLuint shaderprogram; // ���̴����α׷�
			// VAO �������ϰ��Ҵ��ϱ� 
			glGenVertexArrays(1, &vao);
			// VAO�����ε��ϱ� 
			glBindVertexArray(vao);
			// 2���� VBO�������ϰ��Ҵ��ϱ� 
			glGenBuffers(1, vbo);

			//--- 1��° VBO��Ȱ��ȭ�Ͽ����ε��ϰ�, ���ؽ��Ӽ� (��ǥ��)������ 
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

			// ���� diamond �������ؽ������Ͱ������ۿ������Ѵ�.
			// triShape�迭�ǻ�����: 9 * float

			glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(Vector3D), &v[j* 36], GL_STATIC_DRAW);


			// ��ǥ���� attribute �ε��� 0��������Ѵ�: ���ؽ��� 3* float 
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			// attribute �ε��� 0������밡���ϰ��� 
			glEnableVertexAttribArray(0);

			glUseProgram(ShaderProgramID);
			if (drawmode == true) {
				glDrawArrays(GL_POINTS, 0, v.size()); // ��
			}
			else if (drawmode == false) {
				glDrawArrays(GL_LINE_LOOP, 0, v.size()); //��
			}
		}
		glFlush();
	}
	/*drawCircle();*/
	glutSwapBuffers();
	glutPostRedisplay();

}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {

	case '1': //������ �׸���
		drawmode = true;
		break;

	case '2': //������ �׸���
		drawmode = false;
		break;
	}
}
void Mouse(int button, int state, int x, int y)
{
	click = true;

	
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		click_count += 1;
		float ox;
		float oy;
		convertDeviceXYOpneglXY(x, y, &ox, &oy);
		mouseX = ox;
		mouseY = oy;

	}
}

GLvoid Reshape(int w, int h) // �ݹ� �Լ�: �ٽ� �׸��� 
{
	glViewport(0, 0, w, h);
}



