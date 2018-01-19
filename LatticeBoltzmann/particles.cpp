
#include <math.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include "lbm.h"
#include "particles.h"

LBMParticle::LBMParticle(LatticeBoltzmann* boltzmann)
{

	this->boltzmann = boltzmann;

	par = new Particle[nparmax];
	initPar = new Particle[nparmax];
	initposy = new float[nparmax];

	for ( int k=0; k<nparmax; k++)
	{
		while (true) {
			float x = rand() / float(RAND_MAX);
			float y = rand() / float(RAND_MAX);

			if (boltzmann->FLAG[(int)(y * NX) + (int)(x*NX) * NX] == OBSTACLE) continue;

			par[k].x = x;
			par[k].y = y;

			initPar[k].x = x;
			initPar[k].y = y;

			break;
		}
	}
}

LBMParticle::~LBMParticle() {
	delete[] par;
	delete[] initPar;
	delete[] initposy;
}

const float LBMParticle::BilinearInterpolation(float x,float y,int x1,int x2,int y1,int y2,float f11,float f21,float f22,float f12) const{
	// @Numerical Recipes
	float t = (x-x1) / (x2 - x1);
	float u = (y-y1) / (y2 - y1);
	return (1-t)*(1-u)*f11 + t*(1-u)*f21 + t*u*f22 + (1-t)*u*f12;
}

void LBMParticle::movepar(float dt)
{
	int i,j,k;
	int ip,jp;
	float x,y;

	for ( k=0; k<nparmax; k++)
	{
		x = par[k].x;
		y = par[k].y;
		
		i = int(x * (NX -1) + NX) % (NX);	// x=0 => i = 0, x=1 => L-2
		j = int(y * (NX -1) + NX) % (NX);
		
		ip = int(x * (NX -1) + NX + 1 ) % (NX);
		jp = int(y * (NX -1)+ NX + 1 ) % (NX);
		
		x = x + BilinearInterpolation(x*(NX-1),y*(NX -1), i,ip, j,jp, boltzmann->U[ i+j*NX ].x, boltzmann->U[ip+j*NX].x, boltzmann->U[ip+jp*NX].x, boltzmann->U[i+jp*NX].x) * dt;
		y = y + BilinearInterpolation(x*(NX-1),y*(NX -1), i,ip, j,jp, boltzmann->U[ i+j*NX ].y, boltzmann->U[ip+j*NX].y, boltzmann->U[ip+jp*NX].y, boltzmann->U[i+jp*NX].y) * dt;
		
		if(x<0) x = 1+x;
		if(y<0)	y = 1+y;
		if (x > 1) {
			x = x - 1;
			y = initPar[k].y;
		}
		if (y > 1)	y-1;	//
		par[k].x	= x;
		par[k].y	= y;
	}
}

void LBMParticle::draw(void)
{
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable (GL_POINT_SMOOTH);
	glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);

	glPointSize(2);
	glColor4f(0,1,1,1);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2,GL_FLOAT,0,par);
	glDrawArrays(GL_POINTS,0,nparmax);
	glDisableClientState(GL_VERTEX_ARRAY);
}
