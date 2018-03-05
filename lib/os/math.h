#ifndef _OS_MATH_H
#define _OS_MATH_H

#ifdef COCO
#define double float
double sqrt(double x);
double floorf(double x);
double tan(double x);
double fabs(double x);
double pow(double x, double y);
double round(double x);
#define M_PI (3.141592653f)
#else 
#include <math.h>
#endif

#endif // _OS_MATH_H
