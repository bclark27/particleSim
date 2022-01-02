#include "Common.h"

#include "Vector.h"

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

double Vector_projectionScaler(Vector * v1, Vector * v2);

/////////////////////////
//  PRIVATE VARIABLES  //
/////////////////////////

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

void Vector_scaleCpy(Vector * dest, Vector * src, double scale)
{
  dest->x = src->x * scale;
  dest->y = src->y * scale;
  dest->z = src->z * scale;
}

double Vector_length(Vector * v)
{
  return sqrt((v->x * v->x) + (v->y * v->y) + (v->z * v->z));
}

void Vector_normalize(Vector * v)
{
  Vector_scale(v, 1 / Vector_length(v));
}

double Vector_distance(Vector * v1, Vector * v2)
{
  return sqrt(((v1->x - v2->x) * (v1->x - v2->x)) + ((v1->y - v2->y) * (v1->y - v2->y)) + ((v1->z - v2->z) * (v1->z - v2->z)));
}

void Vector_difference(Vector * ans, Vector * v1, Vector * v2)
{
  ans->x = v1->x - v2->x;
  ans->y = v1->y - v2->y;
  ans->z = v1->z - v2->z;
}

double Vector_innerProduct(Vector * v1, Vector * v2)
{
  return (v1->x * v2->x) + (v1->y * v2->y) + (v1->z * v2->z);
}

void Vector_projectOntoR2(Vector * ans, Vector * v1, Vector * v2, Vector * input)
{
  double scaler1 = Vector_projectionScaler(v1, input);
  double scaler2 = Vector_projectionScaler(v2, input);

  Vector u1;
  Vector u2;
  memcpy(&u1, v1, sizeof(Vector));
  memcpy(&u2, v2, sizeof(Vector));

  Vector_scale(&u1, scaler1);
  Vector_scale(&u2, scaler2);

  Vector_add(&u1, &u2);

  memcpy(ans, &u1, sizeof(Vector));
}

void Vector_r3PerpendicularSpace(Vector * ans1, Vector * ans2, Vector * r1Space)
{
  ans1->x = -r1Space->y / r1Space->x;
  ans1->y = 1;
  ans1->z = 0;

  ans2->x = -r1Space->z / r1Space->x;
  ans2->y = 0;
  ans2->z = 1;

  Vector_r2OrthogonalSpace(ans1, ans2);

  Vector_normalize(ans1);
  Vector_normalize(ans2);
}

void Vector_r2OrthogonalSpace(Vector * v1, Vector * v2)
{
  //u1 = v1
  //u2 = v2 - (u1 dot v2) / (u1 dot u1) * u1
  double scaler = Vector_projectionScaler(v1, v2);

  Vector temp;
  memcpy(&temp, v1, sizeof(Vector));

  Vector_scale(&temp, -scaler);

  Vector_add(v2, &temp);
}

void Vector_print(Vector * v)
{
  printf("x: %lf, y: %lf, z: %lf\n", v->x, v->y, v->z);
}

/////////////////////////
//  PRIVATE FUNCTIONS  //
/////////////////////////

double Vector_projectionScaler(Vector * v1, Vector * v2)
{
  return Vector_innerProduct(v1, v2) / Vector_innerProduct(v1, v1);
}
