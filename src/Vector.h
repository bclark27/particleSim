#ifndef VECTOR_H_
#define VECTOR_H_

/////////////
//  TYPES  //
/////////////

typedef struct Vec3
{
  double x;
  double y;
  double z;
} Vec3;

typedef struct Vec4
{
  double x;
  double y;
  double z;
  double w;
} Vec4;

typedef struct Vec44
{
  double vals[4][4];
} Vec44;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

void Vector_zeroize(Vec3 * v);
void Vector_zeroizeVec4(Vec4 * v);
void Vector_zeroizeVec44(Vec44 * v);
void Vector_identityVec44(Vec44 * v);
//add v2 to v1
void Vector_add(Vec3 * v1, Vec3 * v2);
void Vector_scale(Vec3 * v, double scale);
void Vector_scaleCpy(Vec3 * dest, Vec3 * src, double scale);
double Vector_length(Vec3 * v);
void Vector_normalize(Vec3 * v);
double Vector_distance(Vec3 * v1, Vec3 * v2);
//v1 - v2
void Vector_difference(Vec3 * ans, Vec3 * v1, Vec3 * v2);
double Vector_innerProduct(Vec3 * v1, Vec3 * v2);
void Vector_projectOntoR2(Vec3 * ans, Vec3 * v1, Vec3 * v2, Vec3 * input);
void Vector_projectOntoR1(Vec3 * ans, Vec3 * onto, Vec3 * input);
void Vector_r3PerpendicularSpace(Vec3 * ans1, Vec3 * ans2, Vec3 * r1Space);
void Vector_r2OrthogonalSpace(Vec3 * v1, Vec3 * v2);
void Vector_multiplyVec44(Vec44 * ans, Vec44 * v1, Vec44 * v2);
void Vector_multVec4Vec44(Vec4 * ans, Vec4 * v1, Vec44 * v2);

// ALL FOV AND THETA COME IN AS DEGREES
void Vector_createPerspectiveMatrix(Vec44 * matrix, double farPlane, double nearPlane, double fov);
void Vector_createTranslationMatrix(Vec44 * matrix, double x, double y, double z);
void Vector_createRotationMatrixX(Vec44 * matrix, double theta);
void Vector_createRotationMatrixY(Vec44 * matrix, double theta);
void Vector_createRotationMatrixZ(Vec44 * matrix, double theta);

void Vector_printVec3(Vec3 * v);
void Vector_printVec44(Vec44 * v);
#endif
