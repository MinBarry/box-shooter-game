#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <vector>
#include "Target.h"
#include "Shot.h"
#include "Wall.h"


const char *WINDOWTITLE = { "Minna Barry - Final Project" };
const int GLUITRUE = { true };
const int GLUIFALSE = { false };
#define ESCAPE		0x1b
const int INIT_WINDOW_SIZE = { 800 };
const GLfloat BACKCOLOR[] = { 0., 0., 0., 1. };

void	Axes(float);
int		MainWindow;
void	Update();
void	Display();
void	InitLists();
void	InitGraphics();
void	MouseMotion(int, int);
void	MouseButton(int, int, int, int);
void	Keyboard(unsigned char, int, int);
void	Reset();
void	Resize(int, int);
void	Visibility(int);
void	draw_tile();
void	draw_cube(float size);
void	initLights();
void	setSpotLight(int, float, float, float, float, float, float, float, float, float);
void	setPointLight(int, float, float, float, float, float, float);
void	setMaterial(float, float, float, float);
void	initTextures();
float*	Array3(float x, float y, float z);
unsigned char * BmpToTexture(char *filename, int *width, int *height);
void	initEnv();
char*	scoreText();
void	renderString(float x, float y, float z, char *s);

GLuint	BoxList;
GLuint	TileList;
GLuint	walltex;
bool	freeze = true;
bool	shoot = false;
bool	game_over = false;
float	pitch;
float	yaw;
float	Time = 0;
float	dt;
float	spawn_time = 0;
float	time_since_spawm = 0;
float	game_time = 0;
int		total_score = 0;
int		Xmouse, Ymouse;
float sizes[3] = { 1 ,1 ,1 };
typedef std::vector<Target *> Targets;
typedef std::vector<Shot *> Shots;
Targets box_vec;
Shots shot_vec;
Wall *walls[6];


void generateTargets() {
	time_since_spawm += dt;
	if (box_vec.size() >= TARGET_MAX_NUM) {
		return;
	}
	else if(box_vec.size() <= TARGET_MAX_NUM*0.25){
		spawn_time = 1;
	}
	else if(box_vec.size() <= TARGET_MAX_NUM*0.5) {
		spawn_time = 2;
	}
	else {
		spawn_time = 3;
	}
	if (time_since_spawm > spawn_time) {
		Target * box = new Target(sizes, BoxList, (GLuint)0);
		float r = ((rand() % 9) + 1) / 10.;
		float g = ((rand() % 9) + 1) / 10.;
		float b = ((rand() % 9) + 1) / 10.;
		int score = (rand() % 5 + 1)*10;
		box->setScore(score, r, g, b);
		box_vec.push_back(box);
		time_since_spawm = 0;
	}

}

int
main(int argc, char *argv[])
{
	srand(time(NULL));
	glutInit(&argc, argv);
	InitGraphics();
	Reset();
	glutSetWindow(MainWindow);
	glutMainLoop();
	return 0;
}

void
Update()
{
	float newTime = glutGet(GLUT_ELAPSED_TIME);
	dt = (newTime - Time) / 1000.;
	Time = newTime;
	game_time += dt;

	if (game_time > GAME_TIME) {
		game_over = true;
		glutIdleFunc(NULL);
		glutPassiveMotionFunc(NULL);
	}
	generateTargets();

	for (Targets::iterator it = box_vec.begin(); it != box_vec.end(); ++it) {
		(*it)->update(dt);
	}
	for (Shots::iterator it = shot_vec.begin(); it != shot_vec.end(); ++it) {
		(*it)->update(dt);
		if ((*it)->isAlive() == false) {
			delete *it;
			shot_vec.erase(it);
			break;
		}
	}
	for (Shots::iterator it = shot_vec.begin(); it != shot_vec.end(); ++it) {
		for (Targets::iterator bt = box_vec.begin(); bt != box_vec.end(); ++bt) {
			(*it)->checkCollision((*bt));
			if ((*bt)->isDying()) {
				total_score += (*bt)->getScore();
				printf("Score is %d\n", total_score);
			}
			if ((*bt)->isAlive() == false) {
				delete *bt;
				box_vec.erase(bt);
				break;
			}
		}
	}

	glutSetWindow(MainWindow);
	glutPostRedisplay();

}

