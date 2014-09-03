#include "effect.h"
#include "math.h"
#include <iostream>
using namespace std;

float PI = 3.14159;

vect tooning(float r, float g, float b, float i)
{
	float ratioRG = r/g, ratioRB = r/g;
    if (i < 0.1) {
        return vect (r * 0.5, g * 0.5, b * 0.5);
    } else if (i < 0.8) {
        return vect (r * 0.7, g * 0.7, b * 0.7);
	} else {
        return vect (r * 0.9, g * 0.9, b * 0.9);
    }
}

float anisotropic(vect lightPoint, vect pixelPoint, vect viewPoint, vect normal, float a, float b, float ks, float kd,int type)
{
    vect lightDir;
	
	//directional light

	if(type == 0) lightDir = (lightPoint).getUnitVector();
	//pointlight
	if(type == 1) lightDir = (lightPoint-pixelPoint).getUnitVector();
    normal = normal.getUnitVector();
	viewPoint = viewPoint.getUnitVector();
	vect halfVector = (viewPoint+lightDir).getUnitVector();
    float compA = sqrt((a + 1) * (b + 1)) / (8 * PI);
    vect uUn (normal.getY(), -normal.getX(), 0);
	vect u = uUn.getUnitVector();
    vect vUn (normal.getZ(), 0, -normal.getX());
	vect v = vUn.getUnitVector();
    float exp = ((a * pow(halfVector^u, 2)) + b * pow(halfVector^v, 2)) / (1 - pow(halfVector^normal, 2));
    float maxi = max(normal^lightDir, normal^viewPoint);
    float x;
    if ((normal^halfVector) < 0) {x = -(normal^halfVector);} else { x = normal^halfVector;}
    float compB = (pow(x, exp)) / (halfVector^lightDir * maxi);
    float compColor = ks + (1 - ks) * (1 - pow(halfVector^lightDir, 5));
    float difColor = 10 * (28 * kd) / (23 * PI) * (1 - ks) * (1 - pow(1 - 0.5 * (normal^lightDir), 5)) * (1 - pow(1 - 0.5 * (normal^viewPoint), 5));
  
    return 255 * ((compA * compB * compColor / 40) + difColor);

}