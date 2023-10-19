/*********
   CTIS164 - Homework 1
----------
STUDENT : Baris Cihanoglu
SECTION : 2
HOMEWORK: 1
NUMBER  : 22202987
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:

If use arrow keys, you can change the direction of the plane.

Pressing w makes plane faster and pressing s makes it slower.

Pressing n changes day to night.
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define DAY 0
#define NIGHT 1

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD     30 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

int xP = -350, yP = 0; //coordinates of the plane
int animation = 1; //animation on off
int speed = 1; //speed of the plane
int appState = DAY;

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
int x, y;

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

void display_wall(void) {
    // the wall
    glColor3f(186 / 256., 12 / 256., 59 / 256.);
    glRectf(-400, 400, 400, 305);
    glRectf(-400, 400, -205, -400);
    glRectf(200, 400, 400, -400);
    glRectf(-400, -300, 400, -400);

    //label for the name
    glColor3f(192 / 256., 205 / 256., 255 / 256.);
    glRectf(-120, 365, 120, 320);

    //ropes of the label
    glColor3f(0, 0, 0);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(-120, 365);
    glVertex2f(0, 380);

    glVertex2f(120, 365);
    glVertex2f(0, 380);
    glEnd();

    //pin of the label
    glColor3f(1, 1, 1);
    circle(0, 380, 3);

    //frame of label
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-120, 365);
    glVertex2f(-120, 320);
    glVertex2f(120, 320);
    glVertex2f(120, 365);
    glEnd();
    glLineWidth(1);

    glColor3f(0, 0, 0);
    vprint(-70, 340, GLUT_BITMAP_9_BY_15, "BARIS CIHANOGLU");

    vprint(-380, -150, GLUT_BITMAP_9_BY_15, "-PRESS n TO ");
    vprint(-380, -170, GLUT_BITMAP_9_BY_15, " CHANGE TIME");
    vprint(-380, -200, GLUT_BITMAP_9_BY_15, "-USE W AND S TO");
    vprint(-380, -220, GLUT_BITMAP_9_BY_15, " CHANGE SPEED");
    vprint(-380, -300, GLUT_BITMAP_9_BY_15, " Current Speed: %d", speed);
}

void display_background(void) {
    // day and night backgrounds
    switch (appState)
    {
    case (DAY):
        glColor3f(164 / 256., 228 / 256., 255 / 256.);
        glRectf(-200, 300, 200, -300);
        glColor3f(1, 1, 0);
        circle(190, 280, 30);
        break;
    case (NIGHT):
        glColor3f(0.1, 0.1, 0.1);
        glRectf(-200, 300, 200, -300);
        break;
    }
}

void display_building1(void)
{
    //building 1
    glColor3f(158 / 256., 158 / 256., 158 / 256.);
    glBegin(GL_POLYGON);
    glVertex2f(-70, 20);
    glVertex2f(-60, 30);
    glVertex2f(-60, -800);
    glVertex2f(-70, -800);
    glEnd();

    glColor3f(0.7, 0.7, 0.7);
    glBegin(GL_POLYGON);
    glVertex2f(-60, 30);
    glVertex2f(-20, 30);
    glVertex2f(-20, -800);
    glVertex2f(-60, -800);
    glEnd();

    glLineWidth(2);
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    glVertex2f(-70, 20);
    glVertex2f(-60, 30);

    glVertex2f(-60, 30);
    glVertex2f(-20, 30);

    glVertex2f(-70, 20);
    glVertex2f(-70, -800);

    glVertex2f(-60, 30);
    glVertex2f(-60, -800);

    glVertex2f(-20, 30);
    glVertex2f(-20, -800);
    glEnd();

    glLineWidth(1);
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_LINES);
    int i;
    for (i = 25; i > -400; i -= 5)
    {
        glVertex2f(-58, i);
        glVertex2f(-22, i);
    }
    glEnd();
}

void display_building2(void)
{
    //building 2
    glLineWidth(2);
    glColor3f(158 / 256., 158 / 256., 158 / 256.);
    glBegin(GL_POLYGON);
    glVertex2f(10, 15);
    glVertex2f(20, 25);
    glVertex2f(20, -800);
    glVertex2f(10, -800);
    glEnd();

    glColor3f(0.7, 0.7, 0.7);
    glBegin(GL_POLYGON);
    glVertex2f(20, 25);
    glVertex2f(60, 25);
    glVertex2f(60, -800);
    glVertex2f(20, -800);
    glEnd();

    glLineWidth(2);
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    glVertex2f(10, 15);
    glVertex2f(20, 25);

    glVertex2f(20, 25);
    glVertex2f(60, 25);

    glVertex2f(10, 15);
    glVertex2f(10, -800);

    glVertex2f(20, 25);
    glVertex2f(20, -800);

    glVertex2f(60, 25);
    glVertex2f(60, -800);
    glEnd();

    glLineWidth(1);
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_LINES);
    int i;
    for (i = 20; i > -400; i -= 5)
    {
        glVertex2f(22, i);
        glVertex2f(58, i);
    }
    glEnd();
    glLineWidth(1);
}

void display_window(void)
{
    //window frame
    glLineWidth(3);
    glColor3ub(1, 1, 1);
    glBegin(GL_LINES);
    glVertex2f(-200, 150);
    glVertex2f(200, 150);
    glVertex2f(-200, 0);
    glVertex2f(200, 0);
    glVertex2f(-200, -150);
    glVertex2f(200, -150);

    glVertex2f(-100, 300);
    glVertex2f(-100, -300);

    glVertex2f(100, 300);
    glVertex2f(100, -300);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(-202, 300);
    glVertex2f(200, 300);
    glVertex2f(200, -300);
    glVertex2f(-200, -300);
    glEnd();
    glLineWidth(4);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-206, 306);
    glVertex2f(206, 306);
    glVertex2f(206, -306);
    glVertex2f(-206, -306);
    glEnd();
    glLineWidth(9);
    glColor3ub(48 / 256., 67 / 256., 35 / 256.);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-204, 304);
    glVertex2f(204, 304);
    glVertex2f(204, -304);
    glVertex2f(-204, -304);
    glEnd();
    glLineWidth(1);
}

void display_plane(int x, int y)
{
    //the plane
    glColor3f(169 / 256., 169 / 256., 169 / 256.);
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + 10, y - 10);
    glVertex2f(x + 10, y - 30);
    glVertex2f(x, y - 16);
    glEnd();
    glRectf(x + 10, y - 10, x + 61, y - 30);
    circle(x + 61, y - 20, 10);
    //plane's windows
    glColor3f(1, 1, 1);
    glRectf(x + 12, y - 14, x + 17, y - 19);
    glRectf(x + 19, y - 14, x + 24, y - 19);
    glRectf(x + 26, y - 14, x + 31, y - 19);
    glRectf(x + 33, y - 14, x + 38, y - 19);
    glRectf(x + 40, y - 14, x + 45, y - 19);
    glRectf(x + 47, y - 14, x + 52, y - 19);
    glRectf(x + 54, y - 14, x + 59, y - 19);
    glBegin(GL_POLYGON);
    glVertex2f(x + 61, y - 14);
    glVertex2f(x + 65, y - 14);
    glVertex2f(x + 68, y - 19);
    glVertex2f(x + 61, y - 19);
    glEnd();
    //plane's wing
    glColor3f(200 / 256., 200 / 256., 200 / 256.);
    glBegin(GL_TRIANGLES);
    glVertex2f(x + 35, y - 21);
    glVertex2f(x + 48, y - 21);
    glVertex2f(x + 32, y - 40);
    glEnd();
}

void display_cloud(int x, int y, int r)
{
    //cloud
    glColor3f(1, 1, 1);
    circle(x, y, r);
    circle(x + 16, y, r);
    circle(x + 8, y + 7, r);
}

//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    display_background();

    display_building1();

    display_cloud(-200, 100, 10);
    display_cloud(-144, 250, 10);
    display_cloud(-50, 200, 10);
    display_cloud(30, 120, 15);
    display_cloud(-170, 143, 14);
    display_cloud(140, 200, 14);
    display_cloud(170, 100, 15);
    display_cloud(70, 180, 12);

    display_plane(xP, yP);

    display_building2();

    display_wall();

    display_window();

    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);
    // stop animation when space pressed
    if (key == ' ')
        animation = !animation;
    // increase speed
    if (key == 'w' && speed < 40)
        speed += 1;
    // decrease speed
    if (key == 's' && speed > 1)
        speed -= 1;

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);
    // change day-night
    if (key == 'n')
        appState = !appState;

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP:
        up = true;
        break;
    case GLUT_KEY_DOWN:
        down = true;
        break;
    case GLUT_KEY_LEFT:
        left = true;
        break;
    case GLUT_KEY_RIGHT:
        right = true;
        break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP:
        up = false;
        break;
    case GLUT_KEY_DOWN:
        down = false;
        break;
    case GLUT_KEY_LEFT:
        left = false;
        break;
    case GLUT_KEY_RIGHT:
        right = false;
        break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.


    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    if (animation)
    {
        xP += speed;

        if (up && yP < 400 && yP > -400)
            yP += speed;
        if (down && yP < 400 && yP > -400)
            yP -= speed;
        if (right && xP < 400 && xP > -400)
            xP += speed;
        if (left && xP < 400 && xP > -400)
            xP -= speed * 2;

        if (xP > 350)
        {
            xP = -350;
            yP = 0;
        }
    }

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("The Great Accident");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);


#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}