#include "Common.h"

#include "ParticleManager.h"
#include "OctTree.h"
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

ParticleManager * ParticleManager_init(double spaceSize, double thetaAccuracy)
{
  ParticleManager * pm = malloc(sizeof(ParticleManager));
  if(!pm) return NULL;
  memset(pm, 0, sizeof(ParticleManager));

  pm->timeStep = 1;
  pm->particles = NULL;
  pm->length = 0;
  pm->currIndex = 0;
  pm->spaceCubeSideLength = spaceSize;
  pm->thetaAccuracy = thetaAccuracy;

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

  if (!pm->currParticle->inUse) return ParticleManager_loopNext(pm);

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
  Vec3 zero = {0, 0, 0};
  OctTree * ot = OctTree_init(&zero, pm->spaceCubeSideLength);

  for (unsigned int i = 0; i < pm->length; i++)
  {
    if (!pm->particles[i].inUse) continue;

    if (!OctTree_insertParticle(ot, &pm->particles[i]))
    {
      pm->particles[i].inUse = false;
    }
  }

  for (unsigned int i = 0; i < pm->length; i++)
  {
    if (!pm->particles[i].inUse) continue;
    OctTree_updateVelocitySingle(ot, pm->thetaAccuracy, pm->timeStep, &pm->particles[i]);
  }

  for (unsigned int i = 0; i < pm->length; i++)
  {
    if (!pm->particles[i].inUse) continue;
    PhysicsUtils_updateParticalPosition(&pm->particles[i], pm->timeStep);
  }

  //OctTree_updateVelocities(ot, pm->thetaAccuracy, pm->timeStep);
  //OctTree_updatePositions(ot, pm->timeStep);

  OctTree_free(ot, false);
}

////////////////////////
//  PRIVATE FUNCTIONS //
////////////////////////
