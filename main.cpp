//#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#include<string.h>
#include<math.h>

static GLint theta = 0.0;
static int r = 195, xc = 0, yc = 0;
GLfloat bx=0.0,by=0.0 ,vx=0.10, vy=0.10;
//using namespace std;


/*void myinit() {
	glClear(GL_COLOR_BUFFER_BIT);
	gluOrtho2D(-250.0, 250.0, -250.0, 250.0);
	//glPointSize(10.0);
	glClearColor(0.0, 0.0, 0.0, 1.0);
}*/

void text(char *string) {
	glColor3f(1.0,1.0,1.0);
  	glRasterPos2f(0.0,0.0);
 	int len, i;
	len = (int)strlen(string);
 	for (i = 0; i < len; i++)
    		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
}

void drawPixel(int x, int y) {
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POINTS);
		glVertex2i(xc+x, yc+y);
		glVertex2i(xc+y, yc+x);
		glVertex2i(xc+x, yc-y);
		glVertex2i(xc+y, yc-x);
		glVertex2i(xc-y, yc-x);
		glVertex2i(xc-x, yc-y);
		glVertex2i(xc-x, yc+y);
		glVertex2i(xc-y, yc+x);
	glEnd();
}


void drawCircle() {
	glPointSize(2.0);
	int p = 1-r;
	int x=0, y=r;
	for(x=0;x< y;++x) {
		if(p<0) {
			p=p+(2*x)+3;
			drawPixel(x,y);
		} else {
			p=p+(2*x)-(2*y)+5;
			y-=1;
			drawPixel(x,y);
		}
	}
}

void paddle() {
	glColor3f(1.0,1.0,1.0);
	glPointSize(10.0);
	glBegin(GL_POLYGON);
		glVertex2f(-20.0,-190.0);
		glVertex2f(20.0,-190.0);
		glVertex2f(20.0,-200.0);
		glVertex2f(-20.0,-200.0);
	glEnd();
}

void ball(GLfloat x, GLfloat y) {
	glColor3f(1.0,0.0,0.0);
	int i;
	int triangleAmount = 20;

	GLfloat radius = 10.0f;
	GLfloat twicePi = 2.0f * 3.14;

	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(x, y); // center of circle
		for(i = 0; i <= triangleAmount;i++) {
			glVertex2f(
		            x + (radius * cos(i *  twicePi / triangleAmount)),
			    y + (radius * sin(i * twicePi / triangleAmount))
			);
		}
	glEnd();
}

void rotatePaddle() {
	printf("HI");
	//glClear(GL_COLOR_BUFFER_BIT);
	//drawCircle();
	glLoadIdentity();
	glPushMatrix();
	//glTranslatef(0.0,0.0,0.0);
	glRotatef(theta,0.0,0.0,1.0);
	//glTranslatef(0.0,0.0,0.0);
	glPopMatrix();
	paddle();
	ball(bx, by);
	glFlush();
	glutSwapBuffers();
}

void myDisp() {
	//char name[] = "Circle Pong";
	////glScalef(10.0,10.0,1.0);
	//text(name);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	drawCircle();
	paddle();
	ball((float)bx,(float)by);
	glFlush();
	glutSwapBuffers();
}

void keys(unsigned char key, int x, int y) {
	if(key==GLUT_KEY_LEFT) {
		if(theta<360.0) theta+=360.0;
		theta-=5.0;
	}
	if(key==GLUT_KEY_RIGHT) {
		if(theta>360.0) theta-=360.0;
		theta+=5.0;
	}
	rotatePaddle();
	glutPostRedisplay();

}

void myReshape(int w, int h) {
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(w<=h)
		glOrtho(-250.0,250.0,-250.0*(GLfloat)h/(GLfloat)w,250.0*(GLfloat)h/(GLfloat)w,-1.0,1.0);
	else
		glOrtho(-250.0*(GLfloat)w/(GLfloat)h,250.0*(GLfloat)w/(GLfloat)h,-250.0,250.0,-1.0,1.0);
	glMatrixMode(GL_MODELVIEW);
}

//checkcollision
	void checkCollision(int bx,int by){
	if(bx > xPMax && bx < xPMin && by==yPMax)
	//Normal bounce
	else if (by==yPMax && bx==xPMax)
	//hits right sharp corner of paddle
	else if (by==yPMax && bx==xPMin)
	//hits left sharp corner of paddle
	else if(bx)
	if(d +10 > r){
	//bounce ball
	vx*=-1.0;
	vy*=-1.0;
	}
}

void idleFunc() {

	checkCollision(bx, by);
	bx+=vx;
	by+=vy;


	//ball(bx, by);
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	glutInit(&argc,argv);
	glutInitWindowSize(500,500);
	glutCreateWindow("Circle Pong");
	//myinit();
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutIdleFunc(idleFunc);
	glutReshapeFunc(myReshape);
	glutKeyboardFunc(keys);
	glutDisplayFunc(myDisp);
//	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	return 1;
}

