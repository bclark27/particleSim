#ifndef CAMERA_H_
#define CAMERA_H_

#include "Common.h"
#include "Vector.h"

/////////////
//  TYPES  //
/////////////

typedef struct Camera
{
  Vec3 cameraPosition;
  Vec3 cameraRotation;
  Vec44 projectionMatrix;
  double nearPlane;
  double farPlane;
  double fov; //in degrees
} Camera;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

Camera * Camera_init(void);
void Camera_free(Camera * cam);

void Camera_setNearFarPlane(Camera * cam, double near, double far);
void Camera_setFOV(Camera * cam, double fov);
void Camera_updateProjMatrix(Camera * cam);
//returns true if the vector is in the frame
bool Camera_projectVec3Point(Camera * cam, Vec3 * v, Vec3 * ans);
void Camera_setPosition(Camera * cam, Vec3 * pos);
void Camera_setRotation(Camera * cam, Vec3 * angles);
#endif
