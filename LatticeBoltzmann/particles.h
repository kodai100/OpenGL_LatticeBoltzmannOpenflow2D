

void draw();
void initpar();
void movepar(float dt);

const int nparmax = 2*4096;			// num of particles
void initrendervbo(void);
extern float probrand_part;

typedef struct Particle {
	float x, y;
} Particle;