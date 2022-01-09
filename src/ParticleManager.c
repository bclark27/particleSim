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
  /////////////////////////////////////
  //  insert particles into the tree //
  /////////////////////////////////////

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

  /////////////////////////////////////////////////////////////////////////
  //  apply friction from drag force  (before gravity changes veloctiy)  //
  /////////////////////////////////////////////////////////////////////////

  ParticleList * pl = ParticleList_init();
  Particle * this = pm->particles;
  Particle * other;
  Vec3 searchOrigin = {0, 0, 0};
  Vec3 thisToOther;
  Vec3 otherToThis;
  Vec3 currentThisVelocity;
  Vec3 currentOtherVelocity;

  for (unsigned int i = 0; i < pm->length; i++)
  {
    if (!this->inUse) continue;
    ParticleList_eraseList(pl);

    double p1Radius = Particle_getRadius(this);
    double searchRadius = p1Radius * 2;
    searchOrigin.x = this->position.x - (searchRadius / 2);
    searchOrigin.x = this->position.x - (searchRadius / 2);
    searchOrigin.x = this->position.x - (searchRadius / 2);

    OctTree_queeryParticlesInArea(ot, pl, &searchOrigin, searchRadius);

    other = pl->particles;
    for (int k = 0; k < pl->elementCount; k++)
    {
      double p2Radius = Particle_getRadius(other);
      double dist = Vector_distance(&this->position, &other->position);

      if (dist > p1Radius + p2Radius) continue;

      PhysicsUtils_relitiveVelocities(&thisToOther, &otherToThis, this, other);

      //magnitude of velocity is the same for both by definition
      double relitiveVelocity = Vector_length(&thisToOther);

      double minRadius = MIN(p1Radius, p2Radius);
      double areaOfContact = PI * minRadius * minRadius;

      double dragForceThis = PhysicsUtils_dragForce(other->density, areaOfContact, relitiveVelocity);
      double dragForceOther = PhysicsUtils_dragForce(this->density, areaOfContact, relitiveVelocity);

      if (p2Radius * 2 <= dist)
      {
        //if here then p1 can find p2 and p2 can find p1. so
        //the drag calculations will occure twice on these particles
        //since the calculation will be the same both times, the fricion force
        //can be halfed each time so that after each particle finds each other
        //they add up to the full friction force

        dragForceThis /= 2;
        dragForceOther /= 2;
      }



      other++;
    }
    this++;
  }
  ParticleList_freeList(pl);

  ///////////////////////////////////
  //  add velocity due to gravity  //
  ///////////////////////////////////

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
