#pragma once
#include<vector>

struct Vector3D
{
	float x;
	float y;
	float z;
};
#define WIDTH 800
#define HEIGHT 600

void convertDeviceXYOpneglXY(int x, int y, float* ox, float* oy) {
	int w = WIDTH;
	int h = HEIGHT;
	*ox = (float)(x - (float)(w / 2.0)*(float)(1.0 / (float)(w / 2.0)));
	*oy = -(float)(y - (float)(h / 2.0)*(float)(1.0 / (float)(h / 2.0)));
}