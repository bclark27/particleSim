#include "Common.h"

#include "ParticleManager.h"
#include "OctTree.h"
#include "ParticleFormation.h"
#include "PhysicsUtils.h"
#include "ParticleList.h"

///////////////
//  DEFINES  //
///////////////

#define HEAT_SEARCH_RADIUS_MULT 2

/////////////
//  TYPES  //
/////////////

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

void buildOctTree(ParticleManager * pm, OctTree * ot);
void updatePmStats(ParticleManager * pm, OctTree * ot);
void preformFrictionCalculations(ParticleManager * pm, OctTree * ot);
void preformGravityCalculations(ParticleManager * pm, OctTree * ot);
double calculateHeatLoss(Particle * p, OctTree * ot, double timeStep);
void resetAllDeltaStates(ParticleManager * pm);

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
  Particle * p;
  do
  {
    if (pm->currIndex >= pm->length) return NULL;
    p = pm->currParticle;
    pm->currParticle++;
    pm->currIndex++;
  } while (p->inUse == false);

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



  OctTree * ot = OctTree_init(&(Vec3){0, 0, 0}, pm->spaceCubeSideLength);
  buildOctTree(pm, ot);
  resetAllDeltaStates(pm);

  // set the pm stats
  updatePmStats(pm, ot);


  /////////////////////////////////////////////////////////////////////////
  //  apply friction from drag force  (before gravity changes veloctiy)  //
  /////////////////////////////////////////////////////////////////////////


  preformFrictionCalculations(pm, ot);


  ///////////////////////////////////
  //  add velocity due to gravity  //
  ///////////////////////////////////

  preformGravityCalculations(pm, ot);

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
    // pm->particles[i].heatJoulesDelta += calculateHeatLoss(&pm->particles[i], ot, pm->timeStep);
  }

  // for (unsigned int i = 0; i < pm->length; i++)
  // {
  //   if (!pm->particles[i].inUse) continue;
  //   PhysicsUtils_updateHeatEnergy(&pm->particles[i], pm->timeStep);
  // }

  OctTree_free(ot, false);
}

////////////////////////
//  PRIVATE FUNCTIONS //
////////////////////////

void buildOctTree(ParticleManager * pm, OctTree * ot)
{
  for (unsigned int i = 0; i < pm->length; i++)
  {
    if (!pm->particles[i].inUse) continue;
    if (!OctTree_insertParticle(ot, &pm->particles[i]))
    {
      //printf("%lf, %lf, %lf\n", pm->particles[i].position.x, pm->particles[i].position.y, pm->particles[i].position.z);
      pm->particles[i].inUse = false;
    }
  }
}

void updatePmStats(ParticleManager * pm, OctTree * ot)
{
  pm->COM = ot->COM;

  pm->particleCount = 0;
  pm->mass = 0;
  pm->heat = 0;
  OctTree_particleAreaStatsQueery(ot, &ot->position, ot->sideLength, &pm->particleCount, &pm->mass, &pm->heat);

}

void preformFrictionCalculations(ParticleManager * pm, OctTree * ot)
{
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

    // printf("===\n");
    // for (int i = 0; i < pm->length; i++)
    // {
    //   if (pm->particles[i].inUse)
    //   {
    //     printf("%i is in use\n", i);
    //   }
    //   else
    //   {
    //     printf("%i is in not use\n", i);
    //   }
    // }
    //
    // printf("%i\n", pl->elementCount);

    other = pl->particles;
    for (int k = 0; k < pl->elementCount; k++)
    {

      if (!other->inUse) continue;

      if (other == this) continue;

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
      Vector_addCpy(&finalRelitiveVelocityThis, &dragVelocityThis, &initialRelitiveVelocityThis);
      Vector_addCpy(&finalRelitiveVelocityOther, &dragVelocityOther, &initialRelitiveVelocityOther);

      //calculate energy lost due to heat from initial and final relative velocities
      double enerygyLossThis = PhysicsUtils_calculateEnergyLoss(&initialRelitiveVelocityThis, &finalRelitiveVelocityThis, this->mass);
      double enerygyLossOther = PhysicsUtils_calculateEnergyLoss(&initialRelitiveVelocityOther, &finalRelitiveVelocityOther, other->mass);


      // if (enerygyLossThis < 0) enerygyLossThis *= -1;
      // if (enerygyLossOther < 0) enerygyLossOther *= -1;

      // printf("%lf\n", Vector_length(&dragVelocityOther));

      //add drag force to particle velocity and heat in joules
      Vector_add(&this->netAddedVelocity, &dragVelocityThis);
      Vector_add(&other->netAddedVelocity, &dragVelocityOther);

      this->heatJoulesDelta += enerygyLossThis;
      other->heatJoulesDelta += enerygyLossOther;

      // if (enerygyLossThis > 0 || enerygyLossOther > 0)
      // {
      //   printf("%lf, %lf\n", PhysicsUtils_joulesToKelvin(enerygyLossThis), PhysicsUtils_joulesToKelvin(enerygyLossOther));
      //   exit(1);
      // }

      other++;

    }
    this++;
  }
  ParticleList_freeList(pl);
}

void preformGravityCalculations(ParticleManager * pm, OctTree * ot)
{
  for (unsigned int i = 0; i < pm->length; i++)
  {
    if (!pm->particles[i].inUse) continue;
    OctTree_updateVelocitySingle(ot, pm->thetaAccuracy, pm->timeStep, &pm->particles[i]);
  }
}

double calculateHeatLoss(Particle * p, OctTree * ot, double timeStep)
{
  unsigned int queeryCount = 0;
  double queeryMass = 0;
  double queeryHeat = 0;
  double searchRadius = HEAT_SEARCH_RADIUS_MULT;// * pm->particles[i].radius;
  OctTree_particleAreaStatsQueery(ot, &(Vec3){p->position.x - searchRadius,
                                            p->position.y - searchRadius,
                                            p->position.z - searchRadius},
                                            searchRadius * 2,
                                            &queeryCount,
                                            &queeryMass,
                                            &queeryHeat);
  queeryHeat -= p->heatJoules;
  queeryCount--;

  if (queeryCount) queeryHeat /= queeryCount;
  else queeryHeat = 0;
  double surroundingKelvin = PhysicsUtils_joulesToKelvin(queeryHeat);
  return PhysicsUtils_calculateHeatDelta(p->coolingConstant, p->heatJoules, surroundingKelvin, timeStep);
}

void resetAllDeltaStates(ParticleManager * pm)
{
  for (unsigned int i = 0; i < pm->length; i++)
  {
    if (!pm->particles[i].inUse) continue;

    Particle_resetDeltaStates(&pm->particles[i]);

  }
}
