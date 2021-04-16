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
    float Length() {
        return sqrtf(m[0] * m[0] + m[1] * m[1] + m[2] * m[2]);
    }
    void Normalize() {
        float len = Length();
        m[0] /= len;
        m[1] /= len;
        m[2] /= len;
    }
    void Print() {
        printf("(%f, %f, %f)\n", m[0], m[1], m[2]);
    }

    vec3 CrossProduct(vec3 b) {
        vec3 result;
        result.m[0] = m[1] * b.m[2] - m[2] * b.m[1];
        result.m[1] = m[2] * b.m[0] - m[0] * b.m[2];
        result.m[2] = m[0] * b.m[1] - m[1] * b.m[0];
        return result;
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
    mat3(float a0, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8) {
        m[0][0] = a0;   m[0][1] = a1;   m[0][2] = a2;
        m[1][0] = a3;   m[1][1] = a4;   m[1][2] = a5;
        m[2][0] = a6;   m[2][1] = a7;   m[2][2] = a8;
    }
    mat3 operator+(mat3 a) {
        mat3 temp;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                temp.m[i][j] = a.m[i][j] + m[i][j];
            }
        }
        return temp;
    }
    mat3 operator*(mat3 a) {
        mat3 result;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    result.m[i][j] += m[i][k] * a.m[k][j];
                }
            }
        }
        return result;
    }
   
    void LoadIdentity() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (i == j)		m[i][j] = 1;
                else 		m[i][j] = 0;
            }
        }
    }
    mat3 Transpose() {
        mat3 result;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                result.m[i][j] = m[j][i];
            }
        }
        return result;
    }
    void Print() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                printf("%f ", m[i][j]);
            }
            printf("\n");
        }
    }
    
};

vec3 operator*(mat3 x, vec3 v) {
    vec3 temp;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            temp.m[i] = x.m[i][j] * v.m[j];
        }
    }
    return temp;
}

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
    //
    vec3 v1(3.0, 4.0, 5.0), v2(1.0, 1.0, 1.0); // float 값을 받는 생성자 작성
    vec3 v3 = v1 - v2;		// - 연산자 오버로딩 v3 에는 (2,3,4) 대입됨
    v3.Print();				// 대략 (2.0000, 3.0000, 4.0000) 형식으로 printf
    printf("\n%f", v1.Length()); // v1벡터의 크기인 root(50), 대략 7.07... 이 출력. sqrtf함수를 사용할 것
    v2.Normalize();			// v2벡터가 단위벡터(크기가 1이고 방향은 동일)로 변경됨
    vec3 v4 = v1.CrossProduct(v2); // v1과 v2벡터의 외적 결과가 v4에 저장됨
    v4.Normalize();
    v4.Print();				// v4를 normalize한 결과를 출력


    mat3 m1(3, 0, 1, 2, 5, 3, 1, 2, 4), m2(2, 4, 1, 1, 3, 4, 5, 1, 0);// float 값을 9개 받는 생성자 작성
    m1.Print(); // 3.0000 0.0000 1.0000
                // 2.0000 5.0000 3.0000
                // 1.0000 2.0000 4.0000

    mat3 m3 = m1 + m2; // 행렬 덧셈 수행
    m3.Print();
    mat3 m4 = m1 * m2; // 행렬 곱 수행
    m4.Print();
    mat3 m5 = m1.Transpose();	// 전치행렬 생성, 여기서 m1은 변화되지 않는다.
    m5.Print();
    vec3 v5 = m1 * v1;	// mat3와 vec3를 곱한 곱을 vec3로 저장
    v5.Print();

    //
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