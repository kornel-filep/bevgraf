#include <GLUT/glut.h>
#include "bevgrafmath2017.h"

#include <iostream>

GLsizei winWidth = 800, winHeight = 600;

vec2 points[10] = { {50, 400}, {100, 300}, {150, 250},
        {200, 350}, {300,150}/*ez nem used*/, {370,210},
        {300,350}, {400,480}, {500,480},
        {550,400} };


float T1 = -2;
double T2 = 0.5;
double T3 = 1.5;

vec2 E2BEZ;
vec2 points_r1[] = {0,0,0,0};
vec2 points_r2[] = {0,0,0};
vec2 points_r3[] = {0,0};
vec2 points_r4 = {0};

float tolas = 0.5;

mat4 M = {};

GLint dragged = -1;


void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
            exit(0);
            break;
        case 'w':
            if (tolas <= .99)
            tolas += 0.01;
            break;
        case 's':
            if (tolas >= 0.01)
            tolas -= 0.01;
            break;
    }
    glutPostRedisplay();
}



void hermite(){

    //Helyettesitgetes


    glColor3f(.5, 0.0, 0.0);

    vec2 erinto = {points[3].x-points[0].x,points[3].y-points[0].y};
    mat24 G = {points[0],points[1],points[2], erinto};

    vec4 t2dif = {static_cast<float>(3*pow(T3,2)),static_cast<float>(2*T3),1,0};
    vec4 cM = M * t2dif;
    vec2 E2 = erinto*cM[3];
    for(int i= 0; i<3; i++){
        E2 += points[i] * cM[i];
    }

    E2BEZ = E2;


    //A vegso erintovektor megrajzolasa P2bol.
    //P2+E2
    glBegin(GL_LINES);
    glVertex2i((points[2]+E2).x, (points[2]+E2).y);
    glVertex2i(points[2].x, points[2].y);
    glEnd();


    glBegin(GL_POINTS);
    glVertex2i(points[2].x+E2.x, points[2].y+E2.y);
    glEnd();

    glBegin(GL_LINE_STRIP);

    vec2 Q;

    //Iteralas t alapjan
    for (double t = -2; t<=1.5; t+= 0.001){
        vec4 tk = {static_cast<float>(pow(t,3)), static_cast<float>(pow(t,2)), static_cast<float>(pow(t,1)), static_cast<float>(pow(t,0))};
        /*double Qx = points[0].x*(M[0][0]*pow(t,3)+M[0][1]*pow(t,2)+M[0][2]*pow(t,1)+M[0][3]*pow(t,0))+
                points[1].x*(M[1][0]*pow(t,3)+M[1][1]*pow(t,2)+M[1][2]*pow(t,1)+M[1][3]*pow(t,0))+
                points[2].x*(M[2][0]*pow(t,3)+M[2][1]*pow(t,2)+M[2][2]*pow(t,1)+M[2][3]*pow(t,0))+
                erinto.x*(M[3][0]*pow(t,3)+M[3][1]*pow(t,2)+M[3][2]*pow(t,1)+M[3][3]*pow(t,0));

        double Qy = points[0].y*(M[0][0]*pow(t,3)+M[0][1]*pow(t,2)+M[0][2]*pow(t,1)+M[0][3]*pow(t,0))+
                points[1].y*(M[1][0]*pow(t,3)+M[1][1]*pow(t,2)+M[1][2]*pow(t,1)+M[1][3]*pow(t,0))+
                points[2].y*(M[2][0]*pow(t,3)+M[2][1]*pow(t,2)+M[2][2]*pow(t,1)+M[2][3]*pow(t,0))+
                erinto.y*(M[3][0]*pow(t,3)+M[3][1]*pow(t,2)+M[3][2]*pow(t,1)+M[3][3]*pow(t,0));
        */

        Q = G*M*tk;

        glVertex2d(Q.x,Q.y);
    }

    glEnd();
    //Az elso erinto meghuzasa
    glBegin(GL_LINES);
    glVertex2i(points[3].x, points[3].y);
    glVertex2i(points[0].x, points[0].y);
    glEnd();

}

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



