#ifndef PARTICLE_LIST_H_
#define PARTICLE_LIST_H_

#include "Common.h"
#include "Particle.h"

/////////////
//  TYPES  //
/////////////

typedef struct ParticleList
{
  Particle * particles;
  unsigned int listLen;
  unsigned int elementCount;
} ParticleList;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

ParticleList * ParticleList_init(void);
void ParticleList_freeList(ParticleList * pl);

void ParticleList_append(ParticleList * pl, Particle * p);
void ParticleList_eraseList(ParticleList * pl);

#endif
