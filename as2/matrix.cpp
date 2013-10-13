#include "math.h"
#include "matrix.h"
#include <stdio.h>
#include <iostream>

matrix::matrix()
	{
        
	//initialize matrix to 4x4 identity matrix
        this->m = new float*[4];
        for (int i = 0; i < 4; i++) {
            m[i] = new float[4];
        }
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                m[i][j] = 0;
            }
        }
	}

matrix::matrix(float w1, float w2, float w3, float w4, float x1, float x2, float x3, float x4, float y1, float y2, float y3, float y4, float z1, float z2, float z3, float z4)
	{
        this->m = new float*[4];
        for (int i = 0; i < 4; i++) {
            m[i] = new float[4];
        }
        m[0][0] = w1;
		m[0][1] = w2;
		m[0][2] = w3;
		m[0][3] = w4;
		m[1][0] = x1;
		m[1][1] = x2;
		m[1][2] = x3;
		m[1][3] = x4;
		m[2][0] = y1;
		m[2][1] = y2;
		m[2][2] = y3;
		m[2][3] = y4;
		m[3][0] = z1;
		m[3][1] = z2;
		m[3][2] = z3;
		m[3][3] = z4;
	}

float matrix::getVal(int i, int j) {
    return this->m[i][j];
}

matrix matrix::operator+(const matrix& matrix2)
	{
        matrix newMatrix;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                newMatrix.m[i][j] = this->m[i][j] + matrix2.m[i][j];
            }
        }
        return newMatrix;
	}
matrix matrix::operator*(const float multiplier)
	{
        matrix newMatrix;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                newMatrix.m[i][j] = this->m[i][j] * multiplier;
            }
        }
        return newMatrix;
	}

matrix matrix::operator*(const matrix& matrix2)
{
    matrix newMatrix;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                newMatrix.m[i][j] += (this->m[i][k] * matrix2.m[k][j]) ;
            }
        }
    }
    return newMatrix;
}

matrix matrix::operator-(const matrix& matrix2)
    {
        matrix newMatrix;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                newMatrix.m[i][j] = this->m[i][j] - matrix2.m[i][j];
            }
        }
        return newMatrix;
    }

vect matrix::operator*(const vect& v) // matrix multiplication with 4x1 vector
	{
        float newX=0.0f, newY=0.0f, newZ=0.0f;
            newX = this->m[0][0] * v.getX() + this->m[0][1] * v.getY() + this->m[0][2] * v.getZ();
            newY = this->m[1][0] * v.getX() + this->m[1][1] * v.getY() + this->m[1][2] * v.getZ();
            newZ = this->m[2][0] * v.getX() + this->m[2][1] * v.getY() + this->m[2][2] * v.getZ();
        return vect(newX, newY, newZ,0);
    }

matrix matrix::translate(float x, float y, float z)
    {
        return matrix(1, 0, 0, x, 0, 1, 0, y, 0, 0, 1, z, 0, 0, 0, 1);
    }

matrix matrix::scale(float x, float y, float z)
    {
        return matrix(x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1);
    }

matrix matrix::shear(float xy, float yx, float yz, float zy, float xz, float zx)
    {
        return matrix(1, xy, xz, 0, yx, 1, yz, 0, zx, zy, 1, 0, 0, 0, 0, 1);
    }

matrix matrix::rotateX(float angle)
    {
        return matrix(1, 0, 0, 0, 0, cos(angle), -sin(angle), 0, 0, sin(angle), cos(angle), 0, 0, 0, 0, 0);
    }

matrix matrix::rotateY(float angle)
    {
        return matrix(cos(angle), 0, sin(angle), 0, 0, 1, 0, 0, -sin(angle), 0, cos(angle), 0, 0, 0, 0, 0);
    }

matrix matrix::rotateZ(float angle)
    {
        return matrix(cos(angle), -sin(angle), 0, 0, sin(angle), cos(angle), 0, 0, 0, 0, 1, 0, 0, 0, 0, 0);
    }

