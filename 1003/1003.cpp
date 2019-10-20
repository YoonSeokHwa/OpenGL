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

using namespace std;

int WIN;
bool isColorChange = false;
clock_t start;

struct Color
{
	float r;
	float g;
	float b;
	float a;
};

Color color = {1,1,1,1};

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		
		case 'r':
			color = { 1,0,0,1 };
			break;

		case 'g':
			color = { 0,1,0,1 };
			break;

		case 'b':
			color = { 0,0,1,1 };
			break;

		case 'a'://���� ��
			color = { rand() / (float)RAND_MAX,rand()/(float)RAND_MAX, rand() / (float)RAND_MAX,1 };
			break;

		case 'w':
			color = { 1,1,1,1 };
			break;

		case 'k':
			color = { 0,0,0,1 };
			break;

		case's': //Ÿ�̸� ����
			
			isColorChange = false;
			break;

		case 't'://Ÿ�̸Ӹ� �����Ͽ� Ư�� �ð����� ���������� ��� �ٲ�� �Ѵ�.
			if(isColorChange!=true){
				start = clock();
				isColorChange = true;
			}
			break;

		case 'q': 
			glutDestroyWindow(WIN);
			break;

	}
}


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
	//glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutKeyboardFunc(Keyboard); // Ű����
	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}



GLvoid drawScene() // �ݹ� �Լ�: ��� 
{ 
	glClearColor(color.r, color.g,color.b,color.a); // �⺻ ���
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	// �׸��� �κ� ����
	//--- �׸��� ���� �κ��� ���⿡ ���Եȴ�
	
	if (isColorChange == true) {
		clock_t end = clock();
		cout << start <<"////"<< end << endl;

		if(end-start>500 ) {
			cout << end - start << endl;
			start = clock();
			color = { rand() / (float)RAND_MAX,rand() / (float)RAND_MAX, rand() / (float)RAND_MAX,1 };
		}
		
	}

	glFlush(); // ȭ�鿡 ����ϱ�
	glutPostRedisplay();
	return;
}

GLvoid Reshape(int w, int h) // �ݹ� �Լ�: �ٽ� �׸��� 
{
	glViewport(0, 0, w, h);
}