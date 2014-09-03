#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif
#include "collision.h"


#define PI 3.14159265  // Should be used from mathlib
inline float sqr(float x) { return x*x; }
inline float cube(float x) { return x*x*x; }
using namespace std;


enum MatrixType {TR, ROT, SC};
class Matrix {
public:
    Matrix() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (i == j)
                    mat[i][j] = 1;
                else
                    mat[i][j] = 0;
            }
        }
    }
    Matrix(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
           float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33) {
        mat[0][0] = m00; mat[0][1] = m01; mat[0][2] = m02; mat[0][3] = m03;
        mat[1][0] = m10; mat[1][1] = m11; mat[1][2] = m12; mat[1][3] = m13;
        mat[2][0] = m20; mat[2][1] = m21; mat[2][2] = m22; mat[2][3] = m23;
        mat[3][0] = m30; mat[3][1] = m31; mat[3][2] = m32; mat[3][3] = m33;
    }
    Matrix(int type, float p0, float p1, float p2, float p3 = 0) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (i == j)
                    mat[i][j] = 1;
                else
                    mat[i][j] = 0;
            }
        }
        if (type == TR) {
            mat[0][3] = p0;
            mat[1][3] = p1;
            mat[2][3] = p2;
        }
        else if (type == ROT) {
            float norm = sqrt(sqr(p0) + sqr(p1) + sqr(p2));
            p0 /= norm; p1 /= norm; p2 /= norm;
            float n[3][3] = {{0, -p2, p1}, {p2, 0, -p0}, {-p1, p0, 0}};
            float n_square[3][3];
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++)
                    n_square[i][j] = n[i][0]*n[0][j] + n[i][1]*n[1][j] + n[i][2]*n[2][j];
            }
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    mat[i][j] = mat[i][j] + sin(p3/180*PI) * n[i][j] + (1-cos(p3/180*PI)) * n_square[i][j];
                }
            }
        }
        else {
            mat[0][0] = p0;
            mat[1][1] = p1;
            mat[2][2] = p2;
        }
    }
    Matrix& operator=(Matrix m) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++)
                mat[i][j] = m.mat[i][j];
        }
        return *this;
    }
    Matrix transpose() {
        Matrix m;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++)
                m.mat[i][j] = mat[j][i];
        }
        return m;
    }
    Matrix operator*(Matrix m) {
        Matrix out;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                out.mat[i][j] = mat[i][0]*m.mat[0][j] + mat[i][1]*m.mat[1][j] + mat[i][2]*m.mat[2][j] + mat[i][3]*m.mat[3][j];
            }
        }
        return out;
    }
    Vector operator*(Vector v) {
        Vector out;
        out.x = mat[0][0]*v.x + mat[0][1]*v.y + mat[0][2]*v.z;
        out.y = mat[1][0]*v.x + mat[1][1]*v.y + mat[1][2]*v.z;
        out.z = mat[2][0]*v.x + mat[2][1]*v.y + mat[2][2]*v.z;
        return out;
    }
    Point operator*(Point p) {
        Point out;
        float w = mat[3][0]*p.x + mat[3][1]*p.y + mat[3][2]*p.z + mat[3][3];
        out.x = (mat[0][0]*p.x + mat[0][1]*p.y + mat[0][2]*p.z + mat[0][3]) / w;
        out.y = (mat[1][0]*p.x + mat[1][1]*p.y + mat[1][2]*p.z + mat[1][3]) / w;
        out.z = (mat[2][0]*p.x + mat[2][1]*p.y + mat[2][2]*p.z + mat[2][3]) / w;
        return out;
    }
    float mat[4][4];
};
class Color {
public:
    Color(): r(0), g(0), b(0) {}
    Color(float R, float G, float B): r(R), g(G), b(B) {}
    Color& operator=(Color c) {
        r = c.r;
        g = c.g;
        b = c.b;
        return *this;
    }
    Color operator+(Color c) {
        return Color(r+c.r, g+c.g, b+c.b);
    }
    Color& operator+=(Color c) {
        r += c.r; if (r > 1) r = 1;
        g += c.g; if (g > 1) g = 1;
        b += c.b; if (b > 1) b = 1;
        return *this;
    }
    Color operator*(Color c) {
        return Color(r*c.r, g*c.g, b*c.b);
    }
    Color operator*(float f) {
        return Color(r*f, g*f, b*f);
    }
    Color operator/(float f) {
        return Color(r/f, g/f, b/f);
    }
    bool operator>(float k) {
        return (r > k || g > k || b > k);
    }
    float r, g, b;
};
ostream& operator<<(ostream& os, const Color& c) {
    os << c.r << " " << c.g << " " << c.b << endl;
    return os;
}
class Face {
public:
    Face(int s): size(s) {
        vertexIdx = new int[size];
        vtIdx = new int[size];
        normalIdx = new int[size];
    }
    //~Face() { delete[] vertexIdx, normalIdx; }
    void addVertex(int idx, int v_idx, int vt_idx, int vn_idx) {
        vertexIdx[idx] = v_idx;
        vtIdx[idx] = vt_idx;
        normalIdx[idx] = vn_idx;
    }
    int size; // vertex number
    int* vertexIdx;
    int* vtIdx;
    int* normalIdx;
};

