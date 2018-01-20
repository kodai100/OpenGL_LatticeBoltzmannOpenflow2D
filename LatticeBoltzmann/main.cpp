#include "main.h"

float framerate = 60;
float secPerFrame = 1 / framerate;

LatticeBoltzmann* wind_simulator;
LBMParticle* lbm_particle;

void update(void)
{
	
	cout << frame << endl;

	wind_simulator->update();
	lbm_particle->movepar(0.1);
}

void render(void) 
{
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	if(showGrid) wind_simulator->draw();
	lbm_particle->draw();								// DRAW PARTICLES
}

void start(void)
{
	showGrid = false;
	wind_simulator = new LatticeBoltzmann();
	lbm_particle = new LBMParticle(wind_simulator);

}

void exit() {
	delete wind_simulator;
	delete lbm_particle;
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

	//Exit
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	exit();

	return 0;
}

void initGLContext() {
	
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, 1280, 640);
	glOrtho(0, 1, 0, 1, 0, 1);
	glMatrixMode(GL_MODELVIEW);
}


GLFWwindow* initGLFWContext() {
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	GLFWwindow* window = glfwCreateWindow(1280, 640, "Snow Simulator", NULL, NULL);

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
	if (action != GLFW_RELEASE)
		return;
	switch (key) {
	case GLFW_KEY_ENTER:
		showGrid = !showGrid;
		break;
	}
}

//Mouse listener
void mouse_callback(GLFWwindow* window, int btn, int action, int mods) {
}