#pragma once
#include "BaseObject.h"

class Shot :
	public BaseObject
{
public:
	Shot(float [3], float , float ,float);
	Shot(float size[3], GLuint list, GLuint texture, float x, float y, float speed);
	void render();
	void update(float);
	void destroy();
	void handleCollision(BaseObject* other);
	~Shot();

private:
	float speed;
};