class Material {
public:
    Material() : name(""), Ns(0), d(1), map(false) {}
    Color Ka, Kd, Ks, Ke;
    float Ns, d;
    int illum;
    bool map;
    int map_Ka, map_Kd;
    string name;
};
// camera params
float x_min = INT_MAX, x_max = INT_MIN, y_min = INT_MAX, y_max = INT_MIN, z_min = INT_MAX, z_max = INT_MIN; // object boundary
float diam = 10; // object boundary diameter
Point center; // object center
float zoom = 1; // current zoom
float zoomFactor = 0.9; // zoom in/out step
Point lookfrom;
Point lookat;
Vector lookup(0, 1, 0);
bool ortho = false; // ortho or perspective

soccerball ball(ballCenter, ballRadius, ballMass);

class Cloth 
	{
public:
    Cloth() {
        force = new Vector[height*width];
        accelaration = new Vector[height*width];
        velocity = new Vector[height*width];
        position = new Point[height*width];
        normal = new Vector[height*width];
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Vector coor = (orientX * (j-width/2) + orientY * (height/2-i)) * springLen;
                position[i*width+j] = Point(clothCenter.x + coor.x, clothCenter.y + coor.y, clothCenter.z + coor.z);
            }
        }
        for (int i = 0; i < height-1; i++) {
            for (int j = 0; j < width-1; j++) {
                normal[i*width+j] = (position[(i+1)*width+j] - position[i*width+j]).cross(position[i*width+(j+1)] - position[i*width+j]).normalize();
            }
        }
    }
    ~Cloth() {
        delete[] force, accelaration, velocity, position, normal;
    }
    Vector springForce(Point a, Point b, float len) {
        Vector v = (b - a);
        return (v - (v/v.norm() * len)) * stiffness;
    }
    bool detectCollision(Point& a, soccerball& ball) {
        Point c = ball.center;
        float r = ball.radius + radiusRange;
        if ((a - c).norm() < r) {
            ball.collisionForce += (c - a) * (r / (c - a).norm()) * bounceForceCoef;
            a = c + (a - c).normalize() * (r + 0.01);
            return true;
        }
        return false;
    }
    void constrainLen(Point& a, Point& b, float len, bool fixed) {
        if (fixed) {
            b = a + (b - a).normalize() * len * (1+tao);
            if (detectCollision(b, ball)) constrainLen(a, b, len, fixed);
        }
        else {
            Point m = (a + b) / 2;
            a = m + (a - b).normalize() * len/2 * (1+tao);
            b = m + (b - a).normalize() * len/2 * (1+tao);
            if (detectCollision(a, ball)) constrainLen(a, b, len, fixed);
            if (detectCollision(b, ball)) constrainLen(a, b, len, fixed);
        }
    }
    void update() {
        // a, v and p
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (((i != 0) && (i != height-1)) || ((j != 0) && (j != width-1))) {
                    accelaration[i*width+j] = force[i*width+j] / springMass;
                    velocity[i*width+j] += accelaration[i*width+j] * t_step;
                    position[i*width+j] += velocity[i*width+j] * t_step;
                    detectCollision(position[i*width+j], ball);
                }
            }
        }
        
        // normal
        for (int i = 0; i < height-1; i++) {
            for (int j = 0; j < width-1; j++) { // down cross right
                normal[i*width+j] = (position[(i+1)*width+j] - position[i*width+j]).cross(position[i*width+(j+1)] - position[i*width+j]).normalize();
            }
        }
        for (int j = 1; j < width; j++) // bottom row: up cross left
            normal[(height-1)*width+j] = (position[(height-2)*width+j] - position[(height-1)*width+j]).cross(position[(height-1)*width+j-1] - position[(height-1)*width+j]);
        for (int i = 0; i < height-2; i++) // rightmost column: left cross down
            normal[i*width+(width-1)] = (position[i*width+(width-2)] - position[i*width+(width-1)]).cross(position[(i+1)*width+(width-1)] - position[i*width+(width-1)]);
        normal[(height-1)*width] = (position[(height-1)*width+1] - position[(height-1)*width]).cross(position[(height-2)*width] - position[(height-1)*width]); // lower left corner: right cross up;
        
        // force
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (position[i*width+j].y > ground) force[i*width+j] = Vector(0, -springMass*gravity, 0);
                else force[i*width+j] = Vector(0, 0, 0);
                
                if (i < height-1) {
                    force[i*width+j]     +=  springForce(position[i*width+j], position[(i+1)*width+j], springLen);
                    force[(i+1)*width+j] += -springForce(position[i*width+j], position[(i+1)*width+j], springLen);
                    if (i < height-2) {
                        force[i*width+j]     +=  springForce(position[i*width+j], position[(i+2)*width+j], springLen*2);
                        force[(i+2)*width+j] += -springForce(position[i*width+j], position[(i+2)*width+j], springLen*2);
                    }
                    if (j < width-1) {
                        force[i*width+j]         +=  springForce(position[i*width+j], position[(i+1)*width+(j+1)], springLen*sqrt(2));
                        force[(i+1)*width+(j+1)] += -springForce(position[i*width+j], position[(i+1)*width+(j+1)], springLen*sqrt(2));
                    }
                    if (j > 0) {
                        force[i*width+j]         +=  springForce(position[i*width+j], position[(i+1)*width+(j-1)], springLen*sqrt(2));
                        force[(i+1)*width+(j-1)] += -springForce(position[i*width+j], position[(i+1)*width+(j-1)], springLen*sqrt(2));
                    }
                }
                if (j < width-1) {
                    force[i*width+j]     +=  springForce(position[i*width+j], position[i*width+(j+1)], springLen);
                    force[i*width+(j+1)] += -springForce(position[i*width+j], position[i*width+(j+1)], springLen);
                    if (j < width-2) {
                        force[i*width+j]     +=  springForce(position[i*width+j], position[i*width+(j+2)], springLen*2);
                        force[i*width+(j+2)] += -springForce(position[i*width+j], position[i*width+(j+2)], springLen*2);
                    }
                }
                force[i*width+j] += -velocity[i*width+j] * clothDampingCoef;
                force[i*width+j] += normal[i*width+j] * ((u_fluid - velocity[i*width+j]) * normal[i*width+j]) * fluidCoef;
                
            }
        }
        
        // constraint spring length
        for (int t = 0; t < 5; t++) {
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    if ((i < height-1) && (position[(i+1)*width+j] - position[i*width+j]).norm() > springLen*(1+tao)) { // down
                        if ((i == 0) && (j == 0 || j == width-1)) constrainLen(position[i*width+j], position[(i+1)*width+j], springLen, true);
                        else if ((i == height-2) && (j == 0 || j == width-1)) constrainLen(position[(i+1)*width+j], position[i*width+j], springLen, true);
                        else constrainLen(position[i*width+j], position[(i+1)*width+j], springLen, false);
                    }
                    if ((j < width-1) && (position[i*width+(j+1)]  - position[i*width+j]).norm() > springLen*(1+tao)) { // right
                        if ((i == 0 || i == height-1) && j == 0) constrainLen(position[i*width], position[i*width+1], springLen, true);
                        else if ((i == 0 || i == height-1) && j == width-2) constrainLen(position[i*width+j+1], position[i*width+j], springLen, true);
                        else constrainLen(position[i*width+j], position[i*width+(j+1)], springLen, false);
                    }
                    if (((i < height-1) && (j < width-1)) && (position[(i+1)*width+(j+1)] - position[i*width+j]).norm() > springLen*sqrt(2)*(1+tao)) { // lower right
                        if (i == 0 && j == 0) constrainLen(position[0], position[width+1], springLen*sqrt(2), true);
                        else if (i == height-2 && j == width-2) constrainLen(position[(i+1)*width+(j+1)], position[i*width+j], springLen*sqrt(2), true);
                        else constrainLen(position[i*width+j], position[(i+1)*width+(j+1)], springLen*sqrt(2), false);
                    }
                    if (((i < height-1) && (j > 0)) && (position[(i+1)*width+(j-1)] - position[i*width+j]).norm() > springLen*sqrt(2)*(1+tao)) { // lower left
                        if (i == 0 && j == width-1) constrainLen(position[width-1], position[2*width-2], springLen*sqrt(2), true);
                        else if (i == height-2 && j == 1) constrainLen(position[(i+1)*width+(j-1)], position[i*width+j], springLen*sqrt(2), true);
                        else constrainLen(position[i*width+j], position[(i+1)*width+(j-1)], springLen*sqrt(2), false);
                    }
                    if (position[i*width+j].y < ground) position[i*width+j].y = ground;
                }
            }
        }
    }
    
    Vector* force;
    Vector* accelaration;
    Vector* velocity;
    Point* position;
    Vector* normal;
};
Cloth cloth;

