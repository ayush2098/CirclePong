//0.012,0.259,0.325 - DarkBlue - LEVEL1
//0.012,0.325,0.236 - SeaGreen - LEVEL3
//0.56,0.188,0.188 - Maroonish - LEVEL2
#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#include<string.h>
#include<GL/freeglut.h>
#include<math.h>
#include "SOIL.h"
#define pi  3.14159

static GLint theta = 0.0;
GLint xc = 0, yc = 0;
static int rc=195;
GLfloat Angle=0.0, Speed=0.09;
GLdouble radiusb=10.0,bx=0.0,by=0.0 ,vx=cos(Angle * pi / 180) * Speed, vy=sin(Angle * pi / 180) * Speed;
GLdouble x1=-20.0,Y1=-190.0,x2=20.0,y2=-190.0,x3=20.0,y3=-200.0,x4=-20.0,y4=-200.0;
GLdouble mat[4][4] = {{x1, x2, x3, x4},{Y1, y2, y3, y4},{1.0, 1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0}}, rot_mat[4][4], res[4][4];
GLint score = 0,hscore=0;
char toPrint[20];
int screen = 0;
FILE *fin,*fout;

//To print characters on the Screen
void printString(unsigned char *string, GLfloat tx, GLfloat ty, GLfloat tz, GLfloat sx, GLfloat sy) {
	glPushMatrix();
	glTranslatef(tx,ty,tz);
	glScalef(sx,sy,0);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)string);
	glTranslatef(-tx,-ty,-tz);
	glPopMatrix();
}

//Drawing outer circle
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


//Drawing Paddle
void paddle() {
	glColor3f(1.0,1.0,1.0);
	glPointSize(10.0);
	glBegin(GL_POLYGON);
	glVertex2d(x1,Y1);
	glVertex2d(x2,y2);
	glVertex2d(x3,y3);
	glVertex2d(x4,y4);
	glEnd();
}


//Reshape Function
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


//Drawing Ball
void ball(GLdouble x, GLdouble y) {
	glColor3f(1.0,1.0,1.0);
	int i;
	int triangleAmount = 20;
	GLfloat twicePi = 2.0f * 3.14;
	glBegin(GL_TRIANGLE_FAN);
	glVertex2d(x, y);
	for(i = 0; i <= triangleAmount;i++) {
		glVertex2d(
				x + (radiusb * cos(i *  twicePi / triangleAmount)),
				y + (radiusb * sin(i * twicePi / triangleAmount))
			  );
	}
	glEnd();
}


//To Rotate an object 
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
		}
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

//Collision Detection
void getLine(double x1, double y1, double x2, double y2, double &a, double &b, double &c) {
	// (x- p1X) / (p2X - p1X) = (y - p1Y) / (p2Y - p1Y)
	a = y1 - y2;
	b = x2 - x1;
	c = x1 * y2 - x2 * y1;
}
double dist(double pct1X, double pct1Y, double pct2X, double pct2Y, double pct3X, double pct3Y) {
	double a, b, c;
	getLine(pct2X, pct2Y, pct3X, pct3Y, a, b, c);
	return abs(pow(a * pct1X + b * pct1Y + c,2)) / (a * a + b * b);
}
void vRecalc(){
	Angle = atan2(vy * pi / 180, vx * pi / 180);
	Angle+=(1+rand()%30)*pi/180;
	vx = cos(Angle) * Speed;
	vy = sin(Angle) * Speed;
	return;
}
void checkCollision(double bx,double by){
	double dPaddle=dist(bx, by, x1, Y1, x2, y2);
	double d=pow(bx,2)+pow(by,2);
	if(d<pow(rc-radiusb,2)){
		if(dPaddle<=pow(radiusb,2)){
			vRecalc();
			vx*=-1.0;
			vy*=-1.0;
			score++;
		}
	}
	else {
		//Collison Occured. So, Game Over Screen
		screen=3;
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1.0,1.0,0.0);
		if(score>hscore) {
			fout=fopen("score.txt","w");
			if(fout==NULL){
				printf("Error !!");
				exit(1);
			}
			fprintf(fout,"%d",score);
			fclose(fout);
			hscore=score;
		}
		printString((unsigned char*)"GAME OVER", -300, 50, 0, 0.8, 0.8);
		glPushMatrix();
		glTranslatef(-200,-50,0);
		glScalef(0.5,0.5,0);
		glColor3f(1.0,1.0,1.0);
		glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)"Score : ");
		sprintf(toPrint, "%d", score);
		glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)(toPrint));
		glTranslatef(200,50,0);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-130,-100,0);
		glScalef(0.2,0.2,0);
		glColor3f(1.0,1.0,1.0);
		glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)"High Score : ");
		sprintf(toPrint, "%d", hscore);
		glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)(toPrint));
		glTranslatef(130,100,0);
		glPopMatrix();
		glColor3f(0.5,0.5,0);
		glLineWidth(2);
		printString((unsigned char*)"Press ' r ' to replay", -150, -180, 0, 0.08, 0.08);
		printString((unsigned char*)"Press ' ESC ' to exit", +100, -180, 0, 0.08, 0.08);
		glFlush();
		glutMainLoop();
	}
}

