#ifndef PARTICLE64_H_
#define PARTICLE64_H_

#include "Common.h"
#include "Particle.h"

/////////////
//  TYPES  //
/////////////

typedef struct Particle64
{
  //8 byte
  Particle particles[64];

} Particle64;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

Particle64 * Particle64_init(void);
void Particle64_free(void * p);
void Particle64_setAttributesIndexed(Particle64 * p64, Particle * pt, unsigned int index);
void Particle64_setAttributesBlock(Particle64 * p64, Particle * pt);
void Particle64_setInUseIndex(Particle64 * p64, unsigned int index, bool inUse);
void Particle64_setInUseBlock(Particle64 * p64, bool inUse);

bool Particle64_hasNotUsedParticles(Particle64 * p64);
bool Particle64_hasUsedParticles(Particle64 * p64);
bool Particle64_indexIsBeingUsed(Particle64 * p64, unsigned int index);
#endif
