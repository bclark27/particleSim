#include "Common.h"

#include "Vector.h"

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

double Vector_projectionScaler(Vec3 * v1, Vec3 * v2);

/////////////////////////
//  PRIVATE VARIABLES  //
/////////////////////////

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

void Vector_zeroize(Vec3 * v)
{
  v->x = 0;
  v->y = 0;
  v->z = 0;
}

void Vector_zeroizeVec4(Vec4 * v)
{
  v->x = 0;
  v->y = 0;
  v->z = 0;
  v->x = 0;
}

void Vector_zeroizeVec44(Vec44 * v)
{
  for (int i = 0; i < 4; i++)
    for (int k = 0; k < 4; k++)
      v->vals[i][k] = 0;
}

void Vector_identityVec44(Vec44 * v)
{
  for (int i = 0; i < 4; i++)
  {
    for (int k = 0; k < 4; k++)
    {
      if (i == k) {v->vals[i][k] = 1;}
      else        {v->vals[i][k] = 0;}
    }
  }
}

void Vector_add(Vec3 * v1, Vec3 * v2)
{
  v1->x += v2->x;
  v1->y += v2->y;
  v1->z += v2->z;
}

void Vector_scale(Vec3 * v, double scale)
{
  v->x *= scale;
  v->y *= scale;
  v->z *= scale;
}

void Vector_scaleCpy(Vec3 * dest, Vec3 * src, double scale)
{
  dest->x = src->x * scale;
  dest->y = src->y * scale;
  dest->z = src->z * scale;
}

double Vector_length(Vec3 * v)
{
  return sqrt((v->x * v->x) + (v->y * v->y) + (v->z * v->z));
}

void Vector_normalize(Vec3 * v)
{
  Vector_scale(v, 1 / Vector_length(v));
}

double Vector_distance(Vec3 * v1, Vec3 * v2)
{
  return sqrt(((v1->x - v2->x) * (v1->x - v2->x)) + ((v1->y - v2->y) * (v1->y - v2->y)) + ((v1->z - v2->z) * (v1->z - v2->z)));
}

void Vector_difference(Vec3 * ans, Vec3 * v1, Vec3 * v2)
{
  ans->x = v1->x - v2->x;
  ans->y = v1->y - v2->y;
  ans->z = v1->z - v2->z;
}

double Vector_innerProduct(Vec3 * v1, Vec3 * v2)
{
  return (v1->x * v2->x) + (v1->y * v2->y) + (v1->z * v2->z);
}

void Vector_projectOntoR2(Vec3 * ans, Vec3 * v1, Vec3 * v2, Vec3 * input)
{
  double scaler1 = Vector_projectionScaler(v1, input);
  double scaler2 = Vector_projectionScaler(v2, input);

  Vec3 u1;
  Vec3 u2;
  memcpy(&u1, v1, sizeof(Vec3));
  memcpy(&u2, v2, sizeof(Vec3));

  Vector_scale(&u1, scaler1);
  Vector_scale(&u2, scaler2);

  Vector_add(&u1, &u2);

  memcpy(ans, &u1, sizeof(Vec3));
}

void Vector_r3PerpendicularSpace(Vec3 * ans1, Vec3 * ans2, Vec3 * r1Space)
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

void Vector_r2OrthogonalSpace(Vec3 * v1, Vec3 * v2)
{
  //u1 = v1
  //u2 = v2 - (u1 dot v2) / (u1 dot u1) * u1
  double scaler = Vector_projectionScaler(v1, v2);

  Vec3 temp;
  memcpy(&temp, v1, sizeof(Vec3));

  Vector_scale(&temp, -scaler);

  Vector_add(v2, &temp);
}

void Vector_multiplyVec44(Vec44 * ans, Vec44 * v1, Vec44 * v2)
{
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      double result = 0;
      for (int k = 0; k < 4; k++)
      {
        result += v1->vals[i][k] * v2->vals[k][j];
      }
      ans->vals[i][j] = result;
    }
  }
}

void Vector_multVec4Vec44(Vec4 * ans, Vec4 * v1, Vec44 * v2)
{
  ans->x = (v1->x * v2->vals[0][0]) + (v1->y * v2->vals[1][0]) + (v1->z * v2->vals[2][0]) + (v1->w * v2->vals[3][0]);
  ans->y = (v1->x * v2->vals[0][1]) + (v1->y * v2->vals[1][1]) + (v1->z * v2->vals[2][1]) + (v1->w * v2->vals[3][1]);
  ans->z = (v1->x * v2->vals[0][2]) + (v1->y * v2->vals[1][2]) + (v1->z * v2->vals[2][2]) + (v1->w * v2->vals[3][2]);
  ans->w = (v1->x * v2->vals[0][3]) + (v1->y * v2->vals[1][3]) + (v1->z * v2->vals[2][3]) + (v1->w * v2->vals[3][3]);
}

void Vector_createPerspectiveMatrix(Vec44 * matrix, double farPlane, double nearPlane, double fov)
{
  Vector_zeroizeVec44(matrix);

  double s = (double)1 / (tan(DEG_TO_RAD(fov) / 2));
  double a = (-farPlane) / (farPlane - nearPlane);
  double b = (-farPlane * nearPlane) / (farPlane - nearPlane);

  matrix->vals[2][3] = -1;

  matrix->vals[0][0] = s;
  matrix->vals[1][1] = s;

  matrix->vals[2][2] = a;
  matrix->vals[3][2] = b;
}

void Vector_createTranslationMatrix(Vec44 * matrix, double x, double y, double z)
{
  Vector_identityVec44(matrix);

  matrix->vals[3][0] = x;
  matrix->vals[3][1] = y;
  matrix->vals[3][2] = z;
}

void Vector_createRotationMatrixX(Vec44 * matrix, double theta)
{
  double rads = DEG_TO_RAD(theta);
  Vector_identityVec44(matrix);
  double c = cos(rads);
  double s = sin(rads);

  matrix->vals[1][1] = c;
  matrix->vals[2][2] = c;

  matrix->vals[1][2] = s;
  matrix->vals[2][1] = -s;
}

void Vector_createRotationMatrixY(Vec44 * matrix, double theta)
{
  double rads = DEG_TO_RAD(theta);
  Vector_identityVec44(matrix);
  double c = cos(rads);
  double s = sin(rads);

  matrix->vals[0][0] = c;
  matrix->vals[2][2] = c;

  matrix->vals[2][0] = s;
  matrix->vals[0][2] = -s;
}

void Vector_createRotationMatrixZ(Vec44 * matrix, double theta)
{
  double rads = DEG_TO_RAD(theta);
  Vector_identityVec44(matrix);
  double c = cos(rads);
  double s = sin(rads);

  matrix->vals[0][0] = c;
  matrix->vals[1][1] = c;

  matrix->vals[0][1] = s;
  matrix->vals[1][0] = -s;
}

void Vector_printVec3(Vec3 * v)
{
  printf("x: %lf, y: %lf, z: %lf\n", v->x, v->y, v->z);
}

void Vector_printVec44(Vec44 * v)
{
  for (int i = 0; i < 4; i++)
  {
    printf("[");
    for (int k = 0; k < 4; k++)
    {
      printf("%lf", v->vals[i][k]);
      if (k != 3) printf(", ");
    }
    printf("]\n");
  }
}

/////////////////////////
//  PRIVATE FUNCTIONS  //
/////////////////////////

double Vector_projectionScaler(Vec3 * v1, Vec3 * v2)
{
  return Vector_innerProduct(v1, v2) / Vector_innerProduct(v1, v1);
}