//To Draw Image
void drawBG() {
	GLuint tex_2d = SOIL_load_OGL_texture (
			"pesit-logo1.jpg",
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
			);
	glBindTexture(GL_TEXTURE_2D, tex_2d);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2d(0,0);        glVertex2f(-35,0);
	glTexCoord2d(1,0);        glVertex2f(35,0);
	glTexCoord2d(1,1);        glVertex2f(35,90);
	glTexCoord2d(0,1);        glVertex2f(-35,90);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

//Starting/Introduction Page
void screen0() {
	drawBG();
	printString((unsigned char*)"Computer Graphics Project", -150, 200, 0, 0.2, 0.2);
	glColor3f(0.5,0.5,1.0);
	glLineWidth(3.0);
	printString((unsigned char*)"Circle Pong", -100, 130, 0, 0.3, 0.3);
	glLineWidth(1);
	glColor3f(1,0.55,0);
	printString((unsigned char*)"Done By - ", -400, -50, 0, 0.1, 0.1);
	glColor3f(1,1,1);
	printString((unsigned char*)"1. Ayush Agrawal  (1PE15CS033)", -400, -100, 0, 0.15, 0.15);
	printString((unsigned char*)"2. Karthik Shivaram (1PE15CS068)", -400, -150, 0, 0.15, 0.15);
	glColor3f(1,0.55,0);
	printString((unsigned char*)"Under the guidance of - ", 100, -50, 0, 0.1, 0.1);
	glColor3f(1,1,1);
	printString((unsigned char*)"Dr. Sarasvathi V", 100, -100, 0, 0.15, 0.15);
	glColor3f(0.5,0.5,0);
	printString((unsigned char*)"Press ' x ' to move to next screen", -100, -200, 0, 0.08, 0.08);
	glColor3f(1.0,1.0,1.0);
}

//Informational Page
void screen1() {
	x1=-20.0,Y1=-190.0,x2=20.0,y2=-190.0,x3=20.0,y3=-200.0,x4=-20.0,y4=-200.0;
	radiusb = 10;
	Speed = 0.09;
	score=bx=by=theta=Angle=0;
	vx=cos(Angle * pi / 180) * Speed;
	vy=sin(Angle * pi / 180) * Speed;

	glColor3f(1,0.55,0);
	printString((unsigned char*)"Objective", -400, 150, 0, 0.4, 0.4);
	glColor3f(1,1,1);
	printString((unsigned char*)"Keep the ball within", -400, 90, 0, 0.15, 0.15);
	printString((unsigned char*)"the defined circle", -400, 60, 0, 0.15, 0.15);

	glColor3f(1,0.55,0);
	printString((unsigned char*)"How To Play", 170, 150, 0, 0.3, 0.3);
	glColor3f(1,1,1);
	glLineWidth(7);
	printString((unsigned char*)"<    >", 230, 90, 0, 0.28, 0.28);
	glLineWidth(1);
	printString((unsigned char*)"Move      Move", 220, 60, 0, 0.15, 0.15);
	printString((unsigned char*)"Left       Right", 220, 40, 0, 0.15, 0.15);

	drawCircle();
	glLineWidth(2.0);
	glColor3f(1,1,1);
	glBegin(GL_TRIANGLES);
	glVertex2f(-10,-20);
	glVertex2f(-10,20);
	glVertex2f(15,0);
	glEnd();
	glColor3f(0.5,0.5,0);
	printString((unsigned char*)"Click to Play", -40, -45, 0, 0.1, 0.1);
	glColor3f(0.5,0.5,0);
	printString((unsigned char*)"OR Press ' s ' to play", -70, -65, 0, 0.1, 0.1);
	glColor3f(0.5,0.5,0);
	printString((unsigned char*)"Press ' ESC ' to exit", -50, -230, 0, 0.08, 0.08);
	glColor3f(1.0,1.0,1.0);
	glFlush();
}

//To rotate Paddle
void rotatePaddle() {
	if(score<=3) {
		glClearColor(0.012,0.259,0.325,1);
	}else if(score<=6) {
		glClearColor(0.56,0.188,0.188,1);
		radiusb = 5;
		Speed = 0.16;
	}else {
		glClearColor(0.012,0.325,0.236,1);
	}
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	drawCircle();
	ball(bx, by);
	rotate();
	paddle();
	glFlush();
	glutSwapBuffers();
}

/*//Idle Function
void idleFunc() {
	if(screen==2) {
		checkCollision(bx, by);
		bx+=vx;
		by+=vy;
		glutPostRedisplay();
	}
}

//Initialization Function
void init() {
	glClearColor(0.012,0.259,0.325, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}*/

//Display Function
void myDisp() {
	if(screen==0){
		glClearColor(0,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		screen0();
	}else{
		if(score<=3) {
			glClearColor(0.012,0.259,0.325,1);
		}else if(score<=6) {
			glClearColor(0.56,0.188,0.188,1);
			radiusb = 5;
			Speed = 0.16;
		}else{
			glClearColor(0.012,0.325,0.236,1);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawCircle();
		ball(bx, by);
		paddle();
		glPushMatrix();
		glTranslatef(150,200,0);
		glScalef(0.2,0.2,0);
		glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)"Score : ");
		sprintf(toPrint, "%d", score);
		glLineWidth(7);
		glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)(toPrint));
		glTranslatef(-200,-200,0);
		glLineWidth(3);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(170,170,0);
		glScalef(0.1,0.1,0);
		glColor3f(1.0,1.0,1.0);
		glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)"High Score : ");
		sprintf(toPrint, "%d", hscore);
		glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)(toPrint));
		glTranslatef(-170,-170,0);
		glPopMatrix();
		if(score<=3) {
			glColor3f(0.55,0.55,0);
			printString((unsigned char*)"LEVEL 1", 220, 100, 0, 0.3, 0.3);
		}else if(score<=6){
			glColor3f(0.55,0.55,0);
			printString((unsigned char*)"LEVEL 2", 220, 100, 0, 0.3, 0.3);
			glColor3f(1,1,1);
			printString((unsigned char*)"Higher Ball speed", 240, 60, 0, 0.15, 0.15);
		}else{
			glColor3f(0.55,0.55,0);
			printString((unsigned char*)"LEVEL 3", 220, 100, 0, 0.3, 0.3);
			glColor3f(1,1,1);
			printString((unsigned char*)"Paddle direction", 240, 60, 0, 0.15, 0.15);
			printString((unsigned char*)"reverses", 240, 40, 0, 0.15, 0.15);
		}
	}
	glFlush();
	glutSwapBuffers();
}

