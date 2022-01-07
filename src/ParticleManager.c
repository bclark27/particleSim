#include "Common.h"

#include "ParticleManager.h"
#include "ParticleFormation.h"
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
  pm->particles = NULL;
  pm->length = 0;
  pm->currIndex = 0;

  return pm;
}

void ParticleManager_free(ParticleManager * pm)
{
  free(pm->particles);
  free(pm);
}

void ParticleManager_loopInit(ParticleManager * pm)
{
  pm->currParticle = pm->particles;
  pm->currIndex = 0;
}

Particle * ParticleManager_loopNext(ParticleManager * pm)
{
  if (pm->currIndex >= pm->length) return NULL;

  Particle * p = pm->currParticle;
  pm->currParticle++;
  pm->currIndex++;
  return p;
}

void ParticleManager_setTimeStep(ParticleManager * pm, double timeStep)
{
  pm->timeStep = timeStep;
}

void ParticleManager_addFormation(ParticleManager * pm, ParticleFormation * pf)
{
  Particle * newParticles = malloc(sizeof(Particle) * (pf->length + pm->length));
  memcpy(newParticles, pm->particles, sizeof(Particle) * pm->length);
  memcpy(newParticles + pm->length, pf->particles, sizeof(Particle) * pf->length);
  free(pm->particles);
  pm->particles = newParticles;
  pm->length += pf->length;

}

void ParticleManager_updateParticles(ParticleManager * pm)
{
  for (unsigned int i = 0; i < pm->length; i++)
  {
    if (!pm->particles[i].inUse) continue;
      for (unsigned int k = i + 1; k < pm->length; k++)
      {
        if (!pm->particles[k].inUse) continue;
        PhysicsUtils_updateParticalPairVelocities(&pm->particles[i], &pm->particles[k], pm->timeStep);
      }
  }

  for (unsigned int i = 0; i < pm->length; i++)
  {
    if (!pm->particles[i].inUse) continue;
    PhysicsUtils_updateParticalPosition(&pm->particles[i], pm->timeStep);
  }
}

////////////////////////
//  PRIVATE FUNCTIONS //
////////////////////////
