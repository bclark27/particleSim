#include "Common.h"

#include "ParticleList.h"

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

bool singleLoopGoUpOne(ParticleList * pl);

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

ParticleList * ParticleList_init(void)
{
  ParticleList * pl = malloc(sizeof(ParticleList));
  memset(pl, 0, sizeof(ParticleList));
  pl->particleListLen = 0;
  pl->particleCount = 0;

}

void ParticleList_free(ParticleList * pl)
{
  if (pl->particles == NULL) return;
  for (unsigned int i = 0; i < pl->particleListLen; i++)
  {
    if (pl->particles[i]) free(pl->particles[i]);
  }
  free(pl->particles);
}

void ParticleList_loopInit(ParticleList * pl)
{
  if (pl->particles == NULL) return;
  pl->singleCurrParticle = &pl->particles[0][0];
  pl->singleCurrIndex = 0;
  pl->singleCurrSubIndex = 0;
  pl->singleDoneLooping = false;
}

Particle * ParticleList_loopNext(ParticleList * pl)
{
  if (pl->singleDoneLooping) return NULL;

  bool inUseFound = pl->singleCurrParticle->inUse;
  while (inUseFound == false)
  {
    inUseFound = singleLoopGoUpOne(pl);
    if (pl->singleDoneLooping) return NULL;
  }

  Particle * ret = pl->singleCurrParticle;
  singleLoopGoUpOne(pl);
  return ret;
}

void ParticleList_doubleLoopInit(ParticleList * pl)
{

}

void ParticleList_nextPair(ParticleList * pl, Particle ** p1, Particle ** p2);

void ParticleList_allocateParticleSpace(ParticleList * pl, unsigned int count)
{
  unsigned int particlesToAllocate = count - (pl->particleListLen * PARTICLE_ARRAY_LENGTH - pl->particleCount);
  unsigned int chunksToAllocate = (particlesToAllocate / PARTICLE_ARRAY_LENGTH) + (1 * (particlesToAllocate % PARTICLE_ARRAY_LENGTH != 0));

  unsigned int newTotalLen = chunksToAllocate + pl->particleListLen;

  Particle ** newPtr = malloc(sizeof(Particle *) * newTotalLen);

  //cpy old ptr to new array
  memcpy(newPtr, pl->particles, pl->particleListLen * sizeof(Particle *));

  //free old ptr
  free(pl->particles);

  //allocate new spaces at the end of the new list
  for (unsigned int i = pl->particleListLen; i < newTotalLen; i++)
  {
    newPtr[i] = malloc(sizeof(Particle) * PARTICLE_ARRAY_LENGTH);

    //set all new particles to not being used
    for (int k = 0; k < PARTICLE_ARRAY_LENGTH; k++)
    {
      newPtr[i][k].inUse = false;
    }

  }

  //set list ptr
  pl->particles = newPtr;
}

/////////////////////////
//  PRIVATE FUNCTIONS  //
/////////////////////////

bool singleLoopGoUpOne(ParticleList * pl)
{
  pl->singleCurrSubIndex++;
  pl->singleCurrParticle++;

  if (pl->singleCurrSubIndex >= PARTICLE_ARRAY_LENGTH)
  {
    pl->singleCurrSubIndex = 0;
    pl->singleCurrIndex++;

    if (pl->singleCurrIndex >= pl->particleListLen)
    {
      pl->singleDoneLooping = true;
      return false;
    }

    pl->singleCurrParticle = &pl->particles[pl->singleCurrIndex][0];
  }

  return pl->singleCurrParticle->inUse;
}
