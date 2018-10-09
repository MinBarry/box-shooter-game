#include "Target.h"

Target::Target(float size[3], bool *movement, float *start, float *end)
	: Target(size, 0, 0, movement, start, end) {}

Target::Target(float size[3], GLuint list, bool *movement, float *start, float *end)
	: Target(size, list, 0, start, end) {}

Target::Target(float size[3], GLuint list, GLuint texture)
	:BaseObject(size, list, texture)
{
	sys = 0; dying = false;
	int start, end;
	int max = ENV_END - TARGET_SPACE;
	int min = ENV_START + TARGET_SPACE;
	for (int i = 0; i < POSITION_MOVEMENTS; i++) {
		start = rand() % max*2; 
		end = rand() % ((max * 2 + 1) - start)+ start;
		start += min;
		end += min;
 		moveSwitch[i] = 0;
		if (rand() % 100 >= 50)
			this->movement[i] = 0;
		else
			this->movement[i] = 1;
		this->moveStart[i] = start;
		this->moveEnd[i] = end;
		nextpos[i] = moveStart[i];
		position[i] = nextpos[i];
	}
	this->updateCollider();
}
Target::Target(float size[3], GLuint list, GLuint texture, bool *movement, float *start, float *end)
	: BaseObject(size, list, texture)
{
	sys = 0; dying = false;
	for (int i = 0; i < POSITION_MOVEMENTS; i++) {
		moveSwitch[i] = 0;
		this->movement[i] = movement[i];
		this->moveStart[i] = start[i];
		this->moveEnd[i] = end[i];
		nextpos[i] = moveStart[i];	
		position[i] = nextpos[i];
	}
	this->updateCollider();
}

Target::~Target() {}

void Target::update(float dt)
{
	if (sys != 0) {
		if (!sys->update(dt)) {
			alive = false;
			delete sys;
		}
		return;
	}
	for (int i = 0; i < POSITION_MOVEMENTS; i++) {
		if (movement[i]) {
			if (moveSwitch[i])
				nextpos[i] += -speed*dt;
			else
				nextpos[i] += speed*dt;
			if (nextpos[i] > moveEnd[i] ) {
				moveSwitch[i] = !moveSwitch[i];
				nextpos[i] = position[i];
			}
			if (nextpos[i] < moveStart[i]){
				moveSwitch[i] = !moveSwitch[i];
				nextpos[i] = position[i];
			}			
		}
	}
}

void Target::render() {
	if (sys != 0) {
		sys->render();
		return;
	}
	glPushMatrix();
	if (debug) {
		//draw collider
		glColor3f(1, 1, 1);
		glPointSize(5.);
		glBegin(GL_POINTS);
		glVertex3f(collider.xmax, collider.ymax, collider.zmax);
		glVertex3f(collider.xmin, collider.ymin, collider.zmin);
		glEnd();
		glPointSize(1.);
	}
	glColor3f(color[0],color[1],color[2]);
	glTranslatef(nextpos[0], nextpos[1], nextpos[2]);
	if(list !=0)
		glCallList(list);
	else
		glutSolidCube(size[0]);	
	glPopMatrix();

	position[0] = nextpos[0]; 
	position[1] = nextpos[1]; 
	position[2] = nextpos[2];
	this->updateCollider();
}

void Target::handleCollision(BaseObject* other) {
	for (int i = 0; i < NUM_MOVEMENTS; i++) {
		moveSwitch[i] = !moveSwitch[i];
	}
	nextpos[0] = position[0];
	nextpos[1] = position[1];
	nextpos[2] = position[2];
}

void Target::destroy() {
	if (sys == 0) {
		dying = true;
		sys = new TargetParticleSystem(color[0], color[1], color[2], position[0], position[1], position[2]);
	}
}
bool Target::isDying() {
	bool temp = dying;
	dying = false;
	return temp;
}

void Target::setScore(int s, float r, float g, float b) {
	this->score = s;
	color[0] = r;
	color[1] = g;
	color[2] = b;
	//caculate speed based on target score	
	if (score / 10 > TARGET_MAX_SPEED)
		speed = TARGET_MAX_SPEED;
	else
		speed = score / 10;
}
int Target::getScore() { return score; }