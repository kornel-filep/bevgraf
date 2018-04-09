#include <GLUT/glut.h>
#include "bevgrafmath2017.h"

GLsizei winWidth = 600, winHeight = 600;

vec2 windowSizeXCube = {2,2};
vec2 windowPositionXCube = {-1,-1};

vec2 windowSizeOneSlashX = {10,10};
vec2 windowPositionOneSlashX = {-5,-5};

vec2 windowSizeSinAndCos = {4*pi(), 4*pi()};
vec2 windowPositionSinAndCos = {-2*pi(), -2*pi()};

vec2 windowSizeWeird = {6*pi(), 6*pi()};
vec2 windowPositionWeird = { -3*pi(), -3*pi() };

vec2 viewportSize = { 250, 250};

vec2 viewportPositionXCube = {10, 300};
vec2 viewportPositionOneSlashX = {330, 300};
vec2 viewportPositionSinAndCos = {10, 10};
vec2 viewportPositionWeird = { 330, 10};

mat3 wTvXCube, wTvOneSlashX, wTvSinAndCos, wTvWeird;

void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, winWidth, 0.0, winHeight);
    glShadeModel(GL_FLAT);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(5.0);
    glLineWidth(1.0);

    wTvWeird = windowToViewport2(windowPositionWeird, windowSizeWeird, viewportPositionWeird, viewportSize);
    wTvXCube = windowToViewport2(windowPositionXCube, windowSizeXCube, viewportPositionXCube, viewportSize);
    wTvOneSlashX = windowToViewport2(windowPositionOneSlashX, windowSizeOneSlashX, viewportPositionOneSlashX, viewportSize);
    wTvSinAndCos = windowToViewport2(windowPositionSinAndCos, windowSizeSinAndCos, viewportPositionSinAndCos, viewportSize);
}

void drawViewport(){
    glColor3f(0, 0, 0);

    glBegin(GL_LINE_LOOP);

    glVertex2f(viewportPositionWeird.x, viewportPositionWeird.y); // left bottom
    glVertex2f(viewportPositionWeird.x + viewportSize.x, viewportPositionWeird.y); // right bottom

    glVertex2f(viewportPositionWeird.x + viewportSize.x, viewportPositionWeird.y + viewportSize.y); // right top
    glVertex2f(viewportPositionWeird.x, viewportPositionWeird.y + viewportSize.y); // left top

    glEnd();

    glBegin(GL_LINE_LOOP);

    glVertex2f(viewportPositionXCube.x, viewportPositionXCube.y); // left bottom
    glVertex2f(viewportPositionXCube.x + viewportSize.x, viewportPositionXCube.y); // right bottom

    glVertex2f(viewportPositionXCube.x + viewportSize.x, viewportPositionXCube.y + viewportSize.y); // right top
    glVertex2f(viewportPositionXCube.x, viewportPositionXCube.y + viewportSize.y); // left top

    glEnd();

    glBegin(GL_LINE_LOOP);

    glVertex2f(viewportPositionOneSlashX.x, viewportPositionOneSlashX.y); // left bottom
    glVertex2f(viewportPositionOneSlashX.x + viewportSize.x, viewportPositionOneSlashX.y); // right bottom

    glVertex2f(viewportPositionOneSlashX.x + viewportSize.x, viewportPositionOneSlashX.y + viewportSize.y); // right top
    glVertex2f(viewportPositionOneSlashX.x, viewportPositionOneSlashX.y + viewportSize.y); // left top

    glEnd();

    glBegin(GL_LINE_LOOP);

    glVertex2f(viewportPositionSinAndCos.x, viewportPositionSinAndCos.y); // left bottom
    glVertex2f(viewportPositionSinAndCos.x + viewportSize.x, viewportPositionSinAndCos.y); // right bottom

    glVertex2f(viewportPositionSinAndCos.x + viewportSize.x, viewportPositionSinAndCos.y + viewportSize.y); // right top
    glVertex2f(viewportPositionSinAndCos.x, viewportPositionSinAndCos.y + viewportSize.y); // left top

    glEnd();



}

