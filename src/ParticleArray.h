#ifndef PARTICLE_ARRAY_H_
#define PARTICLE_ARRAY_H_

#include "Common.h"
#include "Particle.h"

typedef struct ParticleArray
{
  unsigned int arraySize;
  unsigned int elementCount;
  Particle * particleArray;
} ParticleArray;

ParticleArray * ParticleArray_init(void);
void ParticleArray_free(ParticleArray * pa);

void ParticleArray_addParticle(ParticleArray * pa, Particle * p);
void ParticleArray_removeAtIndex(ParticleArray * pa, unsigned int index);

#endif
