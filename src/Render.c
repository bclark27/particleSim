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

inline double brightnessCap(double max, double bright);
void clearBuffers(Render * r);

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

Render * Render_init(unsigned int size, double maxBright)
{
  Render * r = malloc(sizeof(Render));
  r->size = size;

  r->heatBuffer = malloc(sizeof(double) * size * size);
  r->massBuffer = malloc(sizeof(double) * size * size);
  r->transparencyBuffer = malloc(sizeof(double) * size * size);
  r->brightnessBuffer = malloc(sizeof(double) * size * size);
  r->maxBright = maxBright;

  clearBuffers(r);

  return r;
}

void Render_free(Render * r)
{
  if (r == NULL) return;
  if (r->heatBuffer) free(r->heatBuffer);
  if (r->massBuffer) free(r->massBuffer);
  if (r->transparencyBuffer) free(r->transparencyBuffer);
  if (r->brightnessBuffer) free(r->brightnessBuffer);
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
      double dist = Vector_distance(&cam->cameraPosition, &p->position);// * 2.6;

      double fovRadius = dist * tan(DEG_TO_RAD(cam->fov) / 2);
      double screenRadius = (objRadius / fovRadius) * (r->size / 2);
      double sideLenMeters = objRadius / screenRadius;

      for (int y = MAX(-screenRadius + pixY, 0); y < MIN(screenRadius + pixY, r->size - 1); y++)
      {
        for (int x = MAX(-screenRadius + pixX, 0); x < MIN(screenRadius + pixX, r->size - 1); x++)
        {
          int dx = -(pixX - x);
          int dy = -(pixY - y);
          int index = y * r->size + x;

          double pixDist = sqrt(dx * dx + dy * dy);
          if (pixDist > screenRadius * screenRadius) continue;

          double distFromCenterMeters = MIN(objRadius, (pixDist / screenRadius) * objRadius);
          double depth = sqrt(objRadius * objRadius - distFromCenterMeters * distFromCenterMeters);

          double volume = sideLenMeters * sideLenMeters * depth;

          // double crossSectionMass = volume * p->density;
          // heat buffer[y][x] calcs
          r->heatBuffer[index] += p->heatJoules;
          // density buffer[y][x] calcs
          // r->transparencyBuffer[index] += depth * p->transparency;
          // r->transparencyBuffer[index] = MIN(r->transparencyBuffer[index], 1);
          r->brightnessBuffer[index] += Particle_getBrightness(p, dist);

          // mass buffer calcs
          r->massBuffer[index] += p->density * volume;
        }
      }
    }
    p = ParticleManager_loopNext(pm);
  }

  for (int i = 0; i < r->size * r->size; i++)
  {
    // r->brightnessBuffer[i] = brightnessCap(r->maxBright, r->brightnessBuffer[i]) / r->maxBright;
    r->brightnessBuffer[i] = MIN(r->brightnessBuffer[i], r->maxBright) / r->maxBright;
  }
}

/////////////////////////
//  PRIVATE FUNCTIONS  //
/////////////////////////

inline double brightnessCap(double max, double bright)
{
  return -pow(E, ((-bright / max) + max)) + max;
}

void clearBuffers(Render * r)
{
  for (int i = 0; i < r->size * r->size; i++)
  {
    r->heatBuffer[i] = 0;
    r->massBuffer[i] = 0;
    r->transparencyBuffer[i] = 0;
    r->brightnessBuffer[i] = 0;
  }
}
