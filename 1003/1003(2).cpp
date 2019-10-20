#include <GL/glew.h> // 필요한 헤더파일 include
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

void main(int argc, char** argv) // 윈도우 출력하고 콜백함수 설정 
{ //--- 윈도우 생성하기

	srand(time(NULL));

	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	WIN = glutCreateWindow("Example1"); // 윈도우 생성(윈도우 이름)

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	//glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMainLoop(); // 이벤트 처리 시작 
}


GLvoid drawScene() // 콜백 함수: 출력 
{
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기

	// 그리기 부분 구현
	//--- 그리기 관련 부분이 여기에 포함된다
	
	glColor3f(color.in_r, color.in_g, color.in_b);
	glRectf(rect.x1, rect.y1, rect.x2, rect.y2);
	//cout << "태윤이이노오오오오옹ㅁ" << endl;
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
	glFlush(); // 화면에 출력하기
	glutPostRedisplay();
	return;
}

GLvoid Reshape(int w, int h) // 콜백 함수: 다시 그리기 
{
	glViewport(0, 0, w, h);
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if ((x <= 500 && x >= 300) && (y >= 225 && y <= 370)) { // 사각형 내부
			isInside = inside;
		}
		else {
			isInside = outside;
		}
	}
	else
		isInside = basic;
}
