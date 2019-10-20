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

		case 'a'://랜덤 색
			color = { rand() / (float)RAND_MAX,rand()/(float)RAND_MAX, rand() / (float)RAND_MAX,1 };
			break;

		case 'w':
			color = { 1,1,1,1 };
			break;

		case 'k':
			color = { 0,0,0,1 };
			break;

		case's': //타이머 종료
			
			isColorChange = false;
			break;

		case 't'://타이머를 설정하여 특정 시간마다 렌덤색으로 계속 바뀌게 한다.
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
	//glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard); // 키보드
	glutMainLoop(); // 이벤트 처리 시작 
}



GLvoid drawScene() // 콜백 함수: 출력 
{ 
	glClearColor(color.r, color.g,color.b,color.a); // 기본 흰색
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기

	// 그리기 부분 구현
	//--- 그리기 관련 부분이 여기에 포함된다
	
	if (isColorChange == true) {
		clock_t end = clock();
		cout << start <<"////"<< end << endl;

		if(end-start>500 ) {
			cout << end - start << endl;
			start = clock();
			color = { rand() / (float)RAND_MAX,rand() / (float)RAND_MAX, rand() / (float)RAND_MAX,1 };
		}
		
	}

	glFlush(); // 화면에 출력하기
	glutPostRedisplay();
	return;
}

GLvoid Reshape(int w, int h) // 콜백 함수: 다시 그리기 
{
	glViewport(0, 0, w, h);
}