#include "Common.h"

#include "ParticleManager.h"
#include "Particle64.h"

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

ParticleManager * ParticleManager_init(void)
{
  ParticleManager * pm = malloc(sizeof(ParticleManager));
  if(!pm) return NULL;
  memset(pm, 0, sizeof(ParticleManager));

  pm->particle64List = listInit();

  return pm;
}

void ParticleManager_free(ParticleManager * pm)
{
  destroyList(pm->particle64List, Particle64_free);
  free(pm);
}

void ParticleManager_addParticlesZeroized(ParticleManager * pm, unsigned int count)
{
  ParticleTemp pt;
  ParticleTemp_init(&pt);
  ParticleManager_addParticlesPreset(pm, count, &pt);
}

void ParticleManager_addParticlesPreset(ParticleManager * pm, unsigned int count, ParticleTemp * pt)
{
  if (pm->particlesNotInUse != 0)
  {
    Link * linkPtr = pm->particle64List->head;
    int toFill = pm->particlesNotInUse;

    while (linkPtr && toFill > 0 && count > 0)
    {
      Particle64 * p64 = linkPtr->data;
      if (Particle64_hasNotUsedParticles(p64))
      {
        unsigned long int index = 1;
        for (int i = 0; i < 64; i++)
        {
          if (!Particle64_indexIsBeingUsed(p64, i))//if ((~use) & index)
          {
            Particle64_setAttributesIndexed(p64, pt, i);
            Particle64_setInUseIndex(p64, i, true);
            count--;
            toFill--;

            if(count == 0 || toFill == 0)
            {
              break;
            }
          }
          index <<= 1;
        }
      }
      linkPtr = linkPtr->next;
    }

    pm->particlesNotInUse = toFill;
    if (count == 0) return;
  }

  int fullBlocks = count / 64;
  bool partialBlock = (count % 64) != 0;
  int partialBlockCount = count - (fullBlocks * 64);

  for (int i = 0; i < fullBlocks; i++)
  {
    Particle64 * p64 = Particle64_init();
    Particle64_setAttributesBlock(p64, pt);
    Particle64_setInUseBlock(p64, true);
    queue(pm->particle64List, p64);
  }

  if (partialBlock)
  {
    Particle64 * p64 = Particle64_init();
    for (int i = 0; i < 64; i++)
    {
      if (i < partialBlockCount)
      {
        Particle64_setInUseIndex(p64, i, true);
        Particle64_setAttributesIndexed(p64, pt, i);
      }
      else
      {
        Particle64_setInUseIndex(p64, i, false);
      }
    }
    pm->particlesNotInUse = 64 - partialBlockCount;
    queue(pm->particle64List, p64);

  }
}
