#include "Common.h"

#include "ParticleFormation.h"

/////////////
//  TYPES  //
/////////////


/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

void addParticle64ToFormation(ParticleFormation * pf, Particle64 * p64);

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

ParticleFormation * ParticleFormation_init()
{
  ParticleFormation * pf = malloc(sizeof(ParticleFormation));
  memset(pf, 0, sizeof(ParticleFormation));

  pf->particle64List = listInit();
  pf->particlesNotInUse = 0;
  pf->particleCount = 0;

  return pf;
}

void ParticleFormation_free(ParticleFormation * pf)
{
  destroyList(pf->particle64List, Particle64_free);
  free(pf);
}

void ParticleFormation_appendParticles(ParticleFormation * dest, ParticleFormation * src)
{
  dest->particlesNotInUse += src->particlesNotInUse;
  dest->particleCount += src->particleCount;

  unsigned int blockCount = listLen(src->particle64List);
  Link * currLink = src->particle64List->head;

  for (int i = 0; i < blockCount; i++)
  {
    Particle64 * p64 = Particle64_init();
    memcpy(p64, currLink->data, sizeof(Particle64));
    queue(dest->particle64List, p64);
    currLink = currLink->next;
  }
}

void ParticleFormation_cloudFormation(ParticleFormation * pf, unsigned int particleCount, double x, double y, double z, double avgParticleMass, double radius, double randomness, double velRand, double massRand)
{
  if (particleCount == 0) return;
  pf->particleCount = particleCount;
  pf->particlesNotInUse = 64 - (particleCount % 64);

  Particle p;
  Particle64 * p64 = Particle64_init();
  for (unsigned int i = 0; i < particleCount; i++)
  {
    Particle_init(&p);

    p.position.x = x + (RAND_DOUBLE * radius);
    p.position.y = y + (RAND_DOUBLE * radius);
    p.position.z = z + (RAND_DOUBLE * radius);

    p.mass = avgParticleMass + (RAND_DOUBLE * avgParticleMass * massRand);

    //Vec3 vel = {0, 0, -0.01};
    //p.velocity = vel;

    memcpy(&p64->particles[i % 64], &p, sizeof(Particle));

    if ((i % 64) == 63 || i == particleCount - 1)
    {
      addParticle64ToFormation(pf, p64);
      p64 = Particle64_init();
    }
  }
}

void ParticleFormation_ParticleIter_init(ParticleFormation * pf, ParticleListIter * iter, unsigned int startIndex)
{
  memset(iter, 0, sizeof(ParticleListIter));
  iter->currentLink = pf->particle64List->head;
  for (int i = 0; i < (startIndex / 64); i++) iter->currentLink = iter->currentLink->next;
  iter->currentParticle = startIndex % 64;
}

Particle * ParticleFormation_ParticleIter_next(ParticleListIter * iter)
{
  if (iter->currentLink)
  {
    while (iter->currentLink)
    {
      Particle64 * p64 = iter->currentLink->data;
      for (int i = iter->currentParticle; i < 64; i++)
      {

        if (p64->particles[i].inUse)
        {
          iter->currentParticle = i + 1;
          return &p64->particles[i];
        }
      }
      iter->currentParticle = 0;
      iter->currentLink = iter->currentLink->next;
    }
  }
  return NULL;
}

/////////////////////////
//  PRIVATE FUNCTIONS  //
/////////////////////////

void addParticle64ToFormation(ParticleFormation * pf, Particle64 * p64)
{
  queue(pf->particle64List, p64);
}