void bezier(){
    //elso pontja points[2], a tobbi pedig points[4], points[5], points[6].
    //points[4] nem points[4] hanem egy szamolt pont lesz.
    //Szoval a masodik pontunk points[2]+(E2BEZ/3)

    glColor3f(.5, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
    glVertex2i(points[2].x,points[2].y);
    glVertex2i(points[2].x+E2BEZ.x/3,points[2].y+E2BEZ.y/3);
    glVertex2i(points[5].x,points[5].y);
    glVertex2i(points[6].x,points[6].y);
    glEnd();
    //Erinto pont ami nem mozgathato
    glColor3f(0.0, 2.0, 5.0);
    glBegin(GL_POINTS);
    glVertex2i(points[2].x+E2BEZ.x/3,points[2].y+E2BEZ.y/3);
    glEnd();


    glColor3f(0, 0.5, 0.5);
    glBegin(GL_LINE_STRIP);

    for (double t = 0; t <= 1; t+=0.01) {
        vec2 B = 0;
        vec2 points_tmp[] = {points[2], (points[2] + (E2BEZ / 3)), points[5], points[6]};
        //Egyszerusiteni
        for (int i = 0; i < 4; i++) {
            double KomboBern = choose(3.0,i)*pow(t,i)*pow(1.0-t,3.0-i);
        //B.x =pow(1-t,3)*points_tmp[0].x + 3 * pow((1-t),2)*t*points_tmp[1].x + 3 * ((1-t)*pow(t,2))*points_tmp[2].x+ pow(t,3)*points_tmp[3].x;
        //B.y =pow(1-t,3)*points_tmp[0].y + 3 * pow((1-t),2)*t*points_tmp[1].y + 3 * ((1-t)*pow(t,2))*points_tmp[2].y+ pow(t,3)*points_tmp[3].y;
            B.x += KomboBern*points_tmp[i].x;
            B.y += KomboBern*points_tmp[i].y;

            //printMathObject(B);
        }
        glVertex2d(B.x, B.y);


    }
    glEnd();
    //A gorbe vonalai




}



void bezier5() {
    //Az elso pontja points[6]
    //A masodik pontjat nem valaszthatjuk meg csak ugy mivel C1 folytonos kell hogy legyen
    //Ezert a masodik pont = points[6] + 3/4(points[6]-points[5])
    //


    glColor3f(1.0, 5.0, 0.0);

    glBegin(GL_LINE_STRIP);
    for(double t = 0; t<=1; t+=0.01){
        vec2 points_tmp[] = {points[6], points[6] + ((3/4.0)*(points[6]-points[5])), points[7],points[8],points[9]} ;
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
    glEnd();
    vec2 points_tmp2[] = {points[6], points[6] + ((3/4.0)*(points[6]-points[5])), points[7],points[8],points[9]} ;

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
        std::cout << tolas << std::endl;

    }

    glColor3f(1.0, 0.0, 0.0);

    glBegin(GL_LINE_STRIP);
    glVertex2d(points[6].x,points[6].y);
    glVertex2d((points[6] + ((3/4.0)*(points[6]-points[5]))).x, (points[6] + ((3/4.0)*(points[6]-points[5]))).y);
    glVertex2d(points[7].x,points[7].y);
    glVertex2d(points[8].x,points[8].y);
    glVertex2d(points[9].x,points[9].y);
    glEnd();

    glColor3f(0.0, .5, 0.0);
    glBegin(GL_POINTS);
    glVertex2d((points[6] + ((3/4.0)*(points[6]-points[5]))).x, (points[6] + ((3/4.0)*(points[6]-points[5]))).y);

    glEnd();

    //glLineWidth(1.5);

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

    glColor3f(0.0,0.5,0.5);
    glBegin(GL_POINTS);
    glVertex2f(points_r4.x,points_r4.y);
    glVertex2f(points_r1[0].x,points_r1[0].y);
    glVertex2f(points_r1[1].x,points_r1[1].y);
    glVertex2f(points_r1[2].x,points_r1[2].y);
    glVertex2f(points_r1[3].x,points_r1[3].y);
    glVertex2f(points_r2[0].x,points_r2[0].y);
    glVertex2f(points_r2[1].x,points_r2[1].y);
    glVertex2f(points_r2[2].x,points_r2[2].y);
    glVertex2f(points_r3[0].x,points_r3[0].y);
    glVertex2f(points_r3[1].x,points_r3[1].y);
    glEnd();



}




void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
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

    M ={{static_cast<float>(pow(T1,3.0)),static_cast<float>(pow(T2,3.0)),static_cast<float>(pow(T3,3.0)),static_cast<float>(3*pow(T1,2.0))},
            {static_cast<float>(pow(T1,2.0)),static_cast<float>(pow(T2,2.0)),static_cast<float>(pow(T3,2.0)),static_cast<float>(2*pow(T1,1.0))},
            {static_cast<float>(pow(T1,1.0)),static_cast<float>(pow(T2,1.0)),static_cast<float>(pow(T3,1.0)),static_cast<float>(1)},
            {static_cast<float>(pow(T1,0)),static_cast<float>(pow(T2,0)),static_cast<float>(pow(T3,0)),static_cast<float>(0)}};

    M = inverse(M);
    printMathObject(M);
}

void display()
{
    GLint i;
    glClear(GL_COLOR_BUFFER_BIT);


    hermite();
    bezier();
    glFlush();
    bezier5();



    glColor3f(256.0, 256.0, 256.0);



    glBegin(GL_POINTS);
    for (i = 0; i < 10; i++)
        if (i != 4)
            glVertex2i(points[i].x, points[i].y);
    glEnd();





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
    if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
        if ((i = getActivePoint1(points, 10, 8, xMouse, winHeight - yMouse)) != -1)
            dragged = i;

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