//Idle Function
void idleFunc() {
        if(screen==2) {
                checkCollision(bx, by);
                bx+=vx;
                by+=vy;
                glutPostRedisplay();
        }
}

//Initialization Function
void init() {
        glClearColor(0.012,0.259,0.325, 1);
        glClear(GL_COLOR_BUFFER_BIT);
}

//Function to respond to General Keys
void keys(unsigned char key, int x, int y) {
	if(screen==0){
		if(key=='x'){
			screen=1;
			glClearColor(0.012,0.259,0.325,1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			screen1();
		}
	}else if(screen==1){
		if(key=='s'){
			screen=2;
		}else if(key==27) {
			exit(0);
		}
	}else if(screen==3){
		if(key=='r'){
			screen=1;
			glClearColor(0.012,0.259,0.325,1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			screen1();
		}else if(key==27) {
			exit(0);
		}
	}
}


//Function to respond to Special Keys
void specialkey(int key, int x, int y)
{
	if(screen==2){
		if(key==GLUT_KEY_LEFT) {
			if(score>6){
				if(theta>360.0) theta-=360.0;
				theta+=2.0;
			}else{
				if(theta<360.0) theta+=360.0;
				theta-=2.0;
			}
		}
		if(key==GLUT_KEY_RIGHT) {
			if(score>6){
				if(theta<360.0) theta+=360.0;
				theta-=2.0;
			}else {
				if(theta>360.0) theta-=360.0;
				theta+=2.0;
			}
		}
		rotatePaddle();
	}
}

//Function to respond to mouse clicks
void mouse(int btn, int state, int x, int y) {
	if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
		if(screen==1) {
			if(x>=935 && x<=995 && y>=500 && y<=585)
				screen=2;
		}
	}

}

//Main function/Starting Point
int main(int argc, char **argv) {
	fin=fopen("score.txt","r");
	if(fin==NULL){
		printf("Error !!");
		exit(1);
	}
	fscanf(fin,"%d",&hscore);
	fclose(fin);
	glutInit(&argc,argv);
	glutInitWindowSize(500,500);
	glutCreateWindow("Circle Pong");
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	init();
	glutIdleFunc(idleFunc);
	glutReshapeFunc(myReshape);
	glutKeyboardFunc(keys);
	glutMouseFunc(mouse);
	glutSpecialFunc(specialkey);
	glutDisplayFunc(myDisp);
	glutFullScreen();
	glutMainLoop();
	return 1;
}