matrix matrix::invert()
{
    float m[16];
    int a = 0;
    for (int k = 0; k < 4; k++) {
        for (int j = 0; j < 4; j++) {
            m[a] = this->m[k][j];
            a++;
        }
    }
    float inv[16], det;
    int i;
    
    inv[0] = m[5]  * m[10] * m[15] -
    m[5]  * m[11] * m[14] -
    m[9]  * m[6]  * m[15] +
    m[9]  * m[7]  * m[14] +
    m[13] * m[6]  * m[11] -
    m[13] * m[7]  * m[10];
    
    inv[4] = -m[4]  * m[10] * m[15] +
    m[4]  * m[11] * m[14] +
    m[8]  * m[6]  * m[15] -
    m[8]  * m[7]  * m[14] -
    m[12] * m[6]  * m[11] +
    m[12] * m[7]  * m[10];
    
    inv[8] = m[4]  * m[9] * m[15] -
    m[4]  * m[11] * m[13] -
    m[8]  * m[5] * m[15] +
    m[8]  * m[7] * m[13] +
    m[12] * m[5] * m[11] -
    m[12] * m[7] * m[9];
    
    inv[12] = -m[4]  * m[9] * m[14] +
    m[4]  * m[10] * m[13] +
    m[8]  * m[5] * m[14] -
    m[8]  * m[6] * m[13] -
    m[12] * m[5] * m[10] +
    m[12] * m[6] * m[9];
    
    inv[1] = -m[1]  * m[10] * m[15] +
    m[1]  * m[11] * m[14] +
    m[9]  * m[2] * m[15] -
    m[9]  * m[3] * m[14] -
    m[13] * m[2] * m[11] +
    m[13] * m[3] * m[10];
    
    inv[5] = m[0]  * m[10] * m[15] -
    m[0]  * m[11] * m[14] -
    m[8]  * m[2] * m[15] +
    m[8]  * m[3] * m[14] +
    m[12] * m[2] * m[11] -
    m[12] * m[3] * m[10];
    
    inv[9] = -m[0]  * m[9] * m[15] +
    m[0]  * m[11] * m[13] +
    m[8]  * m[1] * m[15] -
    m[8]  * m[3] * m[13] -
    m[12] * m[1] * m[11] +
    m[12] * m[3] * m[9];
    
    inv[13] = m[0]  * m[9] * m[14] -
    m[0]  * m[10] * m[13] -
    m[8]  * m[1] * m[14] +
    m[8]  * m[2] * m[13] +
    m[12] * m[1] * m[10] -
    m[12] * m[2] * m[9];
    
    inv[2] = m[1]  * m[6] * m[15] -
    m[1]  * m[7] * m[14] -
    m[5]  * m[2] * m[15] +
    m[5]  * m[3] * m[14] +
    m[13] * m[2] * m[7] -
    m[13] * m[3] * m[6];
    
    inv[6] = -m[0]  * m[6] * m[15] +
    m[0]  * m[7] * m[14] +
    m[4]  * m[2] * m[15] -
    m[4]  * m[3] * m[14] -
    m[12] * m[2] * m[7] +
    m[12] * m[3] * m[6];
    
    inv[10] = m[0]  * m[5] * m[15] -
    m[0]  * m[7] * m[13] -
    m[4]  * m[1] * m[15] +
    m[4]  * m[3] * m[13] +
    m[12] * m[1] * m[7] -
    m[12] * m[3] * m[5];
    
    inv[14] = -m[0]  * m[5] * m[14] +
    m[0]  * m[6] * m[13] +
    m[4]  * m[1] * m[14] -
    m[4]  * m[2] * m[13] -
    m[12] * m[1] * m[6] +
    m[12] * m[2] * m[5];
    
    inv[3] = -m[1] * m[6] * m[11] +
    m[1] * m[7] * m[10] +
    m[5] * m[2] * m[11] -
    m[5] * m[3] * m[10] -
    m[9] * m[2] * m[7] +
    m[9] * m[3] * m[6];
    
    inv[7] = m[0] * m[6] * m[11] -
    m[0] * m[7] * m[10] -
    m[4] * m[2] * m[11] +
    m[4] * m[3] * m[10] +
    m[8] * m[2] * m[7] -
    m[8] * m[3] * m[6];
    
    inv[11] = -m[0] * m[5] * m[11] +
    m[0] * m[7] * m[9] +
    m[4] * m[1] * m[11] -
    m[4] * m[3] * m[9] -
    m[8] * m[1] * m[7] +
    m[8] * m[3] * m[5];
    
    inv[15] = m[0] * m[5] * m[10] -
    m[0] * m[6] * m[9] -
    m[4] * m[1] * m[10] +
    m[4] * m[2] * m[9] +
    m[8] * m[1] * m[6] -
    m[8] * m[2] * m[5];
    
    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
    
    if (det == 0) {
        return matrix();
    }
    
    det = 1.0 / det;
    
    for (i = 0; i < 16; i++) {
        inv[i] = inv[i] * det;
    }
    return matrix(inv[0],inv[1],inv[2],inv[3],inv[4],inv[5],inv[6],inv[7],inv[8],inv[9],inv[10],inv[11],inv[12],inv[13],inv[14],inv[15]);
}

