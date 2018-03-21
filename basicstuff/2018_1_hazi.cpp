#include <GLUT/glut.h>					// (or others, depending on the system in use)
#include "bevgrafmath2017.h"
#include <vector>


GLsizei winWidth = 600;
vec2 circlePos = { 150.0, 150.0 };
GLint circleR = 50;
vec2 randomPoint;
vec2 OriVec = {1,1};
std::vector<vec2> circleVectors;
std::vector<vec2> pointVectors;
bool collision = false;
int score = 0;

void init()
{
    glClearColor(1.0, 1.0, 1.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, winWidth, 0.0, winWidth);
    srand(time(NULL));
    OriVec.x = rand() & 1 ? 1 : -1;
    OriVec.y = -OriVec.x;
    randomPoint = vec2(rand() % (winWidth-1)+1,rand() % (winWidth-1)+1);
    //randomPoint = vec2(rand() % (int)(pointSq - (winWidth - pointSq) + 1) + (winWidth - pointSq), rand() % (int)(pointSq - (winWidth - pointSq) + 1) + (winWidth - pointSq));
    //printMathObject(randomPoint);
}


void circle(vec2 O, GLdouble r) {
    for (GLdouble t = 0; t <= 2 * pi(); t += 0.1)
        circleVectors.push_back( vec2(O.x + r * cos(t), O.y + r * sin(t)));
}

void drawRandomPoint() {

    //circle(randomPoint, 50);
    GLdouble r = 10;
    for (GLdouble t = 0; t <= 2 * pi(); t += 0.1)
        pointVectors.push_back( vec2(randomPoint.x + r * cos(t), randomPoint.y + r * sin(t)));
}

void draw() {

    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 0.4f, 0.2f);

    if (score < 10) {
    circle(circlePos, circleR);
    drawRandomPoint();

    glBegin(GL_LINE_LOOP);

    for (vec2 point : pointVectors) {
        glVertex2d(point.x, point.y);
    }
    glEnd();

    glBegin(GL_LINE_LOOP);

    for (vec2 point : circleVectors) {
        glVertex2d(point.x, point.y);
    }
    glEnd();


    circleVectors.clear();
    pointVectors.clear();
    //printMathObject(recti[1]);
    }
    glFlush();

}

void checkForCorrigation(){
    double corrig = 0;
    if (winWidth < circlePos.x + circleR)
    {
        corrig=circlePos.x + circleR - winWidth;
        circlePos.x = circlePos.x - corrig;
    }

    if (0 > circlePos.x - circleR)
    {
        corrig = circleR-circlePos.x;
        circlePos.x = circlePos.x + corrig;
    }

    if (winWidth < circlePos.y + circleR)
    {
        corrig = circlePos.y + circleR - winWidth;
        circlePos.y = circlePos.y- corrig;
    }

    if (0 > circlePos.y - circleR)
    {
        corrig = circleR - circlePos.y;
        circlePos.y = circlePos.y + corrig;
    }

}



void update(int n) {
    if(score < 10){
    circlePos[0] += OriVec.x;
    if (circlePos[0] + circleR > winWidth || circlePos[0] - circleR < 0)
        OriVec.x *= -1;

    circlePos[1] += OriVec.y;
    if (circlePos[1] + circleR > winWidth || circlePos[1] - circleR < 0)
        OriVec.y *= -1;


    if (collision){
        circleR += 5;
        float initialD = 0;
        do {
            randomPoint = vec2(rand() % (winWidth-1)+1,rand() % (winWidth-1)+1);
            //initialD = sqrt((((randomPoint.x) - (circlePos.x))*((randomPoint.x) - (circlePos.x)))
            //        + (((randomPoint.y) - (circlePos.y))*((randomPoint.y) - (circlePos.y))));
            initialD = dist(randomPoint,circlePos);
        } while (initialD + 20 < circleR +10);
    }


    //float D = sqrt((((randomPoint.x ) - (circlePos.x))*((randomPoint.x) - (circlePos.x)))
    //        + (((randomPoint.y) - (circlePos.y))*((randomPoint.y) - (circlePos.y))));
    float D = dist(randomPoint,circlePos);
    //	float D = sqrt((((randomPoint.x + 10) - (circlePos.x + circleR))*((randomPoint.x + 10) - (circlePos.x + circleR)))
    //+(((randomPoint.y + 10) - (circlePos.y + circleR))*((randomPoint.y + 10) - (circlePos.y + circleR))));

    if (D <= circleR + 10) {

        collision = true;
        score++;
    }
    else {
        collision = false;
    }
    printMathObject(D);
    printMathObject(circleR);

    //checking to generate a point that the ball can reach

    if (randomPoint.x < circleR && randomPoint.y < circleR)
    {
        randomPoint = vec2(rand() % (winWidth-1)+1,rand() % (winWidth-1)+1);

    }
    if (randomPoint.x < circleR && randomPoint.y > winWidth-circleR)
    {
        randomPoint = vec2(rand() % (winWidth-1)+1,rand() % (winWidth-1)+1);

    }

    if (randomPoint.x > winWidth-circleR && randomPoint.y < circleR)
    {
        randomPoint = vec2(rand() % (winWidth-1)+1,rand() % (winWidth-1)+1);

    }

    if (randomPoint.x > winWidth - circleR && randomPoint.y > winWidth - circleR)
    {
        randomPoint = vec2(rand() % (winWidth-1)+1,rand() % (winWidth-1)+1);
    }

    checkForCorrigation();





    glutPostRedisplay();

    glutTimerFunc(5, update, 0); }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
            exit(0);
            break;
        case 'w':
            OriVec.x *= -1;
            break;
        case 's':
            OriVec.y *= -1;
            break;
        case 'p':
            //itt uj pont generalasa
            //randomPoint = vec2(rand() % (int)(pointSq - (winWidth - pointSq) + 1) + (winWidth - pointSq), rand() % (int)(pointSq - (winWidth - pointSq) + 1) + (winWidth - pointSq));
            printMathObject(randomPoint);
            break;
    }
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);                         // Initialize GLUT.
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);   // Set display mode.
    glutInitWindowPosition(50, 100);   // Set top-left display-window position.
    glutInitWindowSize(winWidth, winWidth);      // Set display-window width and height.
    glutCreateWindow("An Example OpenGL Program"); // Create display window.

    init();                            // Execute initialization procedure.
    glutDisplayFunc(draw);       // Send graphics to display window.
    //glutDisplayFunc(drawWithVector);       // Send graphics to display window.
    glutKeyboardFunc(keyboard);
    glutIgnoreKeyRepeat(true);
    glutTimerFunc(5, update, 0);
    glutMainLoop();                    // Display everything and wait.
    return 0;
}
