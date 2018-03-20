#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>
int screen = 0;
void drawStrokeText(char *string, int x, int y, int z)
{
    glPushMatrix();
    glTranslatef(x,y+8,z);
    glScalef(0.33f,0.33f,z);
    glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)string);
    glPopMatrix();
}

void drawBitmapText1(char *string, float x, float y)
{
    char *c;
    glRasterPos2f(x,y);
    for (c=string; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*c);
    }
}

void mouse(int btn,int state,int x,int y)
{
    if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        screen = 1;
        glutPostRedisplay();
    }
}
void first_screen()
{
    glColor3f(1.0,1.0,0.0);
    drawStrokeText("Circle Pong",360,500,0);
    drawBitmapText1("Ping Pong but with a twist",430,470);
    glColor3f(1.0,1.0,1.0);
    drawBitmapText1("Done by-",480,200);
    drawBitmapText1("Ayush Agarwal",300,150);
    drawBitmapText1("Karthik Shivaram",300,120);
    drawBitmapText1("1PE15CS032",610,150);
    drawBitmapText1("1PE15CS068",610,120);
    glColor3f(1.0,0.0,1.0);
    drawBitmapText1("Press left mouse button to move to next screen",580,20);
}

void reshape(int w,int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(w<=h)
		 glOrtho(-2.0,2.0,-2.0*(GLfloat)h/(GLfloat)w,2.0*(GLfloat)h/(GLfloat)w,-10.0,10.0);
	else
		 glOrtho(-2.0*(GLfloat)w/(GLfloat)h,2.0*(GLfloat)w/(GLfloat)h,-2.0,2.0,-10.0,10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,1000,10.0,650,-2000,1500);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    first_screen();
    zglutSwapBuffers();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(1000,650);
    glutInitWindowPosition(0,0);
    glutCreateWindow("CG Project");
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}

