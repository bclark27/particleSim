#ifndef VECTOR_H_
#define VECTOR_H_

/////////////
//  TYPES  //
/////////////

typedef struct Vector
{
  double x;
  double y;
  double z;
} Vector;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

void Vector_zeroize(Vector * v);
//add v2 to v1
void Vector_add(Vector * v1, Vector * v2);
void Vector_scale(Vector * v, double scale);
void Vector_scaleCpy(Vector * dest, Vector * src, double scale);
double Vector_length(Vector * v);
void Vector_normalize(Vector * v);
double Vector_distance(Vector * v1, Vector * v2);
void Vector_difference(Vector * ans, Vector * v1, Vector * v2);
double Vector_innerProduct(Vector * v1, Vector * v2);
void Vector_projectOntoR2(Vector * ans, Vector * v1, Vector * v2, Vector * input);
void Vector_r3PerpendicularSpace(Vector * ans1, Vector * ans2, Vector * r1Space);
void Vector_r2OrthogonalSpace(Vector * v1, Vector * v2);

void Vector_print(Vector * v);
#endif
