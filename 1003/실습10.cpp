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
GLUquadricObj *qobj;
using namespace std;

int WIN;
int drawMode = 0;
bool drawStyle = false;

GLuint ShaderProgramID;
GLuint triangleShaderProgramID;

vector<Vector3D> v;
vector<GLuint> vaoArray;
struct MOVE
{
	bool isMoveX = false;
	bool isMoveY = false;
	bool isMoveZ = false;
	glm::mat4 moveTx= glm::mat4(1.0f);
	glm::mat4 moveTy = glm::mat4(1.0f);
	glm::mat4 moveTz = glm::mat4(1.0f);

};

MOVE speare;
MOVE cone_z;
MOVE cone_y;
MOVE disk;
MOVE cylinder;

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
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutSpecialFunc(spckeycallback);
	glutKeyboardFunc(Keyboard);
	glutMainLoop(); // 이벤트 처리 시작 
}

GLvoid drawScene() // 콜백 함수: 출력 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//좌표계그리기
	unsigned int modelLocation = glGetUniformLocation(ShaderProgramID, "u_transform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
	glBegin(GL_LINE_LOOP);
	glVertex3f(1.0, 0.0, 0.0);// 시작점
	glVertex3f(-1.0, 0.0, 0.0);//끝점
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, 1.0, 0.0);// 시작점
	glVertex3f(0.0, -1.0, 0.0);
	glEnd();

	//3차원 도형그리기
	qobj = gluNewQuadric(); // 객체생성하기 
	//도형스타일 
	if (drawStyle == false) {
		gluQuadricDrawStyle(qobj, GLU_LINE); // 와이어
	}
	if(drawStyle == true){
		gluQuadricDrawStyle(qobj, GLU_FILL); //솔리드
	}

	gluQuadricNormals( qobj, GLU_SMOOTH ); //생략가능
	gluQuadricOrientation( qobj, GLU_OUTSIDE ); //생략가능 

	// 구--------------------------------------------------
	if (drawMode == 1) { 
		//움직인 상태
		if (speare.isMoveX == true) {
			unsigned int modelLocation = glGetUniformLocation(ShaderProgramID, "u_transform");  //---버텍스세이더에서모델변환위치가져오기 
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(speare.moveTx)));
		}
		if (speare.isMoveY == true) {
			unsigned int modelLocation = glGetUniformLocation(ShaderProgramID, "u_transform");  //---버텍스세이더에서모델변환위치가져오기 
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(speare.moveTy)));
		}
		if (speare.isMoveX == true && speare.isMoveY == true) {
			unsigned int modelLocation = glGetUniformLocation(ShaderProgramID, "u_transform");  //---버텍스세이더에서모델변환위치가져오기 
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(speare.moveTx * speare.moveTy)));
		}
		//초기 상태
		if (speare.isMoveX == false && speare.isMoveY == false) {
			unsigned int modelLocation = glGetUniformLocation(ShaderProgramID, "u_transform");  //---버텍스세이더에서모델변환위치가져오기 
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
		}
		gluSphere(qobj, 0.5, 50, 50); // 객체만들기
	}
	//원뿔 z축------------------------------------------------------------------------
	if (drawMode == 2) {
		glm::mat4 TR = glm::mat4(1.0f);
		//y축이동
		glm::mat4 Ty = glm::mat4(1.0f);
		Ty = glm::translate(Ty, glm::vec3(0.0, -0.5, 0.0));
		//y축 회전
		glm::mat4 Ry = glm::mat4(1.0f); //--- translation matrix  행렬 초기화
		Ry = glm::rotate(Ry, (float)glm::radians(-90.0), glm::vec3(1.0, 0.0, 0.0)); //y축 회전
		TR = Ty * Ry;
		unsigned int modelLocation = glGetUniformLocation(ShaderProgramID, "u_transform");  //---버텍스세이더에서모델변환위치가져오기 
		//초기 상태
		if (cone_z.isMoveX == false && cone_z.isMoveZ == false) {
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		}
		//움직인 상태
		if (cone_z.isMoveX == true) {
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR * cone_z.moveTx));
		}
		if (cone_z.isMoveZ == true) {
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR * cone_z.moveTz));
		}
		if (cone_z.isMoveZ == true && cone_z.isMoveX == true) {
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR *cone_z.moveTx * cone_z.moveTz));
		}
		gluCylinder(qobj, 0.5, 0.0, 1.0, 20, 8);

	}
	//원뿔 y축--------------------------------------------------------------
	if (drawMode == 3) { 
		glm::mat4 TR = glm::mat4(1.0f);
		//y축이동
		glm::mat4 Ty = glm::mat4(1.0f);
		Ty = glm::translate(Ty, glm::vec3(-0.5, 0.0, 0.0));
		//y축 회전
		glm::mat4 Ry = glm::mat4(1.0f); //--- translation matrix  행렬 초기화
		Ry = glm::rotate(Ry, (float)glm::radians(90.0), glm::vec3(0.0, 1.0, 0.0)); //y축 회전
		TR = Ty * Ry;
		unsigned int modelLocation = glGetUniformLocation(ShaderProgramID, "u_transform");  //---버텍스세이더에서모델변환위치가져오기 
		//초기 상태
		if (cone_y.isMoveY == false && cone_y.isMoveZ == false) {
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		}
		//움직인 상태
		if (cone_y.isMoveZ == true) {
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR * cone_y.moveTz));
		}
		if (cone_y.isMoveY == true) {
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR * cone_y.moveTy));
		}
		if (cone_y.isMoveZ == true && cone_y.isMoveY == true) {
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR* cone_y.moveTz * cone_y.moveTy));
		}
		gluCylinder(qobj, 0.5, 0.0, 1.0, 20, 8);

	}
	//디스크------------------------------------------------------------------
	if(drawMode == 4){ 
		//움직인 상태
		if (disk.isMoveX == true) {
			unsigned int modelLocation = glGetUniformLocation(ShaderProgramID, "u_transform");  //---버텍스세이더에서모델변환위치가져오기 
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(disk.moveTx)));
		}
		if (disk.isMoveY == true) {
			unsigned int modelLocation = glGetUniformLocation(ShaderProgramID, "u_transform");  //---버텍스세이더에서모델변환위치가져오기 
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(disk.moveTy)));
		}
		if (disk.isMoveX == true && disk.isMoveY == true) {
			unsigned int modelLocation = glGetUniformLocation(ShaderProgramID, "u_transform");  //---버텍스세이더에서모델변환위치가져오기 
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(disk.moveTx * disk.moveTy)));
		}
		unsigned int modelLocation = glGetUniformLocation(ShaderProgramID, "u_transform");  
		//초기화 상태
		if (disk.isMoveX == false && disk.isMoveY == false){
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
		}

		gluDisk(qobj, 0.1, 0.5, 20, 3);
	}
	//실린더------------------------------------------------------------------------------
	if(drawMode == 5){ 
		glm::mat4 TR = glm::mat4(1.0f);
		//y축이동
		glm::mat4 Ty = glm::mat4(1.0f);
		Ty = glm::translate(Ty, glm::vec3(0.0, 0.5, 0.0));
		//x축 회전
		glm::mat4 Rx = glm::mat4(1.0f); //--- translation matrix  행렬 초기화
		Rx = glm::rotate(Rx, (float)glm::radians(90.0), glm::vec3(1.0, 0.0, 0.0)); //x축 회전
		TR = Ty * Rx;
		unsigned int modelLocation = glGetUniformLocation(ShaderProgramID, "u_transform");  //---버텍스세이더에서모델변환위치가져오기 
		//초기 상태
		if (cylinder.isMoveZ == false && cylinder.isMoveY == false) {
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		}
		
		//움직인 상태
		if (cylinder.isMoveX == true) {
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR * cylinder.moveTx));
		}
		if (cylinder.isMoveZ == true) {
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR * cylinder.moveTz));
		}
		if (cylinder.isMoveZ == true && cylinder.isMoveX == true) {
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR *cylinder.moveTx * cylinder.moveTz));
		}
		gluCylinder(qobj, 0.5, 0.5, 1.0, 20, 8);
	}
	
	glutSwapBuffers();
	glutPostRedisplay();
}
void spckeycallback(int key, int x, int y) {
	switch (key)
	{
		case 100: //왼쪽 이동
			if (drawMode == 1) {
				speare.isMoveX = true;
				speare.moveTx += glm::translate(speare.moveTx, glm::vec3(-0.1, 0.0, 0.0));
			}
			if (drawMode == 2) {
				cone_z.isMoveX = true;
				cone_z.moveTx += glm::translate(cone_z.moveTx, glm::vec3(-0.1, 0.0, 0.0));
			}
			if (drawMode == 3) {
				cone_y.isMoveZ = true;
				cone_y.moveTz += glm::translate(cone_y.moveTz, glm::vec3(0.0, 0.0, -0.1));
			}
			if (drawMode == 4) {
				disk.isMoveX = true;
				disk.moveTx += glm::translate(disk.moveTx, glm::vec3(-0.1, 0.0, 0.0));
			}
			if (drawMode == 5) {
				cylinder.isMoveX = true;
				cylinder.moveTx += glm::translate(cylinder.moveTx, glm::vec3(-0.1, 0.0, 0.0));
			}
		break;

		case 102: //오른쪽 이동
			if (drawMode == 1) {
				speare.isMoveX = true;
				speare.moveTx += glm::translate(speare.moveTx, glm::vec3(0.1, 0.0, 0.0));
			}
			if (drawMode == 2) {
				cone_z.isMoveX = true;
				cone_z.moveTx += glm::translate(cone_z.moveTx, glm::vec3(0.1, 0.0, 0.0));
			}
			if (drawMode == 3) {
				cone_y.isMoveZ = true;
				cone_y.moveTz += glm::translate(cone_y.moveTz, glm::vec3(0.0, 0.0, 0.1));
			}
			if (drawMode == 4) {
				disk.isMoveX = true;
				disk.moveTx += glm::translate(disk.moveTx, glm::vec3(0.1, 0.0, 0.0));
			}
			if (drawMode == 5) {
				cylinder.isMoveX = true;
				cylinder.moveTx += glm::translate(cylinder.moveTx, glm::vec3(0.1, 0.0, 0.0));
			}
			break;

		case 101: //위쪽 이동
			if (drawMode == 1) {
				speare.isMoveY = true;
				speare.moveTy += glm::translate(speare.moveTy, glm::vec3(0.0, 0.1, 0.0));
			}
			if (drawMode == 2) {
				cone_z.isMoveZ = true;
				cone_z.moveTz += glm::translate(cone_z.moveTz, glm::vec3(0.0, 0.0, 0.1));
			}
			if (drawMode == 3) {
				cone_y.isMoveY = true;
				cone_y.moveTy += glm::translate(cone_y.moveTy, glm::vec3(0.0, 0.1, 0.0));
			}
			if (drawMode == 4) {
				disk.isMoveY = true;
				disk.moveTy += glm::translate(disk.moveTy, glm::vec3(0.0, 0.1, 0.0));
			}
			if (drawMode == 5) {
				cylinder.isMoveZ = true;
				cylinder.moveTz += glm::translate(cylinder.moveTz, glm::vec3(0.0, 0.0, -0.1));
			}
			break;

		case 103: //아래쪽 이동
			if (drawMode == 1) {
				speare.isMoveY = true;
				speare.moveTy += glm::translate(speare.moveTy, glm::vec3(0.0, -0.1, 0.0));
			}
			if (drawMode == 2) {
				cone_z.isMoveZ = true;
				cone_z.moveTz += glm::translate(cone_z.moveTz, glm::vec3(0.0, 0.0, -0.1));
			}
			if (drawMode == 3) {
				cone_y.isMoveY = true;
				cone_y.moveTy += glm::translate(cone_y.moveTy, glm::vec3(0.0, -0.1, 0.0));
			}
			if (drawMode == 4) {
				disk.isMoveY = true;
				disk.moveTy += glm::translate(disk.moveTy, glm::vec3(0.0, -0.1, 0.0));
			}
			if (drawMode == 5) {
				cylinder.isMoveZ = true;
				cylinder.moveTz += glm::translate(cylinder.moveTz, glm::vec3(0.0, 0.0, 0.1));
			}
			break;
	}
}

void Keyboard(unsigned char key, int x, int y)
{
	
	switch (key)
	{
		case '1': //구
			drawMode = 1;
			break;
		case '2': //원뿔 z축
			drawMode = 2;
			break;
		case '3': //원뿔 y축
			drawMode = 3;
			break;
		case '4'://디스크
			drawMode = 4;
			break;
		case'5'://실린더
			drawMode = 5;
			break;
		case'w': // 와이어/솔리드 모드
			if (drawStyle == false) {
				drawStyle = true;
			}
			else if (drawStyle == true) {
				drawStyle = false;
			}
			break;
	}
}

GLvoid Reshape(int w, int h) // 콜백 함수: 다시 그리기 
{
	glViewport(0, 0, w, h);
}



