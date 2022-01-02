#ifndef PARTICLE_PORMATION_H_
#define PARTICLE_PORMATION_H_

#include "Common.h"
#include "List.h"
#include "Particle.h"
#include "Particle64.h"

/////////////
//  TYPES  //
/////////////

typedef struct ParticleFormation
{
  List * particle64List;
  unsigned int particlesNotInUse;
  unsigned long int particleCount;
} ParticleFormation;

typedef struct ParticleListIter
{
  Link * currentLink;
  int currentParticle;
} ParticleListIter;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

ParticleFormation * ParticleFormation_init(void);
void ParticleFormation_free(ParticleFormation * pf);

//TODO: add funcs for basic formation creation
void ParticleFormation_appendParticles(ParticleFormation * dest, ParticleFormation * src);
void ParticleFormation_cloudFormation(ParticleFormation * pf, unsigned int particleCount, double x, double y, double z, double avgParticleMass, double radius, double randomness, double velRand, double massRand);

void ParticleFormation_ParticleIter_init(ParticleFormation * pm, ParticleListIter * iter, unsigned int startIndex);
Particle * ParticleFormation_ParticleIter_next(ParticleListIter * iter);

#endif
