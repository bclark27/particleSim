#include "ParticleList.h"

///////////////
//  DEFINES  //
///////////////

#define DEFAULT_LIST_LEN 100

////////////////////
//  PRIVATE VARS  //
////////////////////

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

void incresseSize(ParticleList * pl);

//////////////////////
// PUBLIC FUNCTIONS //
//////////////////////

ParticleList * ParticleList_init(void)
{
  ParticleList * pl = calloc(1, sizeof(ParticleList));
  pl->particles = calloc(DEFAULT_LIST_LEN, sizeof(Particle));
  pl->listLen = DEFAULT_LIST_LEN;
  pl->elementCount = 0;

  return pl;
}

void ParticleList_freeList(ParticleList * pl)
{
  if (pl->particles) free(pl->particles);
  free(pl);
}

void ParticleList_append(ParticleList * pl, Particle * p)
{
  if (pl->elementCount >= pl->listLen)
  {
    incresseSize(pl);
  }

  pl->particles[pl->elementCount] = *p;

  pl->elementCount++;
}

void ParticleList_eraseList(ParticleList * pl)
{
  memset(pl->particles, 0, sizeof(Particle) * pl->listLen);
  pl->elementCount = 0;
}

/////////////////////////
//  PRIVATE FUNCTIONS  //
/////////////////////////

void incresseSize(ParticleList * pl)
{
  Particle * p = calloc(DEFAULT_LIST_LEN + pl->listLen, sizeof(Particle));
  memcpy(p, pl->particles, sizeof(Particle) * pl->listLen);
  void * temp = pl->particles;
  pl->particles = p;
  pl->listLen += DEFAULT_LIST_LEN;
  free(temp);
}
