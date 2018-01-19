#include "main.h"

float framerate = 60;
float secPerFrame = 1 / framerate;

void update(void)
{
	
	cout << frame << endl;

	lbm();
	movepar(0.1);
}

void render(void) 
{
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	//gdraw();
	draw();								// DRAW PARTICLES
}

void start(void)
{
	initlbm();								// INIT LBM

	initpar();

}










int main(int argc, char** argv) {

	srand(time(NULL));

	GLFWwindow* window = initGLFWContext();
	initGLContext();

	start();

	frame = 0;

	double prevTime = glfwGetTime();

	while (!glfwWindowShouldClose(window)) {

		// skip frames
		//if (glfwGetTime() - prevTime < secPerFrame) continue;
		prevTime = glfwGetTime();

		update();

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		render();

		glfwSwapBuffers(window);
		glfwPollEvents();

		frame++;
	}

	delete U;

	//Exit
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}

void initGLContext() {
	
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, 640, 640);
	glOrtho(0, 1, 0, 1, 0, 1);
	glMatrixMode(GL_MODELVIEW);
}


GLFWwindow* initGLFWContext() {
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	GLFWwindow* window = glfwCreateWindow(640, 640, "Snow Simulator", NULL, NULL);

	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);

	return window;
}

//Print all errors to console
static void error_callback(int error, const char* description) {
	printf("\nError: %s", description);
}

//Key listener
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
}

//Mouse listener
void mouse_callback(GLFWwindow* window, int btn, int action, int mods) {
}