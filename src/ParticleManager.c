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
  Vector_zeroize(&pm->COM);

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
  //
  // if (!pm->currParticle->inUse) return ParticleManager_loopNext(pm);

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
  //if (pm->particles[0].inUse) printf("HERE\n");

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
      //printf("%lf, %lf, %lf\n", pm->particles[i].position.x, pm->particles[i].position.y, pm->particles[i].position.z);
      pm->particles[i].inUse = false;
    }
  }

  //set the COM
  pm->COM = ot->COM;

  /////////////////////////////////////////////////////////////////////////
  //  apply friction from drag force  (before gravity changes veloctiy)  //
  /////////////////////////////////////////////////////////////////////////


  ParticleList * pl = ParticleList_init();
  Particle * this = pm->particles;
  Particle * other;
  Vec3 searchOrigin = {0, 0, 0};
  Vec3 initialRelitiveVelocityThis;
  Vec3 initialRelitiveVelocityOther;
  Vec3 finalRelitiveVelocityThis;
  Vec3 finalRelitiveVelocityOther;
  Vec3 dragVelocityThis;
  Vec3 dragVelocityOther;

  for (unsigned int i = 0; i < pm->length; i++)
  {
    //printf("%lf\n", this->position.x);
    if (!this->inUse) continue;
    ParticleList_eraseList(pl);

    double searchRadius = this->radius * 2;
    searchOrigin.x = this->position.x - (searchRadius / 2);
    searchOrigin.x = this->position.x - (searchRadius / 2);
    searchOrigin.x = this->position.x - (searchRadius / 2);

    OctTree_queeryParticlesInArea(ot, pl, &searchOrigin, searchRadius);

    other = pl->particles;
    for (int k = 0; k < pl->elementCount; k++)
    {

      if (!other->inUse ||
        other->position.x == this->position.x ||
        other->position.y == this->position.y ||
        other->position.z == this->position.z) continue;

      if (other->mass < -0.1 || this->mass < -0.1)
      {
        printf("weird particle mass bug? something wrong with storing and getting from tree and list maybe\n");
        printf("%lf\n", other->mass);
        printf("%lf\n", this->mass);
        //printf("%lf\n", pl->particles[k].mass);
        for (unsigned int i = 0; i < pm->length; i++)
        {
          Particle * this = &pm->particles[i];
          printf("Mass: %lf\n", this->mass);
        }
        exit(1);
      }

      double dist = Vector_distance(&this->position, &other->position);

      if (dist > this->radius + other->radius) continue;
      //printf("%lf, %lf\n",p1Radius,p2Radius);

      PhysicsUtils_relitiveVelocities(&initialRelitiveVelocityThis, &initialRelitiveVelocityOther, this, other);

      //magnitude of velocity is the same for both by definition
      double relitiveVelocity = Vector_length(&initialRelitiveVelocityThis);

      if (relitiveVelocity < 0.0000001) continue;

      double minRadius = MIN(this->radius, other->radius);
      double areaOfContact = PI * minRadius * minRadius;
      //double ok = MIN(this->crossSectionalArea, other->crossSectionalArea);

      //if (ok != areaOfContact) printf("%lf, %lf\n", p2Radius, ok);

      double dragForceNewtonsThis = PhysicsUtils_dragForce(other->density, areaOfContact, relitiveVelocity);
      double dragForceNewtonsOther = PhysicsUtils_dragForce(this->density, areaOfContact, relitiveVelocity);

      if (other->radius * 2 <= dist)
      {
        //if here then p1 can find p2 and p2 can find p1. so
        //the drag calculations will occure twice on these particles
        //since the calculation will be the same both times, the fricion force
        //can be halfed each time so that after each particle finds each other
        //they add up to the full friction force
        dragForceNewtonsThis /= 2;
        dragForceNewtonsOther /= 2;
      }

      //compute drag force vector
      dragVelocityThis = initialRelitiveVelocityThis;
      dragVelocityOther = initialRelitiveVelocityOther;
      // printf("%lf\n", Vector_length(&dragVelocityThis));
      // printf("%lf\n", Vector_length(&dragVelocityOther));

      Vector_scale(&dragVelocityThis, -1);
      Vector_scale(&dragVelocityOther, -1);

      Vector_normalize(&dragVelocityThis);
      Vector_normalize(&dragVelocityOther);

      Vector_scale(&dragVelocityThis, dragForceNewtonsThis / this->mass);
      Vector_scale(&dragVelocityOther, dragForceNewtonsOther / other->mass);

      //calculate final relative velocity
      Vector_addCpy(&finalRelitiveVelocityThis, &dragVelocityThis, &this->velocity);
      Vector_addCpy(&finalRelitiveVelocityOther, &dragVelocityOther, &other->velocity);

      // finalRelitiveVelocityThis = dragVelocityThis;
      // finalRelitiveVelocityOther = dragVelocityOther;
      //calculate energy lost due to heat from initial and final relative velocities
      double enerygyLossThis = PhysicsUtils_calculateEnergyLoss(&initialRelitiveVelocityThis, &finalRelitiveVelocityThis, this->mass);
      double enerygyLossOther = PhysicsUtils_calculateEnergyLoss(&initialRelitiveVelocityOther, &finalRelitiveVelocityOther, other->mass);

      // printf("%lf\n", Vector_length(&dragVelocityOther));

      //add drag force to particle velocity and heat in joules
      Vector_add(&this->netAddedVelocity, &dragVelocityThis);
      Vector_add(&other->netAddedVelocity, &dragVelocityOther);

      this->heatJoules += enerygyLossThis;
      other->heatJoules += enerygyLossOther;

      //if (thisInitSpeed < thisFinalSpeed || otherInitSpeed < otherFinalSpeed) exit(1);
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

  ////////////////////////////////
  //  update particle velocity  //
  ////////////////////////////////

  for (unsigned int i = 0; i < pm->length; i++)
  {
    if (!pm->particles[i].inUse) continue;
    Particle_addNetVelocity(&pm->particles[i]);
  }

  /////////////////////////////////
  //  update positions and heat  //
  /////////////////////////////////

  for (unsigned int i = 0; i < pm->length; i++)
  {
    if (!pm->particles[i].inUse) continue;
    PhysicsUtils_updateParticalPosition(&pm->particles[i], pm->timeStep);
    // PhysicsUtils_updateHeatEnergy(&pm->particles[0], pm->timeStep);

  }
  // printf("%lf\n", pm->particles[0].position.x);
  // printf("%lf\n===\n", pm->particles[0].position.x);

  OctTree_free(ot, false);
}

////////////////////////
//  PRIVATE FUNCTIONS //
////////////////////////
