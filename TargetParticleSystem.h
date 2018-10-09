#pragma once
#include <cstdlib>
#define NUM_PARTICLES 200
struct particle {
	float x, y, z;
	float vx, vy, vz;
};

class TargetParticleSystem
{
public:
	TargetParticleSystem(float, float, float, float ,float, float);
	~TargetParticleSystem();
	bool update(float);
	void render();
private:
	struct particle particles[NUM_PARTICLES];
	float r, g, b;
	float scale;
	float death_time;
	float time_passed;
};

