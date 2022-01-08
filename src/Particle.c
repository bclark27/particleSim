#include "Common.h"

#include "Particle.h"

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

void Particle_init(Particle * p)
{
  memset(p, 0, sizeof(Particle));
  Vector_zeroize(&p->position);
  Vector_zeroize(&p->velocity);

  p->mass = 1;
  p->density = 500;
  p->heatJoules = 1;

  //Particle_updateRadius(p);

  p->fixed = false;
  p->inUse = true;
}

double Particle_getRadius(Particle * p)
{
  return cbrt((3 * p->mass) / (4 * PI * p->density));
}
