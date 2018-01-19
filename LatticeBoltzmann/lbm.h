#ifndef LBM_H
#define LBM_H

#include <GLFW/glfw3.h>

void lbm();
void initlbm();
void gdraw();

#define NX 100

typedef struct float2{
	float x, y;

	float2() {}
	float2(float x, float y) {
		x = x;
		y = y;
	}
} float2;


enum CellType { OBSTACLE, FLUID };

extern CellType FLAG[];

extern float2* U;
extern float R[];

extern float df[2][NX*NX][9];
extern const float w[9];
#endif

