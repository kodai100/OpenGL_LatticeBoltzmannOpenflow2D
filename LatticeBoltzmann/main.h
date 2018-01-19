#ifndef MAIN_H
#define MAIN_H

#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "lbm.h"
#include "particles.h"

using namespace std;

int frame;

void start();
void update();
void render();

// GLFW
GLFWwindow* initGLFWContext();
static void error_callback(int, const char*);
void key_callback(GLFWwindow*, int, int, int, int);
void mouse_callback(GLFWwindow*, int, int, int);

// GL
void initGLContext();




#endif