void setupWidescreen(GLsizei *width, GLsizei *height, GLint *display_pos) {
	GLfloat wide_width = (GLfloat)*width;
	GLfloat wide_height = (wide_width / 1.78f);
	//widescreen aspect ratios (1.78:1) 		
	*display_pos = (*height - (GLint)wide_height) / 2;
	*height = (GLint)wide_height;

}
void resizeViewport(GLsizei width, GLsizei height) {
	GLint	display_pos = 0;
	if (height == 0) { height = 1; }
	setupWidescreen(&width, &height, &display_pos);
	glViewport(0, display_pos, width, height);
	GLfloat aspect_ratio = (GLfloat)width / (GLfloat)height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, aspect_ratio, 0.1f, ENV_END+5.);
}


void
Display()
{
	// erase the background:
	glColor3f(1, 1, 1);
	glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	resizeViewport(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (game_over) {
		glPushMatrix();
		glScalef(10,10,10);
		renderString(0., 0., -0.2, scoreText());
		glPopMatrix();
	}
	else {
		glBegin(GL_LINE_LOOP);
		glColor3f(1, 1, 1);
		glVertex3f(0.005, 0.005, -0.2);
		glVertex3f(-0.005, 0.005, -0.2);
		glVertex3f(-0.005, -0.005, -0.2);
		glVertex3f(0.005, -0.005, -0.2);
		glEnd();
	}
	

	glRotatef((GLfloat)pitch, 1., 0., 0.);
	glRotatef((GLfloat)yaw, 0., 1., 0.);
	glScalef(0.1, 0.1, 0.1);
	//	 eye ,look-at,up-vector:
	gluLookAt(0, 0, 0,	0,0,-1,	0, 1, 0);
	glEnable(GL_NORMALIZE);

	for (int i = 0; i < 6; ++i) {
		float * color = walls[i]->getColor();
		setMaterial(color[0], color[1], color[2], 0.5);
		walls[i]->render();
	}

	glEnable(GL_LIGHTING);

	setPointLight(GL_LIGHT0, 0., 9., 0., 0.8, 0.8, 0.8);
	for (Targets::iterator it = box_vec.begin(); it != box_vec.end(); ++it) {
		float * color = (*it)->getColor();
		setMaterial(color[0], color[1], color[2], 0.5);
		(*it)->render();
	}
	for (Shots::iterator it = shot_vec.begin(); it != shot_vec.end(); ++it) {
		float * color = (*it)->getColor();
		setMaterial(color[0], color[1], color[2], 0.5);
		(*it)->render();
	}
	if (shoot) {
		shoot = false;
		if(total_score > 0)
			total_score -= 5;
		shot_vec.push_back(new Shot(Array3(0.2,0.2,0.2), -pitch, -yaw, SHOT_MAX_SPEED));
	}
	glDisable(GL_LIGHTING);

	if (!freeze && !game_over) {
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0., 100., 0., 100.);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glColor3f(1., 1., 1.);
		renderString(5., 5., 0., scoreText());
	}
	glutSwapBuffers();
	glFlush();
}

void
InitGraphics()
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(INIT_WINDOW_SIZE, INIT_WINDOW_SIZE);

	MainWindow = glutCreateWindow(WINDOWTITLE);
	glutSetWindowTitle(WINDOWTITLE);
	glutSetCursor(GLUT_CURSOR_NONE);
	glClearColor(BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3]);

	glutSetWindow(MainWindow);
	glutDisplayFunc(Display);
	glutReshapeFunc(Resize);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(MouseButton);
	glutVisibilityFunc(Visibility);
	glutTimerFunc(-1, NULL, 0);

