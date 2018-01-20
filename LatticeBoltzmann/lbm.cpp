#include "lbm.h"


LatticeBoltzmann::LatticeBoltzmann() {

	// 全格子のdfを初速計算して
	for (int j = 0; j < NX*NY; j++)
		for (int k = 0; k < 9; k++) {
			//df[0][j][k] = df[1][j][k] = w[k];
			float rho = 1.0;
			float ux = 0.05;
			float uy = 0.0;

			df[0][j][k] = df[1][j][k] = w[k] * rho * (1.0f - (3.0f / 2.0f) * (ux*ux + uy*uy) + 3.0f * (ex[k] * ux + ey[k] * uy) + (9.0f / 2.0f) * (ex[k] * ux + ey[k] * uy) * (ex[k] * ux + ey[k] * uy));
		}

	for (int j = 0; j < NX*NY; j++)
		FLAG[j] = FLUID;

	// obstacle
	int R2 = 10;
	for(int i = (NX - R2) / 4; i < (NX - R2) / 4 + 5; i++)
		for(int j = 0; j < (NY - R2) / 3 + R2 ; j++)
			FLAG[i + j * NX] = OBSTACLE;

	U = new Vector2f[NX*NY];
}

LatticeBoltzmann::LatticeBoltzmann(const LatticeBoltzmann& orig) {}

LatticeBoltzmann::~LatticeBoltzmann() {
	delete U;
}

// we use BGK single relaxation time model with no relaxation on the bounceback nodes Zou, He, 1997
void LatticeBoltzmann::update() {

	// swap buffer
	static int c = 1;
	c = 1-c;

	// 毎フレームMPMの格子のフラグを参照する

	// 全格子について
	for(int i = 0 ; i < NX; i++)
		for (int j = 0; j < NY; j++) {

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

				// Boundary
				if (j == 0 || j == NY - 1) {
					rho = 1.0;
					ux = 0.05;
					uy = 0.0;
					relaxation = 1;
				}

				U[i + j*NX][0] = ux;
				U[i + j*NX][1] = uy;
				R[i + j*NX] = rho;

				//ux = ux + fx;

				// update neighbor df
				for (int k = 0; k < 9; k++) {
					int ip = (i + ex[k] + NX) % (NX);
					int jp = (j + ey[k] + NY) % (NY);

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


void LatticeBoltzmann::draw() {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	for (int i = 0; i < NX; i++)
		for (int j = 0; j < NY; j++) {

			if(FLAG[i + j * NX] == OBSTACLE) glColor4f(0, 0, 0, 1);
			else glColor4f(U[i + j*NX][0] * 5 + 0.5, U[i + j*NX][1] * 5 + 0.5, 0, 1);

			glBegin(GL_POLYGON);
			glVertex2d((i / (float)NX), (j / (float)NY));
			glVertex2d((i / (float)NX), ((j + 1) / (float)NY));
			glVertex2d(((i + 1) / (float)NX), ((j + 1) / (float)NY));
			glVertex2d(((i + 1) / (float)NX), (j / (float)NY));
			glEnd();

		}
	
}