void calcMedians(vec2 window, mat3 wT){
    glColor3f(0,0,0);
    vec2 medp1 = {0, -window.y/2};
    vec2 medp2 = {0, window.y/2};
    glBegin(GL_LINES);
    vec3 pH1 = ihToH(medp1);
    vec3 resultH1 = wT * pH1;
    if( resultH1.z != 0) {
        vec2 result = hToIh(resultH1);
        glVertex2f(result.x, result.y);
    }
    vec3 pH2 = ihToH(medp2);
    vec3 resultH2 = wT * pH2;
    if( resultH2.z != 0) {
        vec2 result = hToIh(resultH2);
        glVertex2f(result.x, result.y);
    }
    glEnd();

    vec2 medp3 = {-window.x/2,0};
    vec2 medp4 = {window.x/2,0};
    glBegin(GL_LINES);
    vec3 pH3 = ihToH(medp3);
    vec3 resultH3 = wT * pH3;
    if( resultH3.z != 0) {
        vec2 result = hToIh(resultH3);
        glVertex2f(result.x, result.y);
    }
    vec3 pH4 = ihToH(medp4);
    vec3 resultH4 = wT * pH4;
    if( resultH4.z != 0) {
        vec2 result = hToIh(resultH4);
        glVertex2f(result.x, result.y);
    }
    glEnd();

}

void drawXCube(){

    glColor3f(0, 0, 1);
    vec2 coord = 0;
    glBegin(GL_LINE_STRIP);
    for(float x = -1; x <= 1; x += 0.01){
        coord = {x, x*x*x};
        vec3 pH = ihToH(coord);
        vec3 resultH = wTvXCube * pH;
        if( resultH.z != 0) {
            vec2 result = hToIh(resultH);
            glVertex2f(result.x, result.y);
        }
    }
    glEnd();

    calcMedians(windowSizeXCube, wTvXCube);
}

void drawOneSlashX(){
    glColor3f(0, 0, 1);
    vec2 coord = 0;
    glBegin(GL_LINE_STRIP);
    for(float x = -5; x < 0; x += 0.3){
        coord = {x, 1/x};
        vec3 pH = ihToH(coord);
        vec3 resultH = wTvOneSlashX * pH;
        if( resultH.z != 0) {
            vec2 result = hToIh(resultH);
            glVertex2f(result.x, result.y);
            printMathObject(result);
        }
    }
    glEnd();


    glBegin(GL_LINE_STRIP);
    for(float x = 0; x < 5; x += 0.2){
        coord = {x, 1/x};
        vec3 pH = ihToH(coord);
        vec3 resultH = wTvOneSlashX * pH;
        if( resultH.z != 0) {
            vec2 result = hToIh(resultH);
            glVertex2f(result.x, result.y);
            printMathObject(result);
        }
    }
    glEnd();

    calcMedians(windowSizeOneSlashX, wTvOneSlashX);

}

void drawSinAndCos(){
    glColor3f(0, 0, 1);
    vec2 coord = 0;
    glBegin(GL_LINE_STRIP);
    for(float x = -two_pi(); x < two_pi(); x += pi()/30){
        coord = {x, sin(x)};

        vec3 pH = ihToH(coord);
        vec3 resultH = wTvSinAndCos * pH;
        if( resultH.z != 0) {
            vec2 result = hToIh(resultH);
            glVertex2f(result.x, result.y);
            printMathObject(result);
        }
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for(float x = -two_pi(); x < two_pi(); x += pi()/30){
        coord = {x, cos(x)};

        vec3 pH = ihToH(coord);
        vec3 resultH = wTvSinAndCos * pH;
        if( resultH.z != 0) {
            vec2 result = hToIh(resultH);
            glVertex2f(result.x, result.y);
            printMathObject(result);
        }
    }
    glEnd();

    calcMedians(windowSizeSinAndCos, wTvSinAndCos);
}

void drawWeirdSinCos(){

    glColor3f(0, 0, 1);

    vec2 coord = 0;
    glBegin(GL_LINE_LOOP);
    for(float u = -1.5*pi(); u <= 2.5*pi(); u += pi()/150){
        coord = {static_cast<float>(u - 1.6*cos(24*u)), static_cast<float>(u - 1.6*sin(25*u))};
        vec3 pH = ihToH(coord);
        vec3 resultH = wTvWeird * pH;
        if( resultH.z != 0) {
            vec2 result = hToIh(resultH);
            glVertex2f(result.x, result.y);
        }
    }

    glEnd();

    calcMedians(windowSizeWeird, wTvWeird);


}



void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0, 0, 0);

    drawViewport();
    drawXCube();
    drawOneSlashX();
    drawSinAndCos();
    drawWeirdSinCos();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Grafikonok");

    init();
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}