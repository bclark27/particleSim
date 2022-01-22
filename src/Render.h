#ifndef RENDER_H_
#define RENDER_H_

#include "Vector.h"
#include "Particle.h"
#include "ParticleManager.h"
#include "Camera.h"

/////////////
//  TYPES  //
/////////////

typedef struct Render
{
  unsigned int size;
  double * heatBuffer;
  double * transparencyBuffer;
  double * brightnessBuffer;
  double * massBuffer;
  double maxBright;
} Render;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

Render * Render_init(unsigned int size, double maxBright);
void Render_free(Render * r);
void Render_renderBuffers(Render * r, ParticleManager * pm, Camera * cam);

#endif
