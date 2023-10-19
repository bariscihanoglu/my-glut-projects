/*********
   CTIS164 - Homework3
----------
STUDENT : Baris Cihanoglu
SECTION : 2
HOMEWORK: 3
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:

The ufo follows a path that is found by using a 3rd degree function.

Birds will spawn randomly every time you open the game.

The laser is shot from the friendly ufo's position.

There is a scoreboard where you can see how many times you shot the ufo.

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
#include <time.h>
#include "vec.h"

#define WINDOW_WIDTH   1000
#define WINDOW_HEIGHT  600

#define FRIEND_X -450

#define TIMER_PERIOD    30 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R          0.0174532
#define BULLET_SPEED 10

typedef struct {
    int x;
    int y;
} bird_t;

typedef struct {
    vec_t pos;
    vec_t vel;
    bool  move;
} bullet_t;

bullet_t B = { { -1000 , -1000 }, { 0, 0 }, 0 };
bird_t   birds[30];

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight, ufoX = -400, ufoY = 0, score = 0, gameState, friendY = 0, ufoShot = 0;
vec_t bulletVec;

// ufo's movement
int f(int x)
{
    int a = 1, b = 5, c = 4;
    ufoY = a * x * x * x * -0.0001 + b * x * x * -0.001 + c * x;
    return ufoY;
}

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

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
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

// display 1 bird
void displayBird(int x, int y)
{
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(x - 12, y + 2);
    glVertex2f(x - 2, y);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(x - 2, y);
    glVertex2f(x, y - 4);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(x, y - 4);
    glVertex2f(x + 2, y);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(x + 2, y);
    glVertex2f(x + 12, y + 2);
    glEnd();
}

// the background, also displays 30 birds
void displayBackground(void)
{
    glBegin(GL_QUADS);
    glColor3f(5 / 255., 115 / 255., 240 / 255.);
    glVertex2f(500, -300);
    glVertex2f(-500, -300);

    glColor3f(230 / 255., 224 / 255., 255 / 255.);
    glVertex2f(-500, 300);
    glVertex2f(500, 300);
    glEnd();

    for (int i = 0; i < 30; i++)
        displayBird(birds[i].x, birds[i].y);
}

// the scoreboard
void displayPanels(void)
{
    glColor3f(1, 1, 1);
    vprint(395, 240, GLUT_BITMAP_HELVETICA_18, "SCORE");
    vprint2(388, 200, 0.35, "%003d", score);
    vprint(270, -200, GLUT_BITMAP_8_BY_13, "Press up-down keys to move");
    vprint(270, -220, GLUT_BITMAP_8_BY_13, "Space to shoot");
}

//displays the enemy ufo (red).
void drawUfo(int x, int y)
{
    glColor3f(28 / 255., 33 / 255., 38 / 255.);
    circle(x, y - 5, 25);
    glColor3f(225 / 255., 52 / 255., 52 / 255.);
    circle(x, y, 25);
    glColor3f(1, 1, 1);
    circle_wire(x, y, 25);

    glColor3f(24 / 255., 32 / 255., 37 / 255.);
    glBegin(GL_POLYGON);
    glVertex2f(x - 25, y);
    glVertex2f(x - 45, y - 25);
    glVertex2f(x + 45, y - 25);
    glVertex2f(x + 25, y);
    glEnd();

    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    glVertex2f(x - 25, y);
    glVertex2f(x - 45, y - 26);
    glVertex2f(x + 45, y - 26);
    glVertex2f(x + 25, y);
    glEnd();

    glColor3f(235 / 255., 87 / 255., 87 / 255.);
    circle(x, y - 15, 5);
    circle(x + 20, y - 15, 5);
    circle(x - 20, y - 15, 5);
}

//displays the friendly ufo (green).
void drawFriendUfo(int y)
{
    glColor3f(28 / 255., 33 / 255., 38 / 255.);
    glBegin(GL_POLYGON);
    glVertex2f(FRIEND_X - 5, y - 20);
    glVertex2f(FRIEND_X + 5, y - 20);
    glVertex2f(FRIEND_X + 5, y - 28);
    glVertex2f(FRIEND_X - 5, y - 28);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(FRIEND_X - 8, y - 28);
    glVertex2f(FRIEND_X + 30, y - 28);
    glVertex2f(FRIEND_X + 30, y - 40);
    glVertex2f(FRIEND_X - 8, y - 40);
    glEnd();

    glColor3f(91 / 255., 247 / 255., 133 / 255.);
    circle(FRIEND_X, y, 20);
    glColor3f(1, 1, 1);
    circle_wire(FRIEND_X, y, 20);

    glColor3f(24 / 255., 32 / 255., 37 / 255.);
    glBegin(GL_POLYGON);
    glVertex2f(FRIEND_X - 20, y);
    glVertex2f(FRIEND_X - 36, y - 20);
    glVertex2f(FRIEND_X + 36, y - 20);
    glVertex2f(FRIEND_X + 20, y);
    glEnd();

    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    glVertex2f(FRIEND_X - 20, y);
    glVertex2f(FRIEND_X - 36, y - 21);
    glVertex2f(FRIEND_X + 36, y - 21);
    glVertex2f(FRIEND_X + 20, y);
    glEnd();

    glColor3f(32 / 255., 213 / 255., 80 / 255.);
    circle(FRIEND_X, y - 12, 4);
    circle(FRIEND_X + 16, y - 12, 4);
    circle(FRIEND_X - 16, y - 12, 4);
}

// displays the bullet which is shot from friendly ufo.
// uses vector operations.
void drawBullet(bullet_t b) {

    polar_t polb;
    polb = rec2pol(b.vel);

    vec_t top1 = pol2rec({ 15, polb.angle + 20 });
    vec_t top2 = pol2rec({ 15, polb.angle - 20 });
    vec_t bot1 = pol2rec({ -15, polb.angle + 20 });
    vec_t bot2 = pol2rec({ -15, polb.angle - 20 });

    glColor3f(1, 0, 0);
    glBegin(GL_POLYGON);
    glVertex2f(b.pos.x + top1.x, b.pos.y + top1.y);
    glVertex2f(b.pos.x + top2.x, b.pos.y + top2.y);
    glVertex2f(b.pos.x + bot1.x, b.pos.y + bot1.y);
    glVertex2f(b.pos.x + bot2.x, b.pos.y + bot2.y);
    glEnd();

    circle((b.pos.x + top1.x + b.pos.x + top2.x) / 2, (b.pos.y + top1.y + b.pos.y + top2.y) / 2, 4);
    circle((b.pos.x + bot1.x + b.pos.x + bot2.x) / 2, (b.pos.y + bot1.y + b.pos.y + bot2.y) / 2, 4);

    glColor3f(255 / 255., 189 / 255., 189 / 255.);
    glLineWidth(2);
    glBegin(GL_LINES);

    top1 = pol2rec({ 14, polb.angle + 10 });
    bot1 = pol2rec({ -10, polb.angle - 15 });
    glVertex2f(b.pos.x + top1.x, b.pos.y + top1.y);
    glVertex2f(b.pos.x + bot1.x, b.pos.y + bot1.y);

    top1 = pol2rec({ 6, polb.angle - 10 });
    bot1 = pol2rec({ -14, polb.angle + 15 });
    glVertex2f(b.pos.x + top1.x, b.pos.y + top1.y);
    glVertex2f(b.pos.x + bot1.x, b.pos.y + bot1.y);

    glEnd();
    glLineWidth(1);
}

//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    displayBackground();
    displayPanels();

    drawUfo(ufoX, f(ufoX));
    drawBullet(B);
    drawFriendUfo(friendY);

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
    // make the bullet spawn inside the gun and make it move.
    if (key == ' ')
    {
        if ((B.pos.x > 510 || B.pos.x < -510) && (B.pos.y > 310 || B.pos.y < -310)) {
            B.move = true;
            x = FRIEND_X;
            y = friendY;
            B.pos = { (float)(FRIEND_X + 30), (float)(friendY - 35) };
        }
    }


    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);



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

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.
    ufoX += 1;

    // if ufo finished its path, make it go to the beggining
    if (ufoX > 250)
    {
        ufoX = -340;
    }

    // move the friendly ufo.
    if (friendY < 270)
    {
        if (up)
            friendY += 10;
    }
    if (friendY > -250)
    {
        if (down)
            friendY -= 10;
    }

    // move the bullet.
    B.vel = mulV(BULLET_SPEED, unitV(subV({ 1. * ufoX, 1. * ufoY }, B.pos)));

    if (B.move)
    {
        B.pos = addV(B.pos, B.vel);
        if (B.pos.x > 510 || B.pos.x < -510)
        {
            B.pos = { -1000, -1000 };
            B.move = false;
        }
        else if (B.pos.y > 310 || B.pos.y < -310)
        {
            B.pos = { -1000, -1000 };
            B.move = false;
        }
        else if (ufoX > 240)
        {
            B.pos = { -1000, -1000 };
            B.move = false;
        }
        else if (B.pos.x > ufoX - 30 && B.pos.x < ufoX + 30 && B.pos.y > ufoY - 30 && B.pos.x < ufoY + 30)
        {
            B.pos = { -1000, -1000 };
            B.move = false;
            ufoShot++;
            score++;
            //score = ufoShot / 5;
        }
    }

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()
}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    srand(time(NULL));

    // randomize the birds' place
    for (int i = 0; i < 30; i++)
    {
        birds[i].x = rand() % 1000 - 500;
        birds[i].y = rand() % 600 - 300;
    }
}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Shoot the UFO! - Barýþ Cihanoðlu");

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