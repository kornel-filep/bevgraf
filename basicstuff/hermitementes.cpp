#include <GLUT/glut.h>
#include "bevgrafmath2017.h"

#include <iostream>
#include <vector>

GLsizei winWidth = 800, winHeight = 600;

//vec2 points[4] = { {50, 400}, {100, 300}, {150, 250}, {200, 300}};
vec2 points[4] = {};

double colorchange = 0;
double T0 = -1;
double T1 = 0;
double T2 = 1;
double T3 = 2.0;
double halfT = 0.5;
double Tmoove = T0;
vec2 maxpoint = 0;
mat24 G;
int count = 0;
bool home = false;
int starFrameCounter = 0;
mat4 M = {};

GLint dragged = -1;
std::vector<vec2> stars;



void randomStars(){
    srand (time(NULL));
    if (starFrameCounter > 30 && home == false){
        stars.clear();
    for (int i = 0; i < 40; i++){
        vec2 star;
        star.x = rand()%(800-0 + 1) + 0;
        star.y = rand()%(600-(int) maxpoint.y + 1) + (int) maxpoint.y;


        stars.push_back(star);
    }
        starFrameCounter = 0;
    }

    //glBegin(GL_POINTS);
    //for (vec2 star:stars){
      //  glVertex2d(star.x, star.y);
    //    printMathObject(star);
   // }
    //glEnd();

}

vec2 hermitePoint(float t){
    vec4 tk = {static_cast<float>(pow(t,3)), static_cast<float>(pow(t,2)), static_cast<float>(pow(t,1)), static_cast<float>(pow(t,0))};
    vec2 Qpicked = G*M*tk;
    return Qpicked;

}


void hermite(){
    glClearColor(0.5+colorchange, 0.5+colorchange, 0.5+colorchange, 0.0);

    //Helyettesitgetes


    glColor3f(.5, 0.0, 0.0);

    //vec2 erinto = {points[3].x-points[0].x,points[3].y-points[0].y};
    G = {points[0],points[1],points[2], points[3]};

    glBegin(GL_LINE_STRIP);

    vec2 Q;

    //Iteralas t alapjan
    for (double t = -1; t<=2; t+= 0.001){
        vec4 tk = {static_cast<float>(pow(t,3)), static_cast<float>(pow(t,2)), static_cast<float>(pow(t,1)), static_cast<float>(pow(t,0))};
        Q = G*M*tk;
        glVertex2d(Q.x,Q.y);
        if ( Q.y > maxpoint.y){
            maxpoint = Q;
        }
    }

    glEnd();

    if ( home == false){
    //vec2 erinto = {points[0].x-points[1].x, points[0].y - points[1].y};
    vec4 tkdif = {static_cast<float>(3*pow(Tmoove,2)), static_cast<float>(2*pow(Tmoove,1)), static_cast<float>(pow(Tmoove,0)), static_cast<float>(0)};

    vec4 cM = M * tkdif;
    vec2 p = 0;
    for(int i = 0; i<4; i++){
        p += points[i] * cM[i];
    }
    //p = normalize(p);
    float tmp = p.x;
    p.x = p.y * -1;
    p.y = tmp;
    p = p / length(p);

    vec2 Qpicked = hermitePoint(Tmoove);

    //glColor3f(1.0, 1.0, 0.0);
    //vec2 erintovektor = erinto*tk;
    glColor3f(0.0,1.0,0.0);
    //EMBERTEST
    glBegin(GL_LINES);
    glVertex2d(Qpicked.x, Qpicked.y);
    //glVertex2d(Qpicked.x+p.x, Qpicked.y+p.y);
    glVertex2d(Qpicked.x+p.x*15, Qpicked.y+p.y*15);
    glEnd();
    vec2 Qleg = hermitePoint(Tmoove + 0.02);
    //labika
    glBegin(GL_LINES);
    glVertex2d(Qleg.x, Qleg.y);
    //glVertex2d(Qpicked.x+p.x, Qpicked.y+p.y);
    glVertex2d(Qpicked.x+p.x*10, Qpicked.y+p.y*10);
    glEnd();
    //kezecske
    glBegin(GL_LINES);
    glVertex2d(Qpicked.x+p.x*13, Qpicked.y+p.y*13);
    //glVertex2d(Qpicked.x+p.x, Qpicked.y+p.y);
    glVertex2d(Qleg.x+p.x*13, Qleg.y+p.y*13);
    glEnd();
    vec2 head = (Qpicked + p* 18);
    glBegin(GL_LINES);
    int r = 3;
    for (GLdouble z = 0; z <= 2 * pi(); z += 0.1)
        glVertex2d(head.x + r * cos(z), head.y + r * sin(z));
    glEnd();

    }



    glColor3f(.5, 0.0, 0.0);

}

vec2 TMP(float T){
    double tmp;
    vec4 tk = {static_cast<float>(3*pow(T,2)), static_cast<float>(2*pow(T,1)), static_cast<float>(pow(T,0)), static_cast<float>(0)};
    vec4 tM = M * tk;
    vec2 p = 0;
    for(int i = 0; i<4; i++){
        p += points[i] * tM[i];
    }
    tmp = p.x;
    p.x = p.y * -1;
    p.y = tmp;

    p = p/length(p);

    return p;
}

void haziko(){
    //HAZIKO
    double tmp;
    vec4 thaziko = {static_cast<float>(3*pow(T3,2)), static_cast<float>(2*pow(T3,1)), static_cast<float>(pow(T3,0)), static_cast<float>(0)};
    vec4 hazM = M * thaziko;
    vec2 hazP = 0;
    for(int i = 0; i<4; i++){
        hazP += points[i] * hazM[i];
    }
    tmp = hazP.x;
    hazP.x = hazP.y * -1;
    hazP.y = tmp;
    //Egyik pont
    hazP = (hazP / length(hazP))*20;
    hazP = TMP(T3)*20;

    //Masik pont
    vec2 hazP2 = hermitePoint(T3);
    hazP += hazP2;
    //Harmadik pont
    vec4 thaziko3 = {static_cast<float>(3*pow(T3-0.15,2)), static_cast<float>(2*pow(T3-0.15,1)), static_cast<float>(pow(T3-0.15,0)), static_cast<float>(0)};
    vec4 haz3M = M*thaziko3;
    vec2 hazP3 = 0;
    for(int i = 0; i<4; i++){
        hazP3 += points[i] * haz3M[i];
    }
    tmp = hazP3.x;
    hazP3.x = hazP3.y * -1;
    hazP3.y = tmp;
    //Negyedik pont
    vec2 hazP4 = hermitePoint(T3-0.15);
    hazP3 = (hazP3 / length(hazP3))*20;
    hazP3 += hazP4;
    //Teljes haziko
    glBegin(GL_POLYGON);
    glVertex2d(hazP.x, hazP.y); //felso pont
    glVertex2d(hazP2.x, hazP2.y);
    glVertex2d(hazP4.x, hazP4.y);
    glVertex2d(hazP3.x, hazP3.y); //felso pont

    glEnd();




    vec2 tetopont = 0;
    vec2 tetoalap = hermitePoint(T3-0.075);
    vec4 tetoT = {static_cast<float>(3*pow(T3-0.075,2)), static_cast<float>(2*pow(T3-0.075,1)), static_cast<float>(pow(T3-0.075,0)), static_cast<float>(0)};
    vec4 tetoM = M*tetoT;
    for(int i = 0; i<4; i++){
        tetopont += points[i] * tetoM[i];
    }
    tmp = tetopont.x;
    tetopont.x = tetopont.y * -1;
    tetopont.y = tmp;
    tetopont = (tetopont / length(tetopont)) * 35  + tetoalap;
    //Hazikonak a teteje
    glColor3f(.5, 0.5, 0.0);
    glBegin(GL_POLYGON);
    glVertex2d(hazP.x, hazP.y);
    glVertex2d(hazP3.x, hazP3.y);
    glVertex2d(tetopont.x, tetopont.y);
    glEnd();

    glColor3f(.5, 0.5, 0.0);

    //ablak
    if(home){
    vec2 ablakpont1;
    vec2 ablakpont2;
    vec2 ablakalap1 = hermitePoint(T3-0.05);
    vec4 ablakT1 = {static_cast<float>(3*pow(T3-0.05,2)), static_cast<float>(2*pow(T3-0.05,1)), static_cast<float>(pow(T3-0.05,0)), static_cast<float>(0)};
    vec4 ablakM1 = M*ablakT1;
    for(int i = 0; i<4; i++){
        ablakpont1 += points[i] * ablakM1[i];
    }
    tmp = ablakpont1.x;
    ablakpont1.x = ablakpont1.y * -1;
    ablakpont1.y = tmp;
    ablakpont2 = ablakpont1;
    ablakpont1 = (ablakpont1 / length(ablakpont1)) * 8 + ablakalap1;
    ablakpont2 = (ablakpont2 / length(ablakpont2)) * 15 + ablakalap1;

    vec2 ablakpont3;
    vec2 ablakpont4;
    vec2 ablakalap2 = hermitePoint(T3 - 0.09);
    vec4 ablakT2 = {static_cast<float>(3*pow(T3-0.09,2)), static_cast<float>(2*pow(T3-0.09,1)), static_cast<float>(pow(T3-0.09,0)), static_cast<float>(0)};
    vec4 ablakM2 = M*ablakT2;
    for(int i = 0; i<4; i++){
        ablakpont3 += points[i] * ablakM2[i];
    }
    tmp = ablakpont3.x;
    ablakpont3.x = ablakpont3.y *-1;
    ablakpont3.y = tmp;
    ablakpont4 = ablakpont3;
    ablakpont3 = (ablakpont3 / length(ablakpont3)) * 8 + ablakalap2;
    ablakpont4 = (ablakpont4 / length(ablakpont4)) * 15 + ablakalap2;


    glBegin(GL_POLYGON);
    glVertex2d(ablakpont1.x, ablakpont1.y);
    glVertex2d(ablakpont2.x, ablakpont2.y);
    glVertex2d(ablakpont4.x, ablakpont4.y);
    glVertex2d(ablakpont3.x, ablakpont3.y);
    glEnd();}


    glColor3f(.5, 0.0, 0.0);
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
    M ={{static_cast<float>(pow(T0,3.0)),static_cast<float>(pow(T1,3.0)),static_cast<float>(pow(T2,3.0)),static_cast<float>(pow(T3,3.0))},
            {static_cast<float>(pow(T0,2.0)),static_cast<float>(pow(T1,2.0)),static_cast<float>(pow(T2,2.0)),static_cast<float>(pow(T3,2.0))},
            {static_cast<float>(pow(T0,1.0)),static_cast<float>(pow(T1,1.0)),static_cast<float>(pow(T2,1.0)),static_cast<float>(pow(T3, 1.0))},
            {static_cast<float>(pow(T0,0)),static_cast<float>(pow(T1,0)),static_cast<float>(pow(T2,0)),static_cast<float>(pow(T3,0))}};

    M = inverse(M);
    printMathObject(M);

}

