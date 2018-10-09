#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#include <GL/glew.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include "GameSettings.h"

struct BoundingBox {
	float xmin, xmax;
	float ymin, ymax;
	float zmin, zmax;
};
typedef struct BoundingBox BoundingBox;
#pragma once
class BaseObject
{
public:
	BaseObject(float size[3]);
	BaseObject(float size[3], GLuint list, GLuint texture);
	virtual void update(float dt)=0;
	virtual void render()=0;
	void checkCollision(BaseObject *);
	virtual void handleCollision(BaseObject* other)=0;
	virtual void destroy()=0;
	virtual ~BaseObject();
	void setPosition(float, float, float);
	void setRotation(float, float, float);
	void setScale(float, float, float);
	void setColor(float, float, float);
	bool isAlive();
	BoundingBox getCollider() { return collider; }
	void updateCollider();
	void setList(GLuint) { this->list = list; };
	void setTexture(GLuint) { this->texture = texture; };
	float * getColor() { return color; };
	

protected:
	float nextpos[3], position[3], rotation[3], scale[3], color[3], size[3];
	bool alive;
	GLuint list, texture;
	BoundingBox collider;
};

