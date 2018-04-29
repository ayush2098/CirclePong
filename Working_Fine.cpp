#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#include<string.h>
#include<math.h>

static GLint theta = 0.0;
static int rc = 195, xc = 0, yc = 0;
GLfloat radiusb=10.0,bx=0.0,by=0.0 ,vx=0.010, vy=0.010;
GLfloat x1=-20.0,Y1=-190.0,x2=20.0,y2=-190.0,x3=20.0,y3=-200.0,x4=-20.0,y4=-200.0;
GLfloat mat[4][4] = {{x1, x2, x3, x4},{Y1, y2, y3, y4},{1.0, 1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0}}, rot_mat[4][4], res[4][4];

/** Drawing Outer Circle **/

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
	int p = 1-rc;
	int x=0, y=rc;
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

/****/


/**Drawing Paddle**/

void paddle() {
	glColor3f(1.0,1.0,1.0);
	glPointSize(10.0);
	glBegin(GL_POLYGON);
		// glVertex2f(-20.0,-190.0);
		// glVertex2f(20.0,-190.0);
		// glVertex2f(20.0,-200.0);
		// glVertex2f(-20.0,-200.0);
		glVertex2f(x1,Y1);
		glVertex2f(x2,y2);
		glVertex2f(x3,y3);
		glVertex2f(x4,y4);
	glEnd();
}

/****/

/**Reshape Functtion**/

void myReshape(int w, int h) {
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(w<=h)
		gluOrtho2D(-250.0,250.0,-250.0*(GLfloat)h/(GLfloat)w,250.0*(GLfloat)h/(GLfloat)w);
	else
		gluOrtho2D(-250.0*(GLfloat)w/(GLfloat)h,250.0*(GLfloat)w/(GLfloat)h,-250.0,250.0);
	glMatrixMode(GL_MODELVIEW);
}

/****/

/**Drawing Ball**/

void ball(GLfloat x, GLfloat y) {
	glColor3f(1.0,0.0,0.0);
	int i;
	int triangleAmount = 20;
	//GLfloat radius = 10.0f;
	GLfloat twicePi = 2.0f * 3.14;

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y); // center of circle
	for(i = 0; i <= triangleAmount;i++) {
		glVertex2f(
				x + (radiusb * cos(i *  twicePi / triangleAmount)),
				y + (radiusb * sin(i * twicePi / triangleAmount))
			  );
	}
	glEnd();
}

/****/

/**Rotating**/

void rotate() {
	int i,j,k;

	float angle = (theta*3.1459)/180.0;
	rot_mat[0][0] = cos(angle);
	rot_mat[0][1] = -sin(angle);
	rot_mat[0][2] = xc*(1-cos(angle))+yc*sin(angle);
	rot_mat[0][3] = 0;
	rot_mat[1][0] = sin(angle);
	rot_mat[1][1] = cos(angle);
	rot_mat[1][2] = yc*(1-cos(angle))-xc*sin(angle);
	rot_mat[1][3] = 0;
	rot_mat[2][0] = 0;
	rot_mat[2][1] = 0;
	rot_mat[2][2] = 1;
	rot_mat[2][3] = 0;
	rot_mat[3][0] = 0;
	rot_mat[3][1] = 0;
	rot_mat[3][2] = 0;
	rot_mat[3][3] = 1;

	//Rotating Matrix
	for(i=0;i<4;++i) {
		for(j=0;j<4;++j) {
			res[i][j]=0;
			for(k=0;k<4;++k) {
				res[i][j] += rot_mat[i][k]*mat[k][j];
			}
			res[i][j];
			//printf("%f\t", mat[i][j]);
		}
		//printf("\n");
	}
	x1=res[0][0];
	x2=res[0][1];
	x3=res[0][2];
	x4=res[0][3];
	Y1=res[1][0];
	y2=res[1][1];
	y3=res[1][2];
	y4=res[1][3];
	//printf("%f\n",x1);
}

/****/

/**Collision Detection**/

void checkCollision(int bx,int by){
	//The Below commented one works amazingly !!
	/*float d=sqrt(pow(bx,2)+pow(by,2));
	if(d +10 > rc){
		//bounce ball
		vx*=-1.0;
		vy*=-1.0;
	}*/

	
}

/****/


void rotatePaddle() {
		glClear(GL_COLOR_BUFFER_BIT);
		drawCircle();
		ball(bx, by);
		//glPushMatrix();
			rotate();
			paddle();
		//glPopMatrix();
		glFlush();
		glutSwapBuffers();
}

void idleFunc() {
	checkCollision(bx, by);
	bx+=vx;
	by+=vy;
	glutPostRedisplay();
}

void init() {
	glClear(GL_COLOR_BUFFER_BIT);
	paddle();
}

void myDisp() {
	glClear(GL_COLOR_BUFFER_BIT);
	drawCircle();
	ball(bx, by);
		paddle();
	glFlush();
	glutSwapBuffers();
}

/*void keys(unsigned char key, int x, int y) {
	if(key=='a') {
		if(theta<360.0) theta+=360.0;
		theta-=2.0;
	}
	if(key=='s') {
		if(theta>360.0) theta-=360.0;
		theta+=2.0;
	}
	rotatePaddle();
}*/

void specialkey(int key, int x, int y)
{
	if(key==GLUT_KEY_LEFT) {
		if(theta<360.0) theta+=360.0;
		theta-=2.0;
	}
	if(key==GLUT_KEY_RIGHT) {
		if(theta>360.0) theta-=360.0;
		theta+=2.0;
	}
	rotatePaddle();

}

int main(int argc, char **argv) {
	glutInit(&argc,argv);
	glutInitWindowSize(500,500);
	glutCreateWindow("Circle Pong");
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	init();
	glutIdleFunc(idleFunc);
	glutReshapeFunc(myReshape);
	//glutKeyboardFunc(keys);
	glutSpecialFunc(specialkey);
	glutDisplayFunc(myDisp);
	glutMainLoop();
	return 1;
}
