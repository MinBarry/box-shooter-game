#include "BaseObject.h"


BaseObject::BaseObject(float size[3]) : BaseObject(size, 0, 0) {}

BaseObject::BaseObject(float size[3], GLuint list, GLuint texture)
{
	for (int i = 0; i < 3; i++) {
		position[i] = 0;
		nextpos[i] = 0;
		rotation[i] = 0;
		scale[i] = 1;
		color[i] = 1;
		this->size[i] = size[i];
	}
	alive = true;
	this->list = list;
	this->texture = texture;
	updateCollider();
}

//called after update
void BaseObject::checkCollision(BaseObject * other) {
	if (collider.xmax > other->collider.xmin && other->collider.xmax > collider.xmin &&
		collider.zmax > other->collider.zmin && other->collider.zmax > collider.zmin &&
		collider.ymax > other->collider.ymin && other->collider.ymax > collider.ymin) {		
		
		this->handleCollision(other);
	}
		
}

void BaseObject::setPosition(float x, float y, float z) {
	position[0] = x;
	position[1] = y;
	position[2] = z; 
	updateCollider();
}

void BaseObject::setRotation(float x, float y, float z) {
	rotation[0] = x;
	rotation[1] = y;
	rotation[2] = z;
}
void BaseObject::setScale(float x, float y, float z) {
	scale[0] = x;
	scale[1] = y;
	scale[2] = z;
	size[0] *= x;
	size[1] *= y;
	size[2] *= z;
	updateCollider();
}
void BaseObject::setColor(float r, float g, float b) {
	color[0] = r;
	color[1] = g;
	color[2] = b;
}

void BaseObject::updateCollider() {
	//Rotation will be handled by children
	collider.xmax = (size[0] / 2.) + position[0];
	collider.ymax = (size[1] / 2.) + position[1];
	collider.zmax = (size[2] / 2.) + position[2];

	collider.xmin = collider.xmax - size[0];
	collider.ymin = collider.ymax - size[1];
	collider.zmin = collider.zmax - size[2];
	
}
bool BaseObject::isAlive() {
	return alive;
}

BaseObject::~BaseObject() {}
