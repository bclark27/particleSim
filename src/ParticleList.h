#ifndef PARTICLE_LIST_H_
#define PARTICLE_LIST_H_

#include "Common.h"
#include "Particle.h"

///////////////
//  DEFINES  //
///////////////

#define PARTICLE_ARRAY_LENGTH 128

/////////////
//  TYPES  //
/////////////

typedef struct ParticleList
{
  Particle ** particles;
  unsigned int particleListLen;
  unsigned int particleCount;

  //single loop vars
  Particle * singleCurrParticle;
  unsigned int singleCurrIndex;
  unsigned int singleCurrSubIndex;
  bool singleDoneLooping;

} ParticleList;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

ParticleList * ParticleList_init(void);
void ParticleList_free(ParticleList * pl);

void ParticleList_loopInit(ParticleList * pl);
Particle * ParticleList_loopNext(ParticleList * pl);

void ParticleList_doubleLoopInit(ParticleList * pl);
void ParticleList_nextPair(ParticleList * pl, Particle ** p1, Particle ** p2);

void ParticleList_allocateParticleSpace(ParticleList * pl, unsigned int count);

#endif
