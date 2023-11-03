#include "Vector2.h"
#include <math.h>

float vLength(Vector2 v) {
	
	return sqrt(v.x * v.x + v.y*v.y);
}


float vSqrLength(Vector2 v) {
	return v.x * v.x + v.y * v.y;
}

