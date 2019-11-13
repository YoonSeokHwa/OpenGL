#pragma once
#include<vector>

using namespace std;

struct Vector3D
{
	float x;
	float y;
	float z;
};
#define WIDTH 800
#define HEIGHT 600

void convertDeviceXYOpneglXY(int x, int y, float* ox, float* oy) { // 윈도우좌표를 OpenGL좌표로 변환
	int w = WIDTH;
	int h = HEIGHT;
	*ox = (float)((x - (float)w / 2.0)*(float)(1.0 / (float)(w / 2.0)));
	*oy = -(float)((y - (float)h / 2.0)*(float)(1.0 / (float)(h / 2.0)));
}


float direction[][2] = {
	{0,0.001},
	{0,-0.001},
	{-0.001,0},
	{0.001,0}
};

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

int triangleDirection[] = { UP,UP,UP,UP };

void MoveTriangle(vector<Vector3D>& positions)
{
	for (int i = 0; i < 4; ++i)
	{
		//cout << direction[triangleDirection[i]][0] << endl;
		
		positions[i * 3].y += direction[triangleDirection[i]][1];
		positions[i * 3 + 1].y += direction[triangleDirection[i]][1];
		positions[i * 3 + 2].y += direction[triangleDirection[i]][1];


		positions[i * 3].x += direction[triangleDirection[i]][0];
		positions[i * 3 + 1].x += direction[triangleDirection[i]][0];
		positions[i * 3 + 2].x += direction[triangleDirection[i]][0];


		if(positions[i * 3].y > 1.0f  && triangleDirection[i] ==UP)
		{
			
			triangleDirection[i] = RIGHT;
		}
		else if (positions[i * 3 + 2].x > 1.0f && triangleDirection[i] == RIGHT)
		{
			triangleDirection[i] = DOWN;
		}
		else if (positions[i * 3 + 1].y < -1 && triangleDirection[i] == DOWN)
		{
			triangleDirection[i] = LEFT;
		}
		else if (positions[i *3 + 1].x < -1 && triangleDirection[i] == LEFT)
		{
			triangleDirection[i] = UP;
		}
	}
}
void spckeycallback(int key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Keyboard(unsigned char key, int x, int y);