#ifndef LBM_H
#define LBM_H

#include <GLFW/glfw3.h>
#define NX 100

typedef struct float2 {
	float x, y;

	float2() {}
	float2(float x, float y) {
		x = x;
		y = y;
	}
} float2;


enum CellType { OBSTACLE, FLUID };

class LatticeBoltzmann {
public:

	const int ex[9] = { 0,  1,0,-1, 0,  1,-1,-1, 1 };
	const int ey[9] = { 0,  0,1, 0,-1,  1, 1,-1,-1 };
	const int inv[9] = { 0, 3,4, 1, 2,  7, 8, 5, 6 };
	const float w[9] = { 4.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 36.0f, 1.0f / 36.0f, 1.0f / 36.0f, 1.0f / 36.0f };

	float df[2][NX*NX][9];	// double buffer

	float2* U;
	float R[NX*NX];
	CellType FLAG[NX*NX];

	float fx = 0.000014;

	LatticeBoltzmann();
	LatticeBoltzmann(const LatticeBoltzmann& orig);
	~LatticeBoltzmann();

	void update();
	void draw();
};
#endif

