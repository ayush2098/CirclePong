#include<stdio.h>
#include<stdlib.h>
//#include<conio.h>
#include<GL/glut.h>
#include<string.h>
//#include<unistd.h>
#include<GL/freeglut.h>
#include<math.h>
#define pi  3.14159

static GLint theta = 0.0;
GLint xc = 0, yc = 0;
static int rc=195;
GLfloat Angle=0.0, Speed=0.1;
GLdouble radiusb=10.0,bx=0.0,by=0.0 ,vx=cos(Angle * pi / 180) * Speed, vy=sin(Angle * pi / 180) * Speed;
GLdouble x1=-20.0,Y1=-190.0,x2=20.0,y2=-190.0,x3=20.0,y3=-200.0,x4=-20.0,y4=-200.0;
GLdouble mat[4][4] = {{x1, x2, x3, x4},{Y1, y2, y3, y4},{1.0, 1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0}}, rot_mat[4][4], res[4][4];
GLint score = 0;
char toPrint[20],lol;

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
		glVertex2d(x1,Y1);
		glVertex2d(x2,y2);
		glVertex2d(x3,y3);
		glVertex2d(x4,y4);
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

void ball(GLdouble x, GLdouble y) {
	glColor3f(1.0,0.0,0.0);
	int i;
	int triangleAmount = 20;
	//GLfloat radius = 10.0f;
	GLfloat twicePi = 2.0f * 3.14;

	glBegin(GL_TRIANGLE_FAN);
	glVertex2d(x, y); // center of circle
	for(i = 0; i <= triangleAmount;i++) {
		glVertex2d(
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

}

/****/

/**Collision Detection**/
void getLine(double x1, double y1, double x2, double y2, double &a, double &b, double &c)
{
    // (x- p1X) / (p2X - p1X) = (y - p1Y) / (p2Y - p1Y)
    a = y1 - y2;
    b = x2 - x1;
    c = x1 * y2 - x2 * y1;
}

double dist(double pct1X, double pct1Y, double pct2X, double pct2Y, double pct3X, double pct3Y)
{
    double a, b, c;
    getLine(pct2X, pct2Y, pct3X, pct3Y, a, b, c);
    return abs(a * pct1X + b * pct1Y + c) / sqrt(a * a + b * b);
}

void Vrecalc(){
double deg=Angle* 180/pi;
if(deg>=180)
    deg =(int)deg%180;
if(deg<=-180)
    deg =(int)deg%180;
//if (collision)recal
Angle = atan2(vy * pi / 180, vx * pi / 180);
deg +=deg+rand()%45+1; // Could also be subtracting here
Angle=deg*pi/180;
printf("theta: %f\n vx: %f,vy: %f",deg,vx,vy);
vx = cos(Angle) * Speed;
vy = sin(Angle) * Speed;
}

void checkCollision(double bx,double by){
	//The Below commented one works amazingly !!
	double dPaddle=dist(bx, by, x1, Y1, x2, y2);
	double d=pow(bx,2)+pow(by,2);
if(d<pow(rc-radiusb,2)){
	if(dPaddle<=radiusb){

    Vrecalc();
    vy*=-1.0;
    vx*=-1.0;
			score+=1;
		}
	}
	else {
		glClear(GL_COLOR_BUFFER_BIT);
		glPushMatrix();
		glTranslatef(-300,50,0);
		glScalef(0.8,0.8,0);
		//glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)"Score : ");
		glColor3f(1.0,1.0,0.0);
		//sprintf(toPrint, "%d", score);
		glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)"GAME OVER");
		glTranslatef(300,-50,0);
		glTranslatef(-25,-50,0);
		glScalef(0.5,0.5,0);
		glColor3f(1.0,1.0,1.0);
		glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)"Score : ");
		sprintf(toPrint, "%d", score);
		glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)(toPrint));
		glTranslatef(-25,50,0);
		glPopMatrix();
		glFlush();
		scanf("%c",&lol);
	}
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
	bx-=vx;
	by-=vy;
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
	glPushMatrix();
	glTranslatef(150,200,0);
	glScalef(0.2,0.2,0);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)"Score : ");
	glColor3f(1.0,0.0,0.0);
	//itoa(score, toPrint, 10);
	sprintf(toPrint, "%d", score);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)(toPrint));
	glTranslatef(-200,-200,0);
	glPopMatrix();
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
	//double dPaddle=dist(bx, by, x1, Y1, x2, y2);
	//printf("dist= %f\nX1: %f Y1: %f\n X2: %f Y2: %f",dPaddle,x1,Y1,x2,y2);
	//glutKeyboardFunc(keys);
	glutSpecialFunc(specialkey);
	glutDisplayFunc(myDisp);
	glutFullScreen();
	glutMainLoop();
	return 1;
}
