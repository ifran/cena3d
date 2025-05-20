#include <GL/glut.h>

float camX = 0.0f, camZ = 5.0f, angleY = 0.0f;
float sunAngle = 0.0f;
bool isPerspective = true;

void drawGround() {
    glColor3f(0.2, 0.8, 0.2);
    glBegin(GL_QUADS);
        glVertex3f(-10, 0, -10);
        glVertex3f( 10, 0, -10);
        glVertex3f( 10, 0,  10);
        glVertex3f(-10, 0,  10);
    glEnd();
}

void drawSky() {
    glColor3f(0.4, 0.6, 1.0);
    glBegin(GL_QUADS);
        glVertex3f(-50, 50, -50);
        glVertex3f( 50, 50, -50);
        glVertex3f( 50, 50,  50);
        glVertex3f(-50, 50,  50);
    glEnd();
}

void drawTree(float x, float z) {
    glPushMatrix();
        glTranslatef(x, 0, z);
        glColor3f(0.5, 0.3, 0.1);
        glPushMatrix();
            glTranslatef(0, 0.75, 0);
            glScalef(0.2, 1.5, 0.2);
            glutSolidCube(1);
        glPopMatrix();

        glColor3f(0.0, 0.5, 0.0);
        glTranslatef(0, 1.5, 0);
        glutSolidSphere(0.5, 16, 16);
    glPopMatrix();
}

void drawHouse(float x, float z) {
    glPushMatrix();
        glTranslatef(x, 0.5, z);
        glColor3f(0.7, 0.4, 0.2);
        glScalef(1.5, 1.0, 1.5);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(x, 1.0, z);
        glColor3f(0.8, 0.1, 0.1);
        glRotatef(-90, 1, 0, 0);
        glutSolidCone(1, 1, 4, 4);
    glPopMatrix();
}

void drawPole(float x, float z) {
    glPushMatrix();
        glTranslatef(x, 1.0, z);
        glColor3f(0.7, 0.7, 0.7);
        glRotatef(-90, 1, 0, 0);
        gluCylinder(gluNewQuadric(), 0.1, 0.1, 2.0, 12, 3);
    glPopMatrix();
}

void drawSun() {
    glPushMatrix();
        glColor3f(1.0, 1.0, 0.0);
        glTranslatef(5 * cos(sunAngle), 4.0, 5 * sin(sunAngle));
        glutSolidSphere(0.5, 16, 16);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(camX, 2.0, camZ, camX, 0.0, camZ - 5.0, 0.0, 1.0, 0.0);

    drawSky();
    drawGround();
    drawSun();

    drawTree(-3, -3);
    drawTree(2, 2);

    drawHouse(0, -4);
    drawHouse(-4, 2);

    drawPole(3, 3);

    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (isPerspective)
        gluPerspective(45.0, aspect, 1.0, 100.0);
    else
        glOrtho(-10, 10, -10, 10, -10, 100);
    glMatrixMode(GL_MODELVIEW);
}

void idle() {
    sunAngle += 0.01f;
    if (sunAngle > 2 * 3.14159f)
        sunAngle = 0.0f;
    glutPostRedisplay();
}

void keyboard(unsigned char key, int, int) {
    if (key == 'p' || key == 'P') {
        isPerspective = !isPerspective;
        glutPostRedisplay();
    }
}

void special(int key, int, int) {
    float step = 0.3f;
    if (key == GLUT_KEY_LEFT) camX -= step;
    else if (key == GLUT_KEY_RIGHT) camX += step;
    else if (key == GLUT_KEY_UP) camZ -= step;
    else if (key == GLUT_KEY_DOWN) camZ += step;
    glutPostRedisplay();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5, 0.8, 0.9, 1.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Cena 3D - OpenGL");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);

    glutMainLoop();
    return 0;
}
