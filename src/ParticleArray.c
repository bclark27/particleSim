#include "ParticleArray.h"

///////////////
//  DEFINES  //
///////////////

#define DEFAULT_ARRAY_SIZE  1000
#define INC_SIZE            1000

/////////////
//  TYPES  //
/////////////

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

void addMoreSpace(ParticleArray * pa);

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

ParticleArray * ParticleArray_init(void)
{
  ParticleArray * pa = malloc(sizeof(ParticleArray));
  memset(pa, 0, sizeof(ParticleArray));
  pa->particleArray = malloc(DEFAULT_ARRAY_SIZE * sizeof(Particle));
  memset(pa->particleArray, 0, DEFAULT_ARRAY_SIZE * sizeof(Particle));

  pa->arraySize = DEFAULT_ARRAY_SIZE;
  pa->elementCount = 0;

  return pa;
}

void ParticleArray_free(ParticleArray * pa)
{
  free(pa->particleArray);
  free(pa);
}

void ParticleArray_addParticle(ParticleArray * pa, Particle * p)
{
  p->inUse = true;
  for (int i = 0; i < pa->arraySize; i++)
  {
    if (pa->particleArray[i].inUse == false)
    {
      memcpy(&pa->particleArray[i], p, sizeof(Particle));
      pa->elementCount++;
      return;
    }
  }

  unsigned int copyIndex = pa->arraySize;
  addMoreSpace(pa);
  memcpy(&pa->particleArray[copyIndex], p, sizeof(Particle));
  pa->elementCount++;
}

void ParticleArray_removeAtIndex(ParticleArray * pa, unsigned int index)
{
  if (index < pa->arraySize && pa->particleArray[index].inUse)
  {
    pa->particleArray[index].inUse = false;
    pa->elementCount--;
  }
}

////////////////////////
//  PRIVATE FUNCTIONS //
////////////////////////

void addMoreSpace(ParticleArray * pa)
{
  pa->particleArray = realloc(pa->particleArray, pa->arraySize + INC_SIZE);
  memset(pa->particleArray + pa->arraySize, 0, INC_SIZE * sizeof(Particle));
  pa->arraySize += INC_SIZE;
}
