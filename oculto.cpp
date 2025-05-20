#include <GL/glut.h>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

enum Mode { NONE, BACKFACE, ZBUFFER, PAINTER };
Mode currentMode = ZBUFFER;

float angleY = 0.0f;

// Estrutura para objetos da cena
struct Object {
    float x, y, z;
    void draw() const {
        glPushMatrix();
            glTranslatef(x, y, z);
            glColor3f(1.0 - z / 10.0, 0.5, z / 10.0);
            glutSolidCube(1.0);
        glPopMatrix();
    }
};

std::vector<Object> objects = {
    { -2, 0, -5 },
    {  0, 0, -7 },
    {  2, 0, -3 },
    { -1, 0, -9 }
};

// Desenha texto 2D na tela
void drawText(const std::string &text, int x, int y) {
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0, 600, 0, 600);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glRasterPos2i(x, y);
    for (char c : text) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// Desenha objetos de acordo com o modo
void drawObjects() {
    if (currentMode == PAINTER) {
        // Ordena manualmente do mais distante para o mais próximo
        std::sort(objects.begin(), objects.end(), [](Object a, Object b) {
            return a.z < b.z;
        });
        for (auto &o : objects) o.draw();
    } else {
        for (auto &o : objects) o.draw();
    }
}

void display() {
    if (currentMode == ZBUFFER) glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);

    if (currentMode == BACKFACE) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    } else {
        glDisable(GL_CULL_FACE);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0, 0, -6);
    glRotatef(angleY, 0, 1, 0);

    drawObjects();

    // Desenha legenda
    std::string modeName = (currentMode == ZBUFFER ? "Z-Buffer" :
                            currentMode == BACKFACE ? "Back-Face Culling" :
                            currentMode == PAINTER ? "Painter’s Algorithm" : "Nenhum");
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    drawText("Modo: " + modeName, 10, 570);

    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, aspect, 1, 100);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int, int) {
    switch (key) {
        case '1': currentMode = BACKFACE; break;
        case '2': currentMode = ZBUFFER; break;
        case '3': currentMode = PAINTER; break;
        case '0': currentMode = NONE; break;
    }
    glutPostRedisplay();
}

void special(int key, int, int) {
    if (key == GLUT_KEY_LEFT) angleY -= 5.0f;
    else if (key == GLUT_KEY_RIGHT) angleY += 5.0f;
    glutPostRedisplay();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Comparação - Técnicas de Remoção de Elementos Ocultos");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);

    glutMainLoop();
    return 0;
}
