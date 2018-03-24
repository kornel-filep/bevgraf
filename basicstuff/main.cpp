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
 {137.000000,200.000000},{145.000000,242.000000},{201.000000,381.000000},{348.000000,354.000000},{461.000000,252.000000}
 ,{569.000000,181.000000},{580.000000,104.000000},{238.000000,230.000000},{226.000000,283.000000},{276.000000,318.000000},
 {368.000000,309.000000},{385.000000,240.000000},};


float T0 = 1;
float T1 = -0.5;
float T2 = -1;
float T3 = T0;
mat24 G;
mat4 M = {};

//Castel pontokhoz kell
vec2 points_r1[] = {0,0,0,0};
vec2 points_r2[] = {0,0,0};
vec2 points_r3[] = {0,0};
vec2 points_r4 = {0};

bool lines = false;
bool allpoints = false;
bool castel = false;

float tolas = 0.5;

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
    glVertex2f(p4.x, p4.y);
    glEnd();
}

void hermite(vec2 p1, vec2 p2, vec2 p3, vec2 p4){
    G = {p1,p2,p3,p4};

    vec2 Q;
    glBegin(GL_LINE_STRIP);
    for (float t = T2; t<=T0; t+= 0.001){
        vec4 tk = {t*t*t, t*t, t, 1};
        Q = G*M*tk;

        glVertex2d(Q.x,Q.y);
    }
    glEnd();
}

void Caste(vec2 p1, vec2 p2, vec2 p3, vec2 p4, vec2 p5){
    glBegin(GL_LINE_STRIP);
    for(double t = 0; t<=1; t+=0.01){
        vec2 points_tmp[] = {p1, p2, p3, p4, p5} ;
        for (int r = 1; r < 5; r++)
        {
            for (int i = 0; i < 5 - r; i++)
            {
                points_tmp[i].x = (1 - t) * points_tmp[i].x + t * points_tmp[i + 1].x;
                points_tmp[i].y = (1 - t) * points_tmp[i].y + t * points_tmp[i + 1].y;


            }


        }
        glVertex2d(points_tmp[0].x, points_tmp[0].y);
    }
    glVertex2d(p5.x,p5.y);
    glEnd();

    vec2 points_tmp2[] = {p1, p2, p3, p4, p5} ;

    for (int r = 1; r < 5; r++) {
        for (int i = 0; i < 5 - r; i++) {
            points_tmp2[i].x = (1 - tolas) * points_tmp2[i].x + tolas * points_tmp2[i + 1].x;
            points_tmp2[i].y = (1 - tolas) * points_tmp2[i].y + tolas * points_tmp2[i + 1].y;

        }
        if (r==1){
            points_r1[0] = points_tmp2[0];
            points_r1[1] = points_tmp2[1];
            points_r1[2] = points_tmp2[2];
            points_r1[3] = points_tmp2[3];
        } else if(r==2){
            points_r2[0] = points_tmp2[0];
            points_r2[1] = points_tmp2[1];
            points_r2[2] = points_tmp2[2];
        } else if(r==3){
            points_r3[0] = points_tmp2[0];
            points_r3[1] = points_tmp2[1];
        } else if(r==4){
            points_r4 = points_tmp2[0];
        }
    }

    if(castel){
    glBegin(GL_LINE_STRIP);
    glVertex2f(points_r1[0].x,points_r1[0].y);
    glVertex2f(points_r1[1].x,points_r1[1].y);
    glVertex2f(points_r1[2].x,points_r1[2].y);
    glVertex2f(points_r1[3].x,points_r1[3].y);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex2f(points_r2[0].x,points_r2[0].y);
    glVertex2f(points_r2[1].x,points_r2[1].y);
    glVertex2f(points_r2[2].x,points_r2[2].y);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex2f(points_r3[0].x,points_r3[0].y);
    glVertex2f(points_r3[1].x,points_r3[1].y);
    glEnd();

    vec2 catepont = (p5-p1)*tolas + p1;
    glBegin(GL_POINTS);
    glVertex2f(points_r4.x,points_r4.y);
    glVertex2f(catepont.x,catepont.y);
    glEnd();
    glColor3f(0,0,0);
    }

}



void update(int n){

}

