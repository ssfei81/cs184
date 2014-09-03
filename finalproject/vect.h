#ifndef _VECT_H_
#define _VECT_H_

#include <iostream>
using namespace std;

class Vector;
class Point {
public:
    Point(): x(0), y(0), z(0) {}
    Point(float X, float Y, float Z): x(X), y(Y), z(Z) {}
    Point& operator=(Point point) {
        x = point.x;
        y = point.y;
        z = point.z;
        return *this;
    }
    Point operator+(Vector v);
    Point operator+(Point p) {
        return Point(x+p.x, y+p.y, z+p.z);
    }
    Point& operator+=(Point p) {
        x += p.x; y += p.y; z += p.z;
        return *this;
    }
    Point& operator+=(Vector v);
    Vector operator-(Point p);
    Point operator-() {
        return Point(-x, -y, -z);
    }
    Point operator*(float k) {
        return Point(x*k, y*k, z*k);
    }
    Point operator/(float k) {
        return Point(x/k, y/k, z/k);
    }
    Point& operator/=(float k) {
        if (k != 0) { x /= k; y /= k; z /= k; }
        return *this;
    }
    float norm() {
        return sqrt(x*x + y*y + z*z);
    }
    float square() {
        return x*x + y*y + z*z;
    }
    bool operator==(Point p) {
        return (x == p.x && y == p.y && z == p.z);
    }
    float x, y, z;
};
ostream& operator<<(ostream& os, const Point& p) {
    os << p.x << " " << p.y << " " << p.z;
    return os;
}
class Vector {
public:
    Vector(): x(0), y(0), z(0) {}
    Vector(float X, float Y, float Z): x(X), y(Y), z(Z) {}
    Vector& operator=(Vector vec) {
        x = vec.x;
        y = vec.y;
        z = vec.z;
        return *this;
    }
    Vector operator-() {
        Vector out = *this;
        out.x = -out.x; out.y = -out.y; out.z = -out.z;
        return out;
    }
    Vector operator+(Vector v) {
        return Vector(x+v.x, y+v.y, z+v.z);
    }
    Vector& operator+=(Vector v) {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }
    Vector operator-(Vector v) {
        return Vector(x-v.x, y-v.y, z-v.z);
    }
    Vector operator*(float m) {
        return Vector(m*x, m*y, m*z);
    }
    float operator*(Vector v) {
        return x*v.x + y*v.y + z*v.z;
    }
    Vector operator/(float d) {
        if (d != 0)
            return Vector(x/d, y/d, z/d);
        return Vector();
    }
    Vector& operator/=(float d) {
        if (d != 0) {
            x /= d; y /= d; z /= d;
        }
        return *this;
    }
    Vector cross(Vector v) {
        return Vector(y*v.z-v.y*z, z*v.x-x*v.z, x*v.y-y*v.x);
    }
    float norm() {
        return sqrt(x*x + y*y + z*z);
    }
    float square() {
        return x*x + y*y + z*z;
    }
    Vector& normalize() {
        float n = this -> norm();
        x /= n; y /= n; z /= n;
        return *this;
    }
    float getX() {
        return this->x;
    }
    float getY() {
        return this->y;
    }
    float getZ() {
        return this->z;
    }
    Vector& reset() {
        x = 0; y = 0; z = 0;
        return *this;
    }
    float x, y, z;
};
ostream& operator<<(ostream& os, const Vector& p) {
    os << p.x << " " << p.y << " " << p.z;
    return os;
}
Point Point::operator+(Vector v) {
    return Point(x+v.x, y+v.y, z+v.z);
}
Vector Point::operator-(Point p) {
    return Vector(x-p.x, y-p.y, z-p.z);
}
Point& Point::operator+=(Vector v) {
    x += v.x; y += v.y; z += v.z;
    return *this;
}
#endif