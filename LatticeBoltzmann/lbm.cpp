#include "lbm.h"

const int ex[9]  = {0,  1,0,-1, 0,  1,-1,-1, 1};
const int ey[9]  = {0,  0,1, 0,-1,  1, 1,-1,-1};
const int inv[9] = {0, 3,4, 1, 2,  7, 8, 5, 6};
const float w[9] = {4.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/36.0, 1.0/36.0, 1.0/36.0, 1.0/36.0};

float df[2][NX*NX][9];	// double buffer

float2* U;
float R[NX*NX];
CellType FLAG[NX*NX];

float fx = 0.000014;

void initlbm(void) {

	// 全格子のdfを初速計算して
	for (int j = 0; j < NX*NX; j++)
		for (int k = 0; k < 9; k++) {
			//df[0][j][k] = df[1][j][k] = w[k];
			float rho = 1.0;
			float ux = 0.05;
			float uy = 0.0;

			df[0][j][k] = df[1][j][k] = w[k] * rho * (1.0f - (3.0f / 2.0f) * (ux*ux + uy*uy) + 3.0f * (ex[k] * ux + ey[k] * uy) + (9.0f / 2.0f) * (ex[k] * ux + ey[k] * uy) * (ex[k] * ux + ey[k] * uy));
		}

	for (int j = 0; j < NX*NX; j++)
		FLAG[j] = FLUID;

	// obstacle
	int R2 = 10;
	for(int i = (NX -R2) / 2; i < (NX - R2) / 2 + R2; i++)
		for(int j = (NX - R2)/2; j < (NX - R2) / 2 + R2 ; j++)
			FLAG[i + j * NX] = OBSTACLE;

	R2 = 30;
	for (int i = (NX - R2) / 2; i < (NX - R2) / 2 + R2; i++)
		for (int j = (NX - R2) / 2; j < (NX - R2) / 2 + R2; j++)
			FLAG[i + j * NX] = OBSTACLE;

	U = new float2[NX*NX];
}

// we use BGK single relaxation time model with no relaxation on the bounceback nodes Zou, He, 1997
void lbm() {

	// swap buffer
	static int c = 1;
	c = 1-c;

	// 毎フレームMPMの格子のフラグを参照する

	// 全格子について
	for(int i = 0 ; i < NX; i++)
		for (int j = 0; j < NX; j++) {

			float relaxation = 1.95;

			// 流体格子のみ計算
			if (FLAG[i + j * NX] == FLUID) {
				float rho = 0, ux = 0, uy = 0;

				// calculate density and velocity using neighbor cell
				for (int k = 0; k < 9; k++) {
					rho += df[c][i + j*NX][k];			// ------- (1)
					ux += df[c][i + j*NX][k] * ex[k];		// ------- (2)
					uy += df[c][i + j*NX][k] * ey[k];
				}

				ux /= rho;
				uy /= rho;

				
				// Boundary
				if (i == 0 || i == NX - 1) {
					rho = 1.0;
					ux = 0.05;
					uy = 0.0;
					relaxation = 1;
				}

				U[i + j*NX].x = ux;
				U[i + j*NX].y = uy;
				R[i + j*NX] = rho;

				//ux = ux + fx;

				// update neighbor df
				for (int k = 0; k < 9; k++) {
					int ip = (i + ex[k] + NX) % (NX);
					int jp = (j + ey[k] + NX) % (NX);

					// collision + streaming
					if (FLAG[ip + jp * NX] == OBSTACLE) {
						// Reflect
						// BGK model
						df[1 - c][i + j*NX][inv[k]] = (1 - relaxation) * df[c][i + j*NX][k] + relaxation* w[k] * rho * (1.0f - (3.0f / 2.0f) * (ux*ux + uy*uy) + 3.0f * (ex[k] * ux + ey[k] * uy) + (9.0f / 2.0f) * (ex[k] * ux + ey[k] * uy) * (ex[k] * ux + ey[k] * uy));
					}
					else
						df[1 - c][ip + jp*NX][k] = (1 - relaxation) * df[c][i + j*NX][k] + relaxation* w[k] * rho * (1.0f - (3.0f / 2.0f) * (ux*ux + uy*uy) + 3.0f * (ex[k] * ux + ey[k] * uy) + (9.0f / 2.0f) * (ex[k] * ux + ey[k] * uy) * (ex[k] * ux + ey[k] * uy));
				}
			}
		}
}


void gdraw() {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	for (int i = 0; i < NX; i++)
		for (int j = 0; j < NX; j++) {

			if(FLAG[i + j * NX] == OBSTACLE) glColor4f(1, 0, 1, 1);
			else glColor4f(U[i + j*NX].x * 5 + 0.5, U[i + j*NX].y * 5 + 0.5, 0, 1);

			glBegin(GL_POLYGON);
			glVertex2d((i / (float)NX), (j / (float)NX));
			glVertex2d((i / (float)NX), ((j + 1) / (float)NX));
			glVertex2d(((i + 1) / (float)NX), ((j + 1) / (float)NX));
			glVertex2d(((i + 1) / (float)NX), (j / (float)NX));
			glEnd();

		}
	
}