#ifdef WIN32
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		fprintf(stderr, "glewInit Error\n");
	}
	else
		fprintf(stderr, "GLEW initialized OK\n");
	fprintf(stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif
	initLights();
	initTextures();
	InitLists();
	initEnv();

	for (int i = 0; i < START_NUM_BOXES; ++i) {
		Target * box = new Target(sizes, BoxList, (GLuint)0);
		float r = ((rand() % 9) + 1) / 10.;
		float g = ((rand() % 9) + 1) / 10.;
		float b = ((rand() % 9) + 1) / 10.;
		box->setScore(10,r,g,b);
		box_vec.push_back(box);
	}

}
void initLights() {
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Array3(0.2, 0.2, 0.2));
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}
void setPointLight(int ilight, float x, float y, float z, float r, float g, float b) {
	glLightfv(ilight, GL_POSITION, Array3(x, y, z));
	glLightfv(ilight, GL_AMBIENT, Array3(0., 0., 0.));
	glLightfv(ilight, GL_DIFFUSE, Array3(r, g, b));
	glLightfv(ilight, GL_SPECULAR, Array3(r, g, b));

	glLightf(ilight, GL_CONSTANT_ATTENUATION, 1.);
	glLightf(ilight, GL_LINEAR_ATTENUATION, 0.);
	glLightf(ilight, GL_QUADRATIC_ATTENUATION, 0.);
	glEnable(ilight);

}
void
setMaterial(float r, float g, float b, float shininess) {

	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Array3(0., 0, 0.));
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Array3(r, g, b));
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Array3(r, g, b));
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Array3(.8, .8, .8));
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}
void
initTextures() {
	// init texture
	unsigned char* tex1;
	int  width = 2048, height = 1024;
	tex1 = BmpToTexture("wallpaper.bmp", &width, &height);
	if (width == 0 || height == 0) {
		fprintf(stderr, "Couldn't load texture from");
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &walltex);
	glBindTexture(GL_TEXTURE_2D, walltex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex1);
	
}

