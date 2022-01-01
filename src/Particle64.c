#include "Common.h"

#include "Particle64.h"

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

Particle64 * Particle64_init(void)
{
  Particle64 * p = malloc(sizeof(Particle64));
  if(!p) return NULL;
  memset(p, 0, sizeof(Particle64));
  return p;
}

void Particle64_free(void * p64)
{
  Particle64 * p = (Particle64 *)p64;
  free(p);
}

void Particle64_setAttributesIndexed(Particle64 * p64, ParticleTemp * pt, unsigned int index)
{
  memcpy(&p64->position[index], &pt->position, sizeof(Vector));
  memcpy(&p64->velocity[index], &pt->velocity, sizeof(Vector));

  p64->mass[index] = pt->mass;
  p64->density[index] = pt->density;
  p64->temp[index] = pt->temp;

  unsigned long int i = 1;
  i <<= index;

  p64->fixed = (p64->fixed & ~i) | (i * pt->fixed);
  p64->inUse = (p64->inUse & ~i) | (i * pt->inUse);
}

void Particle64_setAttributesBlock(Particle64 * p64, ParticleTemp * pt)
{
  for (int i = 0; i < 64; i++)
  {
    Particle64_setAttributesIndexed(p64, pt, i);
  }
}

void Particle64_setInUseIndex(Particle64 * p64, unsigned int index, bool inUse)
{
  unsigned long int i = 1;
  i <<= index;
  p64->inUse = (p64->inUse & ~i) | (i * inUse);
}

void Particle64_setInUseBlock(Particle64 * p64, bool inUse)
{
  if (inUse)  {p64->inUse = 0xffffffffffffffff;}
  else        {p64->inUse = 0;}
}

bool Particle64_hasNotUsedParticles(Particle64 * p64)
{
  return ~p64->inUse;
}

bool Particle64_indexIsBeingUsed(Particle64 * p64, unsigned int index)
{
  unsigned long int i = 0;
  return (i << index) & p64->inUse;
}
