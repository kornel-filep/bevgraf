#include <GLUT/glut.h>
#include "bevgrafmath2017.h"
#include <math.h>

GLsizei winWidth = 800, winHeight = 600;

int N = 10;

//vec2 circle[10] = {};

mat3 matricies[11] = {};

vec2 mainCircleCenter = { 0.0, 0.0 };

vec2 rectanglePoints[4] = {{0, 50}, {50, 100}, {0,150}, {-50, 100}};

mat3 M;

vec2 s = { 0.5f, 0.5f };
float scaleChange = 0.01;
float t = 0;
/*
void initCircle() {

    int idx = 0;
    float R = 100, r = 50;

    float incValue = (2 * pi() / N);

    int i = 0;
    for (float t = 0; t < 2 * pi(); t += incValue) {

        circle[i].x = cos(t) * r + mainCircleCenter.x;
        circle[i].y = sin(t) * r + mainCircleCenter.y;

        i++;
    }
}
*/
void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-(winWidth / 2), (winWidth / 2), -(winHeight / 2), (winHeight / 2));
    glShadeModel(GL_FLAT);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(5.0);
    glLineWidth(1.0);

    M = mat3();

    //initCircle();
}

void drawCircle(vec2 listOfPoints[]) {

    glBegin(GL_POINTS);
    for (int i = 0; i < N; i++) {
        glVertex2f(listOfPoints[i].x, listOfPoints[i].y);
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < N; i++) {
        glVertex2f(listOfPoints[i].x, listOfPoints[i].y);
    }
    glVertex2f(listOfPoints[0].x, listOfPoints[0].y);
    glEnd();
}



void rotateStar() {

    mat3 transToO = translate(-1 * mainCircleCenter);
    mat3 rotated = rotate(t);
    mat3 scaled = scale(s);
    mat3 transToC = translate(mainCircleCenter);
    M = transToC * rotated  * transToO;

}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0, 0, 0);

    glBegin(GL_POINTS);
    glVertex2d(mainCircleCenter.x, mainCircleCenter.y);
    glEnd();


    vec2 listOfPointsAfterTransformation[4] = {};

    for (int i = 0; i < 4; i++) {

        vec3 pointH = ihToH(rectanglePoints[i]);
        vec3 rotatedPointH = M * pointH;
        if (rotatedPointH.z != 0) {
            vec2 result = hToIh(rotatedPointH);
            listOfPointsAfterTransformation[i] = { result.x, result.y };
        }

    }
    //drawCircle(listOfPointsAfterTransformation);

    glBegin(GL_POLYGON);
    for(vec2 p : listOfPointsAfterTransformation){
        glVertex2d(p.x, p.y);
    }
    glEnd();


    glutSwapBuffers();
}

void update(int v) {

    rotateStar();


    t += pi() / 180;

    if (t >= 2 * pi()) {
        t = 0;
    }

    s += scaleChange;

    if (s.x > 3.5 || s.x < 0.5) {
        scaleChange *= -1;
    }

    glutPostRedisplay();
    glutTimerFunc(10, update, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("2D Transformation");

    init();
    glutDisplayFunc(display);
    glutTimerFunc(10, update, 0);

    glutMainLoop();
    return 0;
}