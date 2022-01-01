#ifndef VECTOR_H_
#define VECTOR_H_

/////////////
//  TYPES  //
/////////////

#pragma pack(1)
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
double Vector_length(Vector * v);
void Vector_normalize(Vector * v);

#endif
