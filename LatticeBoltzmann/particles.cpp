
#include <math.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include "lbm.h"
#include "particles.h"

//#define PC 2
//float par[nparmax*PC];

Particle par[nparmax];
Particle initPar[nparmax];

float initposy[nparmax];

void initpar(void)
{
	for ( int k=0; k<nparmax; k++)
	{
		while (true) {
			float x = rand() / float(RAND_MAX);
			float y = rand() / float(RAND_MAX);

			if (FLAG[(int)(y * NX) + (int)(x*NX) * NX] == OBSTACLE) continue;

			par[k].x = x;
			par[k].y = y;

			initPar[k].x = x;
			initPar[k].y = y;

			break;
		}
	}
}

float BilinearInterpolation(float x,float y,int x1,int x2,int y1,int y2,float f11,float f21,float f22,float f12)
{
	// @Numerical Recipes
	float t = (x-x1) / (x2 - x1);
	float u = (y-y1) / (y2 - y1);
	return (1-t)*(1-u)*f11 + t*(1-u)*f21 + t*u*f22 + (1-t)*u*f12;
}

void movepar(float dt)
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
		
		x = x + BilinearInterpolation(x*(NX-1),y*(NX -1), i,ip, j,jp, U[ i+j*NX ].x,U[ip+j*NX].x,U[ip+jp*NX].x,U[i+jp*NX].x) * dt;
		y = y + BilinearInterpolation(x*(NX-1),y*(NX -1), i,ip, j,jp, U[ i+j*NX ].y, U[ip+j*NX].y, U[ip+jp*NX].y, U[i+jp*NX].y) * dt;
		
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

void draw(void)
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