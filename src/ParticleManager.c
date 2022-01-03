#include "Common.h"

#include "ParticleManager.h"
#include "ParticleFormation.h"
#include "Particle64.h"
#include "PhysicsUtils.h"

/////////////
//  TYPES  //
/////////////

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

  pm->timeStep = 1;
  pm->pf = ParticleFormation_init();


  ParticleFormation_ParticleIter_init(pm->pf, &pm->particleLoop, 0);

  return pm;
}

void ParticleManager_free(ParticleManager * pm)
{
  ParticleFormation_free(pm->pf);
  free(pm);
}

void ParticleManager_loopInit(ParticleManager * pm)
{
  ParticleFormation_ParticleIter_init(pm->pf, &pm->particleLoop, 0);
}

Particle * ParticleManager_loopNext(ParticleManager * pm)
{
  return ParticleFormation_ParticleIter_next(&pm->particleLoop);
}

void ParticleManager_setTimeStep(ParticleManager * pm, double timeStep)
{
  pm->timeStep = timeStep;
}

void ParticleManager_addFormation(ParticleManager * pm, ParticleFormation * pf)
{
  ParticleFormation_appendParticles(pm->pf, pf);
}

void ParticleManager_updateParticles(ParticleManager * pm)
{
  //update all particle velocities
  //then update positions according to the new velocities

  ParticleListIter outerLoop;
  ParticleListIter innerLoop;
  Particle * baseParticle;
  Particle * secondParticle;

  // UPDATE VELOCITIES

  ParticleFormation_ParticleIter_init(pm->pf, &outerLoop, 0);
  baseParticle = ParticleFormation_ParticleIter_next(&outerLoop);

  while (baseParticle)
  {
    memcpy(&innerLoop, &outerLoop, sizeof(ParticleListIter));
    secondParticle = ParticleFormation_ParticleIter_next(&innerLoop);

    while (secondParticle)
    {
      PhysicsUtils_updateParticalPairVelocities(baseParticle, secondParticle, pm->timeStep);
      secondParticle = ParticleFormation_ParticleIter_next(&innerLoop);
    }
    baseParticle = ParticleFormation_ParticleIter_next(&outerLoop);
  }

  // UPDATE POSITIONS

  ParticleFormation_ParticleIter_init(pm->pf, &outerLoop, 0);
  baseParticle = ParticleFormation_ParticleIter_next(&outerLoop);

  while (baseParticle)
  {
    PhysicsUtils_updateParticalPosition(baseParticle, pm->timeStep);
    baseParticle = ParticleFormation_ParticleIter_next(&outerLoop);
  }
}

////////////////////////
//  PRIVATE FUNCTIONS //
////////////////////////
