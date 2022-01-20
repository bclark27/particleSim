#include "Common.h"

#include "Camera.h"

/////////////
//  TYPES  //
/////////////

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

Camera * Camera_init(void)
{
  Camera * cam = malloc(sizeof(Camera));
  Vector_zeroize(&cam->cameraPosition);
  Vector_zeroize(&cam->cameraRotation);
  Vector_zeroizeVec44(&cam->projectionMatrix);
  cam->nearPlane = 1;
  cam->farPlane = 10e15;
  cam->fov = 70;

  Camera_updateProjMatrix(cam);

  return cam;
}

void Camera_free(Camera * cam)
{
  free(cam);
}

void Camera_setNearFarPlane(Camera * cam, double near, double far)
{
  cam->nearPlane = near;
  cam->farPlane = far;
}

void Camera_updateProjMatrix(Camera * cam)
{
  Vec44 perspective;
  Vec44 translation;
  Vec44 rx;
  Vec44 ry;
  Vec44 rz;

  Vec44 scratch1;
  Vec44 scratch2;

  Vector_createPerspectiveMatrix(&perspective, cam->farPlane, cam->nearPlane, cam->fov);

  //move points opicit direction of camera
  Vector_createTranslationMatrix(&translation, -cam->cameraPosition.x,
                                                -cam->cameraPosition.y,
                                                -cam->cameraPosition.z);
  Vector_createRotationMatrixX(&rx, -cam->cameraRotation.x);
  Vector_createRotationMatrixY(&ry, -cam->cameraRotation.y);
  Vector_createRotationMatrixZ(&rz, -cam->cameraRotation.z);

  Vector_multiplyVec44(&scratch1, &translation, &rx);
  //Vector_printVec44(&scratch1);
  Vector_multiplyVec44(&scratch2, &scratch1, &ry);
  Vector_multiplyVec44(&scratch1, &scratch2, &rz);
  Vector_multiplyVec44(&cam->projectionMatrix, &scratch1, &perspective);


}

bool Camera_projectVec3Point(Camera * cam, Vec3 * v, Vec3 * ans)
{
  Vec4 temp;
  Vec4 temp2;
  temp.x = v->x;
  temp.y = v->y;
  temp.z = v->z;
  temp.w = 1;

  Vector_multVec4Vec44(&temp2, &temp, &cam->projectionMatrix);

  if (temp2.w != 1)
  {
    temp2.x /= temp2.w;
    temp2.y /= temp2.w;
    temp2.z /= temp2.w;
  }

  ans->x = temp2.x;
  ans->y = temp2.y;
  ans->z = temp2.z;

  return (ans->x > -1) && (ans->x < 1) &&
          (ans->y > -1) && (ans->y < 1) &&
          (ans->z > 0) && (ans->z < 1);
}

void Camera_setPosition(Camera * cam, Vec3 * pos)
{
  memcpy(&cam->cameraPosition, pos, sizeof(Vec3));
  Camera_updateProjMatrix(cam);
}

void Camera_moveCameraPosition(Camera * cam, Vec3 * deltaPos)
{
  cam->cameraPosition.x += deltaPos->x;
  cam->cameraPosition.y += deltaPos->y;
  cam->cameraPosition.z += deltaPos->z;

  Camera_updateProjMatrix(cam);
}

void Camera_setRotation(Camera * cam, Vec3 * angles)
{
  memcpy(&cam->cameraRotation, angles, sizeof(Vec3));
  Camera_updateProjMatrix(cam);
}