void update(int n){
    if(Tmoove < T3-0.075){
        Tmoove += 0.01;
        colorchange -= 0.002;



        glutPostRedisplay();

        glutTimerFunc(8, update, 0);
    } else{
        home = true;
        glutPostRedisplay();

        //glutTimerFunc(8, update, 0);
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
            exit(0);
            break;
        case 'g':
            //go = true;
            glutTimerFunc(8, update, 0);
    }
    glutPostRedisplay();
}

void display()
{
    GLint i;
    glClear(GL_COLOR_BUFFER_BIT);

    if(count == 4){
    hermite();haziko();}
    glFlush();

    glColor3f(256.0, 256.0, 256.0);



    glBegin(GL_POINTS);
    for (i = 0; i < 4; i++)
            glVertex2i(points[i].x, points[i].y);
    glEnd();


    if (Tmoove > halfT){
        //std::cout << halfT << std::endl;
        glColor3f(1.0,1.0,1.0);
        randomStars();
        glBegin(GL_POINTS);
        for(vec2 str:stars){
            printMathObject(str);
            glVertex2d(str.x, str.y);
        }
        glEnd();
        starFrameCounter++;
    }


    glutSwapBuffers();
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


void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{

    GLint i;
    if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN){
        vec2 p = vec2(xMouse, winHeight - yMouse - 1);
        if ((i = getActivePoint1(points, 10, 8, xMouse, winHeight - yMouse)) != -1){
            dragged = i;}
        if (count < 4){
            points[count] = p;
            count++;
            glutPostRedisplay();
        }
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
    glutCreateWindow("Hermite & Bezier4 & Bezier5");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(processMouse);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(processMouseActiveMotion);
    glutMainLoop();
    return 0;
}
