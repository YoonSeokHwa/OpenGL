#include <GL/glew.h> // �ʿ��� ������� include
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <conio.h>
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

#define inside 0
#define outside 1
#define basic 2

using namespace std;

int WIN;
int isInside = basic;

clock_t start;

struct Color
{
	float r;
	float g;
	float b;
	float a;

	float in_r;
	float in_g;
	float in_b;
};

struct Rect
{
	float x1;
	float y1;
	float x2;
	float y2;
};
Rect rect = { -0.25,-0.25,0.25,0.25 };
Color color = { 0,0,0,0,1,1,1 };

void Mouse(int button, int state, int x, int y);

void main(int argc, char** argv) // ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�

	srand(time(NULL));

	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(800, 600); // �������� ũ�� ����
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

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	//glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}


GLvoid drawScene() // �ݹ� �Լ�: ��� 
{
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	// �׸��� �κ� ����
	//--- �׸��� ���� �κ��� ���⿡ ���Եȴ�
	
	glColor3f(color.in_r, color.in_g, color.in_b);
	glRectf(rect.x1, rect.y1, rect.x2, rect.y2);
	//cout << "�������̳���������ˤ�" << endl;
	if (isInside == inside) {
		color.in_r = rand()/(float)RAND_MAX;
		color.in_g = rand() / (float)RAND_MAX;
		color.in_b = rand() / (float)RAND_MAX;
	}
	else if(isInside == outside) {
		color.r = rand() / (float)RAND_MAX;
		color.g = rand() / (float)RAND_MAX;
		color.b = rand() / (float)RAND_MAX;
	}
	glFlush(); // ȭ�鿡 ����ϱ�
	glutPostRedisplay();
	return;
}

GLvoid Reshape(int w, int h) // �ݹ� �Լ�: �ٽ� �׸��� 
{
	glViewport(0, 0, w, h);
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if ((x <= 500 && x >= 300) && (y >= 225 && y <= 370)) { // �簢�� ����
			isInside = inside;
		}
		else {
			isInside = outside;
		}
	}
	else
		isInside = basic;
}
