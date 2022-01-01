#include "Common.h"

#include "ParticleTemp.h"

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

void ParticleTemp_init(ParticleTemp * p)
{
  memset(p, 0, sizeof(ParticleTemp));
  Vector_zeroize(&p->position);
  Vector_zeroize(&p->velocity);

  p->mass = 0;
  p->density = 0;
  p->temp = 0;

  p->fixed = false;
  p->inUse = true;
}