void display()
{
    GLint i;
    glClear(GL_COLOR_BUFFER_BIT);

    glFlush();

    glColor3f(0,0.6,0.2);
    glBegin(GL_POLYGON);
        glVertex2f(-10,104);
        glVertex2f(900,104);
        glVertex2f(900,-104);
        glVertex2f(-10,-104);
    glEnd();

    glColor3f(0,0.3,0.4);
    glBegin(GL_POLYGON);
    glVertex2f(-10,104);
    glVertex2f(-10,900);
    glVertex2f(900,900);
    glVertex2f(900,104);
    glEnd();

    glColor3f(1,1,0);
    glBegin(GL_POLYGON);
    for(GLdouble t = 0; t<= 2* pi(); t+= 0.1 ){
        vec2 cpoint = vec2(800 + 125 * cos(t), 600 + 125 * sin(t));
        glVertex2f(cpoint.x,cpoint.y);
    }
    glEnd();




    points[4] = (points[3]-points[2]) + points[3];
    points[7] = (points[6]-points[5]) + points[6];




    glColor3f(0.5,0.5,0);
    bezier(4,points[0],points[1],points[2],points[3]); // pontjai 0,1,2,3
    glColor3f(0.2,0.5,0);
    bezier(4,points[3],points[4],points[5],points[6]); // pontjai 3,4,5,6 ahol 3 ugyan az mint az elozoe, 4 pedig szamoljuk az erintot.
    glColor3f(0.2,0.2,1);
    bezier(4,points[6],points[7],points[8],points[9]); // kozos a 6, a 7et meg szamoljuk

    vec2 erinto = points[9]-points[10];
    //hermite(erinto,points[10],points[11], points[12]);
    glColor3f(0.6,0.3,0.5);
    hermite(points[12],points[11],points[10], erinto);
    glColor3f(0,0,0);
    glColor3f(0,0.5,0.5);
    glBegin(GL_LINES);
    glVertex2d(points[9].x, points[9].y);
    glVertex2d(points[10].x, points[10].y);
    glEnd();

    glColor3f(0,0,0);
    Caste(points[13],points[14],points[15],points[16],points[17]);
    glBegin(GL_LINES);
        glVertex2f(points[13].x,points[13].y);
        glVertex2f(points[17].x, points[17].y);
    glEnd();


    glBegin(GL_LINES);
    glVertex2f(points[0].x,points[0].y);
    glVertex2f(points[12].x,points[12].y);
    glEnd();

    vec2 kerek1 = (points[12]-points[0])*1/4.0 + points[0];
    vec2 kerek2 = (points[12]-points[0])*3/4.0 + points[0];


    glColor3f(0.5,0.5,0.5);
    //glColor3f(0,0,0);
    glBegin(GL_POLYGON);
    for(GLdouble t = 0; t<= 2* pi(); t+= 0.1 ){
        vec2 cpoint = vec2(kerek1.x + 40 * cos(t), kerek1.y + 40 * sin(t));
        glVertex2f(cpoint.x,cpoint.y);
    }
    glEnd();

    glColor3f(0,0,0);

    glBegin(GL_POLYGON);
    for(GLdouble t = 0; t<= 2* pi(); t+= 2*pi()/10 ){
        vec2 cpoint = vec2(kerek1.x + 25 * cos(t), kerek1.y + 25 * sin(t));
        glVertex2f(cpoint.x,cpoint.y);
    }
    glEnd();

    glColor3f(0.5,0.5,0.5);

    glBegin(GL_POLYGON);
    for(GLdouble t = 0; t<= 2* pi(); t+= 0.1 ){
        vec2 cpoint = vec2(kerek2.x + 40 * cos(t), kerek2.y + 40 * sin(t));
        glVertex2f(cpoint.x,cpoint.y);
    }
    glEnd();

    glColor3f(0,0,0);

    glBegin(GL_POLYGON);
    for(GLdouble t = 0; t<= 2* pi(); t+= 2*pi()/10 ){
        vec2 cpoint = vec2(kerek2.x + 25 * cos(t), kerek2.y + 25 * sin(t));
        glVertex2f(cpoint.x,cpoint.y);
    }
    glEnd();


    glColor3f(1,0,0);

    if(lines){
        glBegin(GL_LINE_LOOP);
        for (i = 0; i < 13; i++)
            glVertex2i(points[i].x, points[i].y);
        glEnd();
    }


    glColor3f(0, 0, 0);
    if(castel){
        glBegin(GL_LINE_LOOP);
        for (i = 13; i < 18; i++)
            glVertex2i(points[i].x, points[i].y);
        glEnd();}

    glColor3f(1,0,0);
    if (allpoints){
        glBegin(GL_POINTS);
        for (i = 0; i < 18; i++)
            glVertex2i(points[i].x, points[i].y);
        glEnd();
    }



    glColor3f(1,1,1);



    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
            exit(0);
            break;
        case 'a':
            if (tolas >= .01)
                tolas -= 0.01;
            break;
        case 'd':
            if (tolas <= .99)
                tolas += 0.01;
            break;
        case 'j':
            castel = !castel;
            break;
        case 'k':
            allpoints = !allpoints;
            break;
        case 'l':
            lines = !lines;
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

    vec4 M1 = {T0*T0*T0, T0*T0, T0, 1};
    vec4 M2 = {T1*T1*T1, T1*T1, T1, 1};
    vec4 M3 = {T2*T2*T2, T2*T2, T2, 1};
    vec4 ME = {3*T0*T0, 2*T0, 1, 0};
    M = {M3,M2,M1,ME,true};
    //M = {ME,M1,M2,M3,true};
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
    glutCreateWindow("Zsiguli!");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(processMouse);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(processMouseActiveMotion);
    glutMainLoop();
    return 0;
}
