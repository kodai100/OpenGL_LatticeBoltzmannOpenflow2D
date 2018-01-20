

typedef struct Particle {
	float x, y;
} Particle;

class LBMParticle {
public:
	const int nparmax = 2 * 4096;			// num of particles

	LatticeBoltzmann* boltzmann;

	Particle* par;

	LBMParticle(LatticeBoltzmann* boltzmann);
	~LBMParticle();

	void update(float dt);
	void draw();
	const float BilinearInterpolation(float x, float y, int x1, int x2, int y1, int y2, float f11, float f21, float f22, float f12) const;
};