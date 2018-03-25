
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <math.h>
#ifdef __linux__
#include <unistd.h>
#endif


bool checkCollision(int x, int y, int r)
{   //fuction to check whether the ball is beyond the circle or not, returns true if on the circle false if inside
    bool flag=true;
    float val = x^2+y^2;
    if(val<r^2)
        flag=false;
    return flag;
    }


void idle()
{
usleep(10000);
if(ball.y <= ball_maxd)
    {
        /* Check ball's co-ord is between bar's width */
        if(checkCollision(x.ball,y.ball,r)) // Bar missing
        {
            //add code to display the score and show game screen.
            //1. player score
            //2. Redisplay splash screen after any key press
        }
        else{
                //velocity+=0.1;
                //redisplay a new label or include object or change conditions whatever
            }

        /* Ball bounce */
if(checkCollision(x.ball,y.ball,r))
    {
    ball.xstep=-1*ball.xstep;
    ball.ystep=-1*ball.ystep;
    ball.x += ball.xstep * ball.velocity;
    ball.y += ball.ystep * ball.velocity;
    }
    glutPostRedisplay();

}



/*code to be added in main.cpp is below, find out where what goes okay, almost all the essentials are there
struct Ball
{
    float r;
    float x;
    float y;
    float velocity;
    float xstep;
    float ystep;
};

Ball ball = {0.5, 0, 0, 1.0, 0.10, 0.10};

flushing the ball:
// Ball
    glPushMatrix();
    glTranslatef(ball.x, ball.y, zoom);//zoom is some variable with const value for camera pos change it whatever
    glColor3f(0.8, 0, 0.1);
    glutSolidSphere(ball.r, 20, 20);
    glPopMatrix();

  //reshape function if required:
    void reshape(int w, int h)
{
    float aspectRatio;
    h = (h == 0) ? 1 : h;
    w = (w == 0) ? 1 : w;
    // Setting window dimention as viewport //
    glViewport( 0, 0, w, h );
    aspectRatio = (float)w/(float)h;
    // Projection mode //
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set perspective //
    gluPerspective(45, aspectRatio, 1.0, 100.0);
    // Back to modelview mode //
    glMatrixMode(GL_MODELVIEW);
}
*/