void drawBar(int length) {
	glEnable(GL_COLOR_MATERIAL);
    glMatrixMode(GL_PROJECTION); // change the current matrix to PROJECTION
    double matrix[16]; // 16 doubles in stack memory
    glGetDoublev(GL_PROJECTION_MATRIX, matrix); // get the values from PROJECTION matrix to local variable
    glLoadIdentity(); // reset PROJECTION matrix to identity matrix
    glOrtho(0, 800, 0, 600, -5, 5); // orthographic perspective
    glMatrixMode(GL_MODELVIEW); // change current matrix to MODELVIEW matrix again
    glLoadIdentity(); // reset it to identity matrix
    glPushMatrix(); // push current state of MODELVIEW matrix to stack
    glLoadIdentity(); // reset it again. (may not be required, but it my convention)
    glColor3f(0, 1, 0);
    glBegin(GL_POLYGON);
    glVertex3f(10, 10, -1);
    glVertex3f(10, length/2, -1);
    glVertex3f(50, length/2, -1);
    glVertex3f(50, 10, -1);
    glEnd();
    glPopMatrix(); // get MODELVIEW matrix value from stack
    glMatrixMode(GL_PROJECTION); // change current matrix mode to PROJECTION
    glLoadMatrixd(matrix); // reset
    glMatrixMode(GL_MODELVIEW); // change current matrix mode to MODELVIEW
	glDisable(GL_COLOR_MATERIAL);
}

