#include "Common.h"

#include "Render.h"

/////////////
//  TYPES  //
/////////////

////////////////////
//  PRIVATE VARS  //
////////////////////

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

void clearBuffers(Render * r);

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

Render * Render_init(unsigned int size)
{
  Render * r = malloc(sizeof(Render));
  r->size = size;

  r->heatBuffer = malloc(sizeof(double) * size * size);
  r->massBuffer = malloc(sizeof(double) * size * size);
  r->transparencyBuffer = malloc(sizeof(double) * size * size);

  clearBuffers(r);

  return r;
}

void Render_free(Render * r)
{
  if (r == NULL) return;
  if (r->heatBuffer) free(r->heatBuffer);
  if (r->massBuffer) free(r->massBuffer);
  if (r->transparencyBuffer) free(r->transparencyBuffer);
  free(r);
}

void Render_renderBuffers(Render * r, ParticleManager * pm, Camera * cam)
{
  clearBuffers(r);
  ParticleManager_loopInit(pm);
  Particle * p = ParticleManager_loopNext(pm);
  Vec3 ans;
  unsigned int pixX;
  unsigned int pixY;

  while (p)
  {
    if (p->inUse && Camera_projectVec3Point(cam, &p->position, &ans))
    {
      pixX = (0.5 * (ans.x + 1)) * r->size;
      pixY = (1 - (ans.y + 1) * 0.5) * r->size;

      double objRadius = p->radius;
      double dist = Vector_distance(&cam->cameraPosition, &p->position);
      if (dist == 0) continue;

      double fovRadius = dist * atan(DEG_TO_RAD(cam->fov) / 2);
      double screenRadius = (objRadius / fovRadius) * r->size;
      double sideLenMeters = objRadius / screenRadius;

      for (int dy = -screenRadius; dy < screenRadius; dy++)
      {
        for (int dx = -screenRadius; dx < screenRadius; dx++)
        {
          int x = pixX + dx;
          int y = pixY + dy;
          int index = y * r->size + x;
          double pixDist = sqrt(dx * dx + dy * dy);
          if (x < 0 || y < 0 || x >= r->size || y >= r->size ||
            pixDist > screenRadius * screenRadius) continue;

          double distFromCenterMeters = MIN(objRadius, (pixDist / screenRadius) * objRadius);
          double depth = sqrt(objRadius * objRadius - distFromCenterMeters * distFromCenterMeters);

          double volume = sideLenMeters * sideLenMeters * depth;

          // double crossSectionMass = volume * p->density;
          // heat buffer[y][x] calcs

          // density buffer[y][x] calcs
          // r->transparencyBuffer[index] += depth * p->transparency;
          // r->transparencyBuffer[index] = MIN(r->transparencyBuffer[index], 1);

          // mass buffer calcs
          r->massBuffer[index] += p->density * volume;
        }
      }
    }
    p = ParticleManager_loopNext(pm);
  }
}

/////////////////////////
//  PRIVATE FUNCTIONS  //
/////////////////////////

void clearBuffers(Render * r)
{
  for (int i = 0; i < r->size * r->size; i++)
  {
    r->heatBuffer[i] = 0;
    r->massBuffer[i] = 0;
    r->transparencyBuffer[i] = 0;
  }
}
