#include "vect.h"

vect tooning(float r, float g, float b, float i);

float anisotropic(vect lightPoint, vect pixelPoint, vect viewPoint, vect normal, float a, float b, float ks, float kd,int type);