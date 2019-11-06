#include <GL/glew.h> // 필요한 헤더파일 include
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

struct Circle
{
	double m_x;
	double m_y;
	float m_rad;
	bool isInfinite = false;
};

vector<Vector3D> v;
vector<GLuint> vaoArray;
vector<Circle> circles;

bool click = false;
double mouseX, mouseY;
bool drawmode = false; // 선
double increase = 0;
float rad = 0.1;


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

	//---버텍스세이더읽어저장하고컴파일하기 
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexsource, NULL);
	glCompileShader(vertexShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader 컴파일실패\n" << errorLog << endl;
		return false;
	}

	//---프래그먼트세이더읽어저장하고컴파일하기
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentsource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader 컴파일실패\n" << errorLog << endl;
		return false;
	}
	ShaderProgramID = glCreateProgram(); //---세이더프로그램만들기
	glAttachShader(ShaderProgramID, vertexShader); // 세이더프로그램에버텍스세이더붙이기 
	glAttachShader(ShaderProgramID, fragmentShader); // 세이더프로그램에프래그먼트세이더붙이기

	glBindAttribLocation(ShaderProgramID, 0, "in_Position");
	glBindAttribLocation(ShaderProgramID, 1, "in_Color");


	glLinkProgram(ShaderProgramID); // 세이더프로그램링크하기
	glDeleteShader(vertexShader); // 세이더프로그램에링크하여세이더객체자체는삭제가능 
	glDeleteShader(fragmentShader);
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result); // 세이더가잘연결되었는지체크하기 
	if (!result) {
		glGetProgramInfoLog(triangleShaderProgramID, 512, NULL, errorLog);
		cerr << "ERROR: shader program 연결실패\n" << errorLog << endl;
		return false;
	}
	glUseProgram(ShaderProgramID); //---만들어진세이더프로그램사용하기 // 여러개의프로그램만들수있고, 특정프로그램을사용하려면 // glUseProgram함수를호출하여사용할특정프로그램을지정한다. // 사용하기직전에호출할수있다. return ShaderProgramID;
}

void main(int argc, char** argv) // 윈도우 출력하고 콜백함수 설정 
{ //--- 윈도우 생성하기

	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(WIDTH, HEIGHT); // 윈도우의 크기 지정
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

	complie_shaders();
	glutMouseFunc(Mouse);
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard);
	glutMainLoop(); // 이벤트 처리 시작 
}
GLvoid drawScene() // 콜백 함수: 출력 
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // 기본 흰색
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	
	GLuint  vbo[1];
	GLchar *vertexsource, *fragmentsource; // 소스코드저장변수
	GLuint vertexshader, fragmentshader; // 세이더
	GLuint shaderprogram; // 세이더프로그램

	for (int j = 0; j < circles.size(); j++) {
		if (circles[j].m_rad > 0.4 && circles[j].isInfinite == false) {
			circles[j].m_rad = 0.1;
		}
		for (int i = j*36; i < (j*36)+36; i++)
		{
			v[i].x = (float)((circles[j].m_rad * cos((i * 10) * 3.141592 / 180)) + circles[j].m_x);
			v[i].y = (float)((circles[j].m_rad * sin((i * 10) * 3.141592 / 180)) + circles[j].m_y);
		}
		circles[j].m_rad += 0.0001;
		

	}
	for (int i = 0; i < vaoArray.size(); i++)
	{
		// VAO 를지정하고할당하기 
		glGenVertexArrays(1, &vaoArray[i]);
		// VAO를바인드하기 
		glBindVertexArray(vaoArray[i]);
		// 2개의 VBO를지정하고할당하기 
		glGenBuffers(1, vbo);

		//--- 1번째 VBO를활성화하여바인드하고, 버텍스속성 (좌표값)을저장 
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

		// 변수 diamond 에서버텍스데이터값을버퍼에복사한다.
		// triShape배열의사이즈: 9 * float

		glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(Vector3D), &v[i*36], GL_STATIC_DRAW);


		// 좌표값을 attribute 인덱스 0번에명시한다: 버텍스당 3* float 
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// attribute 인덱스 0번을사용가능하게함 
		glEnableVertexAttribArray(0);

		glUseProgram(ShaderProgramID);
		if (drawmode == true) {
			glDrawArrays(GL_POINTS, 0, 36); // 점
		}
		else if (drawmode == false) {
			glDrawArrays(GL_LINE_LOOP, 0, 36); //선
		}
	}

	glutSwapBuffers();
	glutPostRedisplay();
	glFlush();
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {

	case '1': //점으로 그리기
		drawmode = true;
		break;

	case '2': //선으로 그리기
		drawmode = false;
		break;
	}
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//좌표계변환
		float ox;
		float oy;
		convertDeviceXYOpneglXY(x, y, &ox, &oy);
		mouseX = ox;
		mouseY = oy;

		//원 그리기

		if (vaoArray.size() < 10)
		{
			if (rand() % 10 < 7)
			{
				circles.push_back({ mouseX,mouseY,rad, false });
			}
			else
			{
				circles.push_back({ mouseX,mouseY,rad, true });
			}
	
			for (int i = 0; i < 36; i++)
			{
				v.push_back({ (float)((rad * cos((i * 10) * 3.141592 / 180)) + mouseX) ,(float)((rad * sin((i * 10) * 3.141592 / 180)) + mouseY) ,0 });
			}
			vaoArray.push_back(0);
		}
	}

}

GLvoid Reshape(int w, int h) // 콜백 함수: 다시 그리기 
{
	glViewport(0, 0, w, h);
}



