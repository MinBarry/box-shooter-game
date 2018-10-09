#pragma once
#include "BaseObject.h"
class Wall :
	public BaseObject
{
public:
	Wall(float [3],GLuint , GLuint);
	void render();
	void update(float);
	void destroy();
	void handleCollision(BaseObject *);
	~Wall();
};

