#include "TargetParticleSystem.h"
#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#include <GL/glew.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

TargetParticleSystem::TargetParticleSystem(float r, float g, float b, float x, float y, float z)
{
	time_passed = 0;
	death_time = 0.5;
	scale = 10;
	this->r = r;
	this->b = b;
	this->g = g;
	for (int i = 0; i < NUM_PARTICLES; ++i) {
		particles[i].x = x;
		particles[i].y = y;
		particles[i].z = z;
		particles[i].vx = rand() % 20;
		particles[i].vy = rand() % 20;
		particles[i].vz = rand() % 20;
		particles[i].vx -= 10;
		particles[i].vy -= 10;
		particles[i].vz -= 10;
	}
}
void TargetParticleSystem::render() {
	float s = scale / 10.;
	for (int i = 0; i < NUM_PARTICLES; ++i) {
		//glBegin(GL_POINTS);
		//glPointSize(5.);
		//glColor3f(r, g, b);
		//glVertex3f(particles[i].x, particles[i].y, particles[i].z);
		glPushMatrix();
		glTranslatef(particles[i].x, particles[i].y, particles[i].z);
		glScalef(s,s,s);
		glutSolidCube(0.1);
		glPopMatrix();
		//glPointSize(1.);
		glEnd();
	}
}

bool TargetParticleSystem::update(float dt) {
	time_passed += dt;
	scale -= 15*dt;
	if (time_passed > death_time)
		return false;
	for (int i = 0; i < NUM_PARTICLES; ++i) {
		particles[i].x += particles[i].vx*dt;
		particles[i].y += particles[i].vy*dt;
		particles[i].z += particles[i].vz*dt;
	}
	return true;
}

TargetParticleSystem::~TargetParticleSystem()
{
}
