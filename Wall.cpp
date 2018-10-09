#include "Wall.h"



Wall::Wall(float size[3], GLuint list, GLuint tex)
	:BaseObject(size, list, tex){}

Wall::~Wall() {}

void Wall::render() {
	if (list != 0) {
		glPushMatrix();
		glTranslatef(position[0],position[1],position[2]);
		glRotatef(rotation[0],1,0,0);
		glRotatef(rotation[1], 0, 1, 0);
		glRotatef(rotation[2], 0, 0, 1);
		if (texture != 0) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture);
		}
		glCallList(list);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
}
void Wall::update(float dt) {}
void Wall::handleCollision(BaseObject * other) {}
void Wall::destroy() {}