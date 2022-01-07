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
  p->temp = 0;

  p->fixed = false;
  p->inUse = true;
}
