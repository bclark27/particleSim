#include "Common.h"

#include "Vector.h"

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

void Vector_zeroize(Vector * v)
{
  v->x = 0;
  v->y = 0;
  v->z = 0;
}

void Vector_add(Vector * v1, Vector * v2)
{
  v1->x += v2->x;
  v1->y += v2->y;
  v1->z += v2->z;
}

void Vector_scale(Vector * v, double scale)
{
  v->x *= scale;
  v->y *= scale;
  v->z *= scale;
}

double Vector_length(Vector * v)
{
  return sqrt((v->x * v->x) + (v->y * v->y) + (v->z * v->z));
}

void Vector_normalize(Vector * v)
{
  double len = Vector_length(v);
  Vector_scale(v, 1 / len);
}
