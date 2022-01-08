#include "Common.h"

#include "ParticleManager.h"
#include "OctTree.h"
#include "ParticleFormation.h"
#include "PhysicsUtils.h"
#include "ParticleList.h"

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
  /*

  ParticleList * pl = ParticleList_init();
  Particle * this = pm->particles;
  Particle * other;
  Vec3 searchOrigin = {0, 0, 0};
  double p1Radius;
  double p2Radius;
  double dist;
  double searchRadius = 1;
  Vec3 thisToOther;
  Vec3 otherToThis;

  for (unsigned int i = 0; i < pm->length; i++)
  {
    if (!this->inUse) continue;
    ParticleList_eraseList(pl);

    p1Radius = Particle_getRadius(this);
    searchRadius = p1Radius * 2;
    searchOrigin.x = this->position.x - (searchRadius / 2);
    searchOrigin.x = this->position.x - (searchRadius / 2);
    searchOrigin.x = this->position.x - (searchRadius / 2);

    OctTree_queeryParticlesInArea(ot, pl, &searchOrigin, searchRadius);

    other = pl->particles;
    for (int k = 0; k < pl->elementCount; k++)
    {
      p2Radius = Particle_getRadius(other);
      dist = Vector_distance(&this->position, &other->position);

      if (dist > p1Radius + p2Radius) continue;

      PhysicsUtils_relitiveVelocities(&thisToOther, &otherToThis, this, other);

      if (p2Radius * 2 <= dist)
      {
        //if here that p1 can find p2 and p2 can find p1. so
        //the drag calculations will occure twice on these particles
        //since the calculation will be the same both times, the fricion force
        //can be halfed each time so that after each particle finds each other
        //they add up to the full friction force
      }
      else
      {

      }

      other++;
    }
    this++;
  }
  ParticleList_freeList(pl);
*/
  /*
  double mass = 0;
  double heat = 0;
  unsigned int count = 0;

  OctTree_particleAreaStatsQueery(ot, &zero, ot->sideLength, &count, &mass, &heat);

  printf("%i\n", count);

  */


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

  OctTree_free(ot, false);
}

////////////////////////
//  PRIVATE FUNCTIONS //
////////////////////////
