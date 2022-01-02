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

void Particle64_setAttributesIndexed(Particle64 * p64, Particle * pt, unsigned int index)
{
  memcpy(&p64->particles[index], pt, sizeof(Particle));

}

void Particle64_setAttributesBlock(Particle64 * p64, Particle * pt)
{
  for (int i = 0; i < 64; i++)
  {
    Particle64_setAttributesIndexed(p64, pt, i);
  }
}

void Particle64_setInUseIndex(Particle64 * p64, unsigned int index, bool inUse)
{
  p64->particles[index].inUse = inUse;
}

void Particle64_setInUseBlock(Particle64 * p64, bool inUse)
{
  for (int i = 0; i < 64; i++)
  {
    Particle64_setInUseIndex(p64, i, inUse);
  }
}

bool Particle64_hasNotUsedParticles(Particle64 * p64)
{
  for (int i = 0; i < 64; i++)
  {
    if (!p64->particles[i].inUse) return true;
  }
  return false;
}

bool Particle64_hasUsedParticles(Particle64 * p64)
{
  for (int i = 0; i < 64; i++)
  {
    if (p64->particles[i].inUse) return true;
  }
  return false;
}

bool Particle64_indexIsBeingUsed(Particle64 * p64, unsigned int index)
{
  return p64->particles[index].inUse;
}
