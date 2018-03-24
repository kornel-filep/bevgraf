#include <GLUT/glut.h>
#include "bevgrafmath2017.h"

#include <iostream>
#include <vector>
#include <string>

GLsizei winWidth = 800, winHeight = 600;

//vec2 points[4] = { {50, 400}, {100, 300}, {150, 250}, {200, 300}};
//vec2 points[18] = {};
vec2 points[18] = {{95.000000,104.000000},{94.000000,123.000000},
 {93.000000,143.000000},{120.000000,142.000000},{145.000000,142.000000},{129.000000,162.000000},
 {137.000000,200.000000},{145.000000,242.000000},{201.000000,381.000000},{348.000000,354.000000},{461.000000,302.000000}
 ,{569.000000,231.000000},{568.000000,110.000000},{238.000000,230.000000},{226.000000,283.000000},{276.000000,318.000000},
 {368.000000,309.000000},{385.000000,240.000000},};


double T0 = -1;
double T1 = -0.5;
double T2 = 1;
double T3 = T0;
mat24 G;
mat4 M = {};

GLint dragged = -1;

//stackoverflowos fuggveny
unsigned long long choose(unsigned long long n, unsigned long long k) {
    if (k > n) {
        return 0;
    }
    unsigned long long r = 1;
    for (unsigned long long d = 1; d <= k; ++d) {
        r *= n--;
        r /= d;
    }
    return r;
}


void bezier(int pontokSzama, vec2 p1, vec2 p2, vec2 p3, vec2 p4){
    glBegin(GL_LINE_STRIP);
    for (double t = 0; t <= 1; t+=0.01) {
        vec2 B = 0;
        vec2 points_tmp[] = {p1, p2, p3, p4 /*ez kesobb erintore atirni*/};
        for (int i = 0; i < pontokSzama; i++) {
            double KomboBern = choose(3.0,i)*pow(t,i)*pow(1.0-t,3.0-i);
            B += KomboBern*points_tmp[i];
        }
        glVertex2d(B.x, B.y);
    }
    glEnd();
}

void hermite(vec2 p1, vec2 p2, vec2 p3, vec2 p4){
    G = {p1,p2,p3,p4};

    vec2 Q;
    glBegin(GL_LINE_STRIP);
    for (double t = T0; t<=T2; t+= 0.001){
        vec4 tk = {static_cast<float>(pow(t,3)), static_cast<float>(pow(t,2)), static_cast<float>(pow(t,1)), static_cast<float>(pow(t,0))};

        Q = G*M*tk;

        glVertex2d(Q.x,Q.y);
    }
    glEnd();
}




void update(int n){

}

void display()
{
    GLint i;
    glClear(GL_COLOR_BUFFER_BIT);

    glFlush();

    glColor3f(256.0, 256.0, 256.0);

    points[4] = (points[3]-points[2]) + points[3];
    points[7] = (points[6]-points[5]) + points[6];

    glBegin(GL_POINTS);
    for (i = 0; i < 18; i++)
        glVertex2i(points[i].x, points[i].y);
    glEnd();

    /*glBegin(GL_LINE_LOOP);
    for (i = 0; i < 13; i++)
        glVertex2i(points[i].x, points[i].y);
    glEnd();
    */



    glBegin(GL_LINE_LOOP);
    for (i = 13; i < 18; i++)
        glVertex2i(points[i].x, points[i].y);
    glEnd();

    bezier(4,points[0],points[1],points[2],points[3]); // pontjai 0,1,2,3
    bezier(4,points[3],points[4],points[5],points[6]); // pontjai 3,4,5,6 ahol 3 ugyan az mint az elozoe, 4 pedig szamoljuk az erintot.
    bezier(4,points[6],points[7],points[8],points[9]); // kozos a 6, a 7et meg szamoljuk
    vec2 erinto = points[10]-points[9];
    hermite(points[12],points[11],points[10], erinto);

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
            exit(0);
            break;
    }
    glutPostRedisplay();
}

GLint getActivePoint1(vec2 p[], GLint size, GLint sens, GLint x, GLint y)
{
    GLint i, s = sens * sens;
    vec2 P = { (float)x, (float)y };

    for (i = 0; i < size; i++)
        if (dist2(p[i], P) < s)
            return i;
    return -1;
}



void init()
{
    glClearColor(0.5, 0.5, 0.5, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, winWidth, 0.0, winHeight);
    glShadeModel(GL_FLAT);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(8.0);
    glLineWidth(1.5);
    glLineStipple(1, 0xFF00);
    M ={{static_cast<float>(pow(T0,3.0)),static_cast<float>(pow(T1,3.0)),static_cast<float>(pow(T2,3.0)),static_cast<float>(3.0*pow(T3,2.0))},
            {static_cast<float>(pow(T0,2.0)),static_cast<float>(pow(T1,2.0)),static_cast<float>(pow(T2,2.0)),static_cast<float>(2.0*pow(T3,1.0))},
            {static_cast<float>(pow(T0,1.0)),static_cast<float>(pow(T1,1.0)),static_cast<float>(pow(T2,1.0)),static_cast<float>(1)},
            {static_cast<float>(pow(T0,0)),static_cast<float>(pow(T1,0)),static_cast<float>(pow(T2,0)),static_cast<float>(0)}};

    M = inverse(M);
    printMathObject(M);

}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{

    GLint i;
    if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN){
        if ((i = getActivePoint1(points, 18, 8, xMouse, winHeight - yMouse)) != -1){
            dragged = i;}
    }



    if (button == GLUT_LEFT_BUTTON && action == GLUT_UP)
        dragged = -1;
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse)
{
    GLint i;
    if (dragged >= 0) {
        points[dragged].x = xMouse;
        points[dragged].y = winHeight - yMouse;
        glutPostRedisplay();
    }
}



int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Haza utam :(");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(processMouse);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(processMouseActiveMotion);
    glutMainLoop();
    return 0;
}