void initEnv() {
	
	//walls = new Wall[6];
	walls[0] = new Wall(Array3(20, 20, 0), TileList, walltex);
	walls[0]->setPosition(0, 0, ENV_END);
	walls[0]->setRotation(0, 0, 180);

	walls[1] = new Wall(Array3(20, 20, 0), TileList, walltex);
	walls[1]->setPosition(0, 0, ENV_START);
	walls[1]->setRotation(0, 180, 180);

	walls[2] = new Wall(Array3(20, 0, 20), TileList, walltex); //ceil
	walls[2]->setPosition(0, ENV_END, 0);
	walls[2]->setRotation(90,0,0);

	walls[3] = new Wall(Array3(20, 0, 20), TileList, walltex);
	walls[3]->setPosition(0, ENV_START, 0);
	walls[3]->setRotation(-90, 0, 0);

	walls[4] = new Wall(Array3(0, 20, 20), TileList, walltex);
	walls[4]->setPosition(ENV_END, 0, 0);
	walls[4]->setRotation(0, -90, 0);

	walls[5] = new Wall(Array3(0, 20, 20), TileList, walltex);
	walls[5]->setPosition(ENV_START, 0, 0);
	walls[5]->setRotation(0, 90, 0);
}
void Keyboard(unsigned char c, int x, int y){
	switch (c)
	{
	case 'f':
	case 'F':
		freeze = !freeze;
		if (freeze) {
			glutIdleFunc(NULL);
			glutPassiveMotionFunc(NULL);			
		}
		else if(!game_over){
			glutIdleFunc(Update);
			glutPassiveMotionFunc(MouseMotion);
			Time = 0;
		}
		break;

	case(' ') :
		if (!freeze && !game_over)
			shoot = true;
		break;

	case('d') :
		debug = !debug;
		break;
	}
	
	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

bool pressed = 0;
void MouseMotion(int x, int y){
	if (pressed) {

		int dx = x - Xmouse;		// change in mouse coords
		int dy = y - Ymouse;
		pitch += (1 * dy);
		yaw += (1 * dx);
		pitch += CAM_SPEED*dy;
		yaw += CAM_SPEED*dx;

		Xmouse = x;			// new current position
		Ymouse = y;
	}
	glutSetWindow(MainWindow);
	glutPostRedisplay();
}
void MouseButton(int button, int state, int x, int y){
	if (state == GLUT_DOWN){
		Xmouse = x;
		Ymouse = y;
		pressed = !pressed;
	}

}

void
Reset()
{
	pitch = 0;
	yaw = 0.;
	Time = 0;
	freeze = 1;
	shoot = false;
	game_over = false;
	Time = 0;
	spawn_time = 0;
	time_since_spawm = 0;
	game_time = 0;
	total_score = 0;
}

void
Resize(int width, int height)
{
	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void
Visibility(int state)
{
	if (state == GLUT_VISIBLE)
	{
		glutSetWindow(MainWindow);
		glutPostRedisplay();
	}
}

void
InitLists()
{
	
	BoxList = glGenLists(1);
	glNewList(BoxList, GL_COMPILE);
	draw_cube(sizes[0]);
	glEndList();

	TileList = glGenLists(1);
	glNewList(TileList, GL_COMPILE);
	draw_tile();
	glEndList();
}

void draw_cube(float size) {
	float dx = size / 2.f;
	float dy = size / 2.f;
	float dz = size / 2.f;

	glBegin(GL_QUADS);

	glNormal3f(0., 0., 1.);
	glVertex3f(-dx, -dy, dz);
	glVertex3f(dx, -dy, dz);
	glVertex3f(dx, dy, dz);
	glVertex3f(-dx, dy, dz);

	glNormal3f(0., 0., -1.);
	glTexCoord2f(0., 0.);
	glVertex3f(-dx, -dy, -dz);
	glTexCoord2f(0., 1.);
	glVertex3f(-dx, dy, -dz);
	glTexCoord2f(1., 1.);
	glVertex3f(dx, dy, -dz);
	glTexCoord2f(1., 0.);
	glVertex3f(dx, -dy, -dz);

	glNormal3f(1., 0., 0.);
	glVertex3f(dx, -dy, dz);
	glVertex3f(dx, -dy, -dz);
	glVertex3f(dx, dy, -dz);
	glVertex3f(dx, dy, dz);

	glNormal3f(-1., 0., 0.);
	glVertex3f(-dx, -dy, dz);
	glVertex3f(-dx, dy, dz);
	glVertex3f(-dx, dy, -dz);
	glVertex3f(-dx, -dy, -dz);

	glNormal3f(0., 1., 0.);
	glVertex3f(-dx, dy, dz);
	glVertex3f(dx, dy, dz);
	glVertex3f(dx, dy, -dz);
	glVertex3f(-dx, dy, -dz);

	glNormal3f(0., -1., 0.);
	glVertex3f(-dx, -dy, dz);
	glVertex3f(-dx, -dy, -dz);
	glVertex3f(dx, -dy, -dz);
	glVertex3f(dx, -dy, dz);
	glEnd();
}

void draw_tile()
{
	glBegin(GL_QUADS);
	glNormal3f(0., 1., 0.);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(float(ENV_START), float(ENV_START),0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(float(ENV_END), float(ENV_START),0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(float(ENV_END), float(ENV_END),0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(float(ENV_START), float(ENV_END),0);
	glEnd();

}

float * Array3(float x, float y, float z) {
	static float array[4];
	array[0] = x;
	array[1] = y;
	array[2] = z;
	array[3] = 1.0;
	return array;
}

char * scoreText() {
	char buffer[20] = { "\0" };
	sprintf(buffer,"SCORE  %d",total_score);
	return buffer;
}
void renderString(float x, float y, float z, char *s)
{
	glRasterPos3f((GLfloat)x, (GLfloat)y, (GLfloat)z);
	char buffer[20] = { "\0" };
	sprintf(buffer, "SCORE  %d", total_score);
	char c;	
	for (int i=0; (c = buffer[i]) != '\0'; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}
}