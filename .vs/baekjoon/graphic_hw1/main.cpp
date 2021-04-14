#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
GLint TopLeftX, TopLeftY, BottomRightX, BottomRightY;

#define WSIZE (600)
int color = 0;
int mode = 0;
GLint px[3];
GLint py[3];
int index = 0;

class vec3 {
public:
    float m[3];
    vec3() {
        for (int i = 0; i < 3; i++) {
            m[i] = 0;
        }
    }
    vec3(float x, float y, float z) {
        m[0] = x;
        m[1] = y;
        m[2] = z;
    }
    vec3 operator+(vec3 x) {
        vec3 temp;
        temp.m[0] = m[0] + x.m[0];
        temp.m[1] = m[1] + x.m[1];
        temp.m[2] = m[2] + x.m[2];
        return temp;
    }
    vec3 operator-(vec3 x) {
        vec3 temp;
        temp.m[0] = m[0] - x.m[0];
        temp.m[1] = m[1] - x.m[1];
        temp.m[2] = m[2] - x.m[2];
        return temp;
    }
    float length() {
        return sqrtf(m[0] * m[0] + m[1] * m[1] + m[2] * m[2]);
    }
    void normalize() {
        float len = length();
        m[0] / -len;
        m[1] / -len;
        m[2] / -len;
    }
};

class mat3 {
public:
    float m[3][3];
    mat3() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                m[i][j] = 0;
            }
        }
    }
};

void MyDisplay() {
    //glViewport(0, 0, 300, 300);

    if (color == 0)
        glColor3f(0, 0, 0);
    else if (color == 1)
        glColor3f(1, 0, 0);
    else if (color == 2)
        glColor3f(0, 1, 0);
    else if (color == 3)
        glColor3f(0, 0, 1);
    if (mode == 0) {
        glBegin(GL_LINES);
        glVertex3f((float)TopLeftX / WSIZE, (float)(WSIZE - TopLeftY) / WSIZE, 0.0);
        //glVertex3f((float)TopLeftX / WSIZE, (float)(WSIZE - BottomRightY) /WSIZE, 0.0);
        glVertex3f((float)BottomRightX / WSIZE, (float)(WSIZE - BottomRightY) / WSIZE, 0.0);
        //glVertex3f((float)BottomRightX / WSIZE, (float)(WSIZE - TopLeftY) / WSIZE, 0.0);
        glEnd();
        glFlush();
        TopLeftX = BottomRightX;
        TopLeftY = BottomRightY;
    }
    else if (mode == 1 && index==3) {
        glBegin(GL_POLYGON);
        glVertex3f((float)px[0] / WSIZE, (float)(WSIZE - py[0]) / WSIZE, 0.0);
        glVertex3f((float)px[1] / WSIZE, (float)(WSIZE - py[1]) / WSIZE, 0.0);
        glVertex3f((float)px[2] / WSIZE, (float)(WSIZE - py[2]) / WSIZE, 0.0);
        //glVertex3f((float)BottomRightX / WSIZE, (float)(WSIZE - TopLeftY) / WSIZE, 0.0);
        glEnd();
        glFlush();
        index = 0;
        mode = 0;
    }
}

void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y) {

    if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN) {
        if (mode == 1) {
            px[index] = X;
            py[index] = Y;
            index++;
            if (index == 3) glutPostRedisplay();
        }
        else {
            TopLeftX = X;
            TopLeftY = Y;
            BottomRightX = X;
            BottomRightY = Y;
            glutPostRedisplay();
        }
    }

}

void MyMouseMove(GLint X, GLint Y) {
    
    BottomRightX = X;
    BottomRightY = Y;
    if (mode == 0) {
        glutPostRedisplay();
    }

}

void MyKeyboard(unsigned char KeyPressed, int X, int Y) {
    switch (KeyPressed) {
    case 'r':
        color = 1;
        break;
    case 'R':
        color = 1;
        break;
    case 'g':
        color = 2;
        break;
    case 'G':
        color = 2;
        break;
    case 'b':
        color = 3;
        break;
    case 'B':
        color = 3;
        break;
    case 'c':
        glClear(GL_COLOR_BUFFER_BIT);
        glutPostRedisplay();
        color = 0;
        break;
    case 'C':
        glClear(GL_COLOR_BUFFER_BIT);
        glutPostRedisplay();
        color = 0;
        break;
    case 't':
    case 'T':
        mode = 1;
        break;
    case 27:                //'esc' 키의 아스키 코드 값
        exit(0); break;
    }
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("OpenGL Drawing Example");

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

    glutDisplayFunc(MyDisplay);
    glutMouseFunc(MyMouseClick);
    glutMotionFunc(MyMouseMove);
    glutKeyboardFunc(MyKeyboard);
    glutMainLoop();
    return 0;
}