#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "Common.h"
#include "ParticleTemp.h"

/////////////
//  TYPES  //
/////////////

typedef struct Particle64
{
  //8 byte
  unsigned long int inUse;
  unsigned long int fixed;
  Vector position[64];
  Vector velocity[64];

  //4 byte
  float mass[64];
  float density[64];
  float temp[64];

  //2 byte

  //1 byte

} Particle64;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

Particle64 * Particle64_init(void);
void Particle64_free(void * p);
void Particle64_setAttributesIndexed(Particle64 * p64, ParticleTemp * pt, unsigned int index);
void Particle64_setAttributesBlock(Particle64 * p64, ParticleTemp * pt);
void Particle64_setInUseIndex(Particle64 * p64, unsigned int index, bool inUse);
void Particle64_setInUseBlock(Particle64 * p64, bool inUse);

bool Particle64_hasNotUsedParticles(Particle64 * p64);
bool Particle64_indexIsBeingUsed(Particle64 * p64, unsigned int index);
#endif
