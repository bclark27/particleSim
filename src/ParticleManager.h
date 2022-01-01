#ifndef PARTICLE_MANAGER_H_
#define PARTICLE_MANAGER_H_

#include "Common.h"
#include "List.h"
#include "ParticleTemp.h"

/////////////
//  TYPES  //
/////////////

typedef struct ParticleManager
{
  List * particle64List;
  unsigned long int particlesNotInUse;
} ParticleManager;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

ParticleManager * ParticleManager_init(void);
void ParticleManager_free(ParticleManager * pm);

void ParticleManager_addParticlesZeroized(ParticleManager * pm, unsigned int count);
void ParticleManager_addParticlesPreset(ParticleManager * pm, unsigned int count, ParticleTemp * pt);

#endif
