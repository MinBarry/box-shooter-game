#include "Shot.h"



Shot::Shot(float size[3], GLuint list, GLuint texture, float x, float y, float speed)
	:BaseObject(size, list, texture)
{
	this->speed = speed;
	rotation[0] = x;
	rotation[1] = y;
	updateCollider();
	
}

Shot::Shot(float size[3], float x, float y, float speed) : Shot(size, 0, 0, x, y, speed) {}

Shot::~Shot() {}
                          
void Shot::update(float dt) {
	nextpos[2] += (-1)*speed*dt;
	if (nextpos[2] > ENV_END+10 || nextpos[2] < ENV_START-10) {
		alive = false;
	}
}
void Shot::render() {
 	glPushMatrix();
	if (debug) {
		//draw collider
		glColor3f(1, 1, 1);
		glPointSize(10.);
		glBegin(GL_POINTS);
		glVertex3f(collider.xmax, collider.ymax, collider.zmax);
		glVertex3f(collider.xmin, collider.ymin, collider.zmin);
		glEnd();
		glPointSize(1.);
	}
	glColor3f(color[0],color[1],color[2]);
	glRotatef((GLfloat)rotation[1], 0, 1, 0.);
	glRotatef((GLfloat)rotation[0], 1., 0., 0.);
	
	glTranslatef(0, 0, nextpos[2]);
	if (list != 0) {
		glCallList(list);
	}
	else {
		glutSolidSphere(size[0], 20, 20);
	}
	glPopMatrix();
	position[0] = nextpos[0];
	position[1] = nextpos[1];
	position[2] = nextpos[2];
	if (rotation[0] != 0) {
		float radx = (rotation[0] * (3.14159265359 / 180.f));
		float x = position[0];
		float y = position[1];
		float z = position[2];

		float xp = x;
		float yp = y*cos(radx) - z*sin(radx);
		float zp = y*sin(radx) + z*cos(radx);

		position[0] = xp;
		position[1] = yp;
		position[2] = zp;
	}
	if (rotation[1] != 0) {
		float rady = (rotation[1] * (3.14159265359 / 180.f));
		float x = position[0];
		float y = position[1];
		float z = position[2];

		float xp = x*cos(rady) + z*sin(rady);
		float yp = y;
		float zp = -x*sin(rady) + z*cos(rady);

		position[0] = xp;
		position[1] = yp;
		position[2] = zp;
	}

	this->updateCollider();
}

void Shot::handleCollision(BaseObject* other) {
	alive = false;
	other->destroy();
}

void Shot::destroy() {

}