void draw() {
    
    // draw cloth
    GLfloat mat_specular[] = { 0.3, 0.3, 0.3, 1.0 };
    GLfloat mat_diffuse[]  = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat mat_ambient[]  = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat mat_shininess  = { 10 };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
    for (int i = 0; i < height-1; i++) {
        for (int j = 0; j < width-1; j++) {
            glBegin(GL_LINE_LOOP);
            Point p1 = cloth.position[i*width+j];
            Vector v1 = cloth.normal[i*width+j];
            Point p2 = cloth.position[i*width+(j+1)];
            Vector v2 = cloth.normal[i*width+(j+1)];
            Point p3 = cloth.position[(i+1)*width+(j+1)];
            Vector v3 = cloth.normal[(i+1)*width+(j+1)];
            Point p4 = cloth.position[(i+1)*width+j];
            Vector v4 = cloth.normal[(i+1)*width+j];
            //glColor3f(1, 1, 1);
            glNormal3f(v1.x, v1.y, v1.z); glVertex3f(p1.x, p1.y, p1.z);
            glNormal3f(v2.x, v2.y, v2.z); glVertex3f(p2.x, p2.y, p2.z);
            glNormal3f(v3.x, v3.y, v3.z); glVertex3f(p3.x, p3.y, p3.z);
            glNormal3f(v4.x, v4.y, v4.z); glVertex3f(p4.x, p4.y, p4.z);
            glEnd();
        }
    }
    
//    // draw ball
//    GLfloat mat_specular2[] = { 0.3, 0.3, 0.3, 1.0 };
//    GLfloat mat_diffuse2[]  = { 0.0, 0.8, 0.0, 1.0 };
//    GLfloat mat_ambient2[]  = { 0.1, 0.1, 0.1, 1.0 };
//    GLfloat mat_shininess2  = { 10 };
//    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular2);
//    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient2);
//    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse2);
//    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess2);
//    glPushMatrix();
//    //glTranslatef(ball.center.x + 20, ball.center.y + 10, ball.center.z - 100);
//    //glTranslatef(50,5, -120);
//    float ballScale = 1;
//    //float ballScale = 0.;
//    glScalef(ballScale, ballScale, ballScale);
//    glutSolidSphere(ball.radius, 100, 100);
//    glPopMatrix();
    
    // draw power bar
}
