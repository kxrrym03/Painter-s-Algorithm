#include <GL/glut.h>
#include <cmath>
#include <vector>

float handAngle = 0.0f;
bool picking = true;
float carX = 0.5f;
float personX = -0.5f;
bool pickedTrash = false;

struct Trash {
    float x, y;
    bool picked;
};

std::vector<Trash> trashItems = {{-0.3f, -0.2f, false}, {-0.25f, -0.25f, false}, {-0.35f, -0.3f, false}};

struct Cloud {
    float x, y;
};

std::vector<Cloud> clouds = {{-0.3f, 0.4f}, {0.2f, 0.45f}, {-0.6f, 0.5f}};

void drawCircle(float x, float y, float radius) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 360; i += 10) {
        float angle = i * M_PI / 180.0f;
        glVertex2f(x + radius * cos(angle), y + radius * sin(angle));
    }
    glEnd();
}

void drawRectangle(float x1, float y1, float x2, float y2) {
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

void drawSky() {
    glColor3f(0.53f, 0.81f, 0.98f); // Light blue sky
    drawRectangle(-1.0f, 0.1f, 1.0f, 1.0f);
}

void drawGround() {
    glColor3f(0.6f, 0.3f, 0.1f); // Brown ground
    drawRectangle(-1.0f, -0.3f, 1.0f, -0.1f);
}

void drawMountains() {
    glColor3f(0.0f, 0.5f, 0.0f); // Green mountains
    glBegin(GL_TRIANGLES);
    glVertex2f(-1.0f, 0.1f);
    glVertex2f(-0.5f, 0.5f);
    glVertex2f(0.0f, 0.1f);

    glVertex2f(0.0f, 0.1f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(1.0f, 0.1f);
    glEnd();
}

void drawBuildings() {
    glColor3f(0.6f, 0.6f, 0.8f); // Light blue buildings
    drawRectangle(-1.0f, 0.1f, -0.7f, 0.4f);
    drawRectangle(0.7f, 0.1f, 1.0f, 0.45f);
}

void drawClouds() {
    glColor3f(1.0f, 1.0f, 1.0f); // White clouds
    for (const auto& cloud : clouds) {
        drawCircle(cloud.x, cloud.y, 0.05f);
        drawCircle(cloud.x + 0.05f, cloud.y, 0.04f);
        drawCircle(cloud.x - 0.05f, cloud.y, 0.04f);
    }
}

void drawTrash() {
    glColor3f(1.0f, 1.0f, 1.0f); // Brown trash
    for (auto& t : trashItems) {
        if (!t.picked) {
            drawCircle(t.x, t.y, 0.02f);
        }
    }
}

void drawBin() {
    glColor3f(0.3f, 0.3f, 0.3f); // Gray bin
    drawRectangle(0.1f, -0.05f, 0.2f, -0.2f);
}

void drawRoad() {
    glColor3f(0.2f, 0.2f, 0.2f); // Dark gray road
    drawRectangle(-1.0f, -0.1f, 1.0f, 0.1f);
    glColor3f(1.0f, 1.0f, 1.0f); // White road lines
    glLineWidth(2.0);
    glBegin(GL_LINES);
    for (float i = -0.9f; i < 0.9f; i += 0.2f) {
        glVertex2f(i, 0.0f);
        glVertex2f(i + 0.1f, 0.0f);
    }
    glEnd();
}

void drawCar() {
    glColor3f(1.0f, 0.0f, 0.0f); // Red car
    drawRectangle(carX - 0.1f, 0.05f, carX + 0.1f, 0.12f);
    glColor3f(0.0f, 0.0f, 0.0f); // Black wheels
    drawCircle(carX - 0.07f, 0.04f, 0.02f);
    drawCircle(carX + 0.07f, 0.04f, 0.02f);
}

void drawMan() {
    glColor3f(0.0f, 0.0f, 0.0f); // Black person
    drawCircle(personX, 0.3f, 0.05f);
    glLineWidth(3.0);
    glBegin(GL_LINES);
    glVertex2f(personX, 0.25f); glVertex2f(personX, 0.0f);
    glVertex2f(personX, 0.0f); glVertex2f(personX - 0.05f, -0.2f);
    glVertex2f(personX, 0.0f); glVertex2f(personX + 0.05f, -0.2f);
    glVertex2f(personX, 0.2f); glVertex2f(personX - 0.05f, 0.1f);
    glVertex2f(personX, 0.2f); glVertex2f(personX + 0.05f + 0.1f * cos(handAngle), 0.1f - 0.1f * sin(handAngle));
    glEnd();
}

void update(int value) {
    // Update hand angle for animation
    handAngle += 0.1f;
    if (handAngle > 2 * M_PI) {
        handAngle -= 2 * M_PI;
    }

    // Move car
    carX -= 0.01f;
    if (carX < -1.0f) {
        carX = 1.0f;
    }

    // Move person
    personX += 0.003f;
    if (personX > 1.0f) {
        personX = -1.0f;
    }

    // Check for trash picking
    for (auto& t : trashItems) {
        if (!t.picked && fabs(personX - t.x) < 0.1f && fabs(-0.2f - t.y) < 0.1f) {
            t.picked = true; // Person picks up trash
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // 60 FPS
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawSky(); // Draw blue sky
    drawGround();
    drawMountains();
    drawBuildings();
    drawClouds();
    drawRoad();
    drawCar();
    drawMan();
    drawTrash();
    drawBin();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Trash Picking Game");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}
