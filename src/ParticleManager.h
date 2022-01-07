#ifndef PARTICLE_MANAGER_H_
#define PARTICLE_MANAGER_H_

#include "Common.h"
#include "List.h"
#include "Particle.h"
#include "ParticleFormation.h"

/////////////
//  TYPES  //
/////////////

typedef struct ParticleManager
{
  Particle * particles;
  double timeStep;
  unsigned int length;
  unsigned int currIndex;
  Particle * currParticle;
} ParticleManager;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

ParticleManager * ParticleManager_init(void);
void ParticleManager_free(ParticleManager * pm);

void ParticleManager_loopInit(ParticleManager * pm);
Particle * ParticleManager_loopNext(ParticleManager * pm);

void ParticleManager_setTimeStep(ParticleManager * pm, double timeStep);
void ParticleManager_addFormation(ParticleManager * pm, ParticleFormation * pf);
void ParticleManager_updateParticles(ParticleManager * pm);

#endif
