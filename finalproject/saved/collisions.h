#ifndef _COLLISION_H_
#define _COLLISION_H_

#include <cmath>
#include "vect.h"
#include <math.h>

using namespace std;

// cloth + ball params
float gravity = 9.8;
float t_step = 0.05; // time step
Vector u_fluid(10, 10, -10); // fluid speed
float ground = 1.5; // ground height

// cloth params
int width = 30;
int height = 17;
float springMass = 1;
float stiffness = 1; // force when spring is stretched
float clothDampingCoef = 10; // force against velocity
Point clothCenter(30, height/2+1, -80);
Vector clothOrientX(1, 0, 0.25); // two directions to determine the orientation of the cloth
Vector clothOrientY(0, 1, 0);
Vector orientX = clothOrientX.normalize(); // orientations of the cloth
Vector orientZ = orientX.cross(clothOrientY.normalize()).normalize();
Vector orientY = orientZ.cross(orientX);
float springLen = 1;
float fluidCoef = 1;
float tao = 0.1; // maximum stretch ratio

// ball params
float ballDist = 40; // distance from cloth to ball
float ballRadius = 1.5;
Point ballCenter(clothCenter.x + orientZ.x * ballDist + 20.5, ground + ballRadius + 1, clothCenter.z + orientZ.z * ballDist - 93);
float ballMass = 0.5;
float radiusRange = 1; // within this range, collision starts to occur
float bounceForceCoef = 0.03; // collision force coefficient
float ballDampingCoef = 0.1; // velocity damping force coefficient
float frictionCoef = 0.1; // friction force to ground
float strengthMin = 100; // kick strength minimum
float strengthStep = 10;
float strengthMax = 600;
float strength = strengthMin;
Vector forceDir(0, 0, -1);

class soccerball
{
public:
    soccerball() {};
    soccerball(Point c, float r, float m): center(c), radius(r), mass(m), force(Vector(0, 0, 0) * strength) {};
    void reset(Point c, Vector f)
    {
        center = c;
        force = f;
        acceleration.reset();
        v.reset();
        collisionForce.reset();
    }
    
    void update()
    {
        acceleration = force / mass;
        v += acceleration * t_step;
        center += v * t_step;
        force = Vector(0, -mass*gravity, 0);
        if (center.y <= radius + ground)
        {
            center.y = radius + ground;
            force.y = 0;
            force += -v * frictionCoef;
        }
        force += -v * ballDampingCoef;
        //force += normal[i*width+j] * ((u_fluid - velocity[i*width+j]) * normal[i*width+j]) * fluidCoef;
        force += collisionForce;
        collisionForce.reset();
        
    }
    Vector acceleration;
    Vector force;
    Vector collisionForce;
    Point center;
    float radius;
    float mass;
    Vector v = Vector(0, 0, 0); //Velocity
    Vector pos;
    float r; //Radius
    float rotation;
    
};

class surface
{
public:
    surface(int t) {type = t;};
    int type; //type == 0 (Wall); type == 1 (Base)
    float height;
    float left;
    float right;
    float bottom=0;
    Vector pos; //Position
    
    void setSurface(float h, float l, float r, Vector v, float b=0) {height = h; left = l; right = r; bottom = b; pos = v;};
    
};
class collision
{
public:
    int WALL = 0, BASE = 1;
    float rateOfEnergyLoss = 0.8;
 
    Vector rotateBall(Vector v, float angle) {
        angle = angle / 180.0f * 3.1415;
        float newX = v.getX() * cos(angle) + v.getZ() * sin(angle);
        float newY = v.getY();
        float newZ = v.getX() * -sin(angle) + v.getZ() * cos(angle);
        return Vector(newX, newY, newZ);
    }
    
    void rotationUpdate(soccerball *b) {
        if (b->v.getZ() > 0) {
            b->rotation +=  1.5 * b->v.norm();
        } else if (b->v.getZ() < 0) {
            b->rotation -= 1.5 * b->v.norm();
        } else {
            b->rotation = 0;
        }
    }
    
    bool testCollision(soccerball b, surface s) {
        Vector dir;
        Vector vx = Vector(1, 0, 0);
        Vector vy = Vector(0, 1, 0);
        if (s.type == WALL) { //Collision with Wall
            dir = Vector(0, 0, 1);
            return (b.pos)*dir + b.r + 0.5 > s.pos.getZ() && (b.pos)*dir + b.r + 0.5 < s.pos.getZ() + 0.5 //distance to wall is small enough
                    && s.left > b.pos.getX() && b.pos.getX() > s.right //left and right
                    && b.v.getZ() > 0
                    && s.height > b.pos.getY()
                    && s.bottom < b.pos.getY();
        } else if (s.type == BASE) { //Collision with Base
            dir = Vector(0, -1, 0);
            return (b.pos)*dir + b.r > 0.75 && (b.v)*dir > 0;
        }
    }
    
    void afterCollision(soccerball *b, surface s) {
        Vector dir;
        if (testCollision(*b, s)) {
            if (s.type == WALL) {
                dir = Vector(0, 0, -1); //soccerball bounces in opposite Z direction
            } else if (s.type == BASE) {
                dir = Vector(0, -1, 0); //*soccerball bounces in opposite Y direction
            }
                b->v = b->v + Vector(2, 0, 0) - (dir * 2) * (b->v*dir) * rateOfEnergyLoss;
        }
    }
};

#endif






