#pragma once
#include "BaseObject.h"
#include "TargetParticleSystem.h"


//movements: x, y, z TODO:add x,y,x rotations
int const NUM_MOVEMENTS = 3;
int const POSITION_MOVEMENTS = 3;

class Target :
	public BaseObject
{
public:
	Target(float size[3], bool *movement, float *moveStart, float *moveEnd);
	Target(float size[3], GLuint list, bool *movement, float *moveStart, float *moveEnd);
	Target(float size[3], GLuint list, GLuint texture);
	Target(float size[3], GLuint list, GLuint texture, bool *movement, float *moveStart, float *moveEnd);
	~Target();
	void update(float dt);
	void render();
	void handleCollision(BaseObject* other);
	void destroy();
	void setScore(int s, float r, float g, float b);
	int getScore();
	bool isDying();

private:
	float speed;
	bool movement[NUM_MOVEMENTS];
	float moveStart[NUM_MOVEMENTS];
	float moveEnd[NUM_MOVEMENTS];
	bool moveSwitch[NUM_MOVEMENTS];
	int score;
	TargetParticleSystem * sys;
	bool dying;
};

