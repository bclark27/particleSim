#include "Common.h"

#include "Particle.h"
#include "PhysicsUtils.h"

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

void updateConstants(Particle * p);

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

void Particle_init(Particle * p)
{
  memset(p, 0, sizeof(Particle));
  Vector_zeroize(&p->position);
  Vector_zeroize(&p->velocity);
  Vector_zeroize(&p->netAddedVelocity);

  p->mass = 1;
  p->density = 500;
  p->heatJoules = 0.0000000000000000000001;
  p->coolingConstant = 0.000000015;
  p->specificHeatCapacity = 4.12;
  p->luminocity = 0;

  updateConstants(p);

  p->fixed = false;
  p->inUse = true;
}

double Particle_getRadius(Particle * p)
{
  return cbrt((3 * p->mass) / (4 * PI * p->density));
}

void Particle_addNetVelocity(Particle * p)
{
  Vector_add(&p->velocity, &p->netAddedVelocity);
  Vector_zeroize(&p->netAddedVelocity);
}

double Particle_getBrightness(Particle * p, double dist)
{
  return PhysicsUtils_calculateBrightness(p, dist);
}

void Particle_setMass(Particle * p, double mass)
{
  p->mass = mass;
  updateConstants(p);
}

void Particle_setDensity(Particle * p, double density)
{
  p->density = density;
  updateConstants(p);
}

void Particle_resetDeltaStates(Particle * p)
{
  Vector_zeroize(&p->netAddedVelocity);
  p->heatJoulesDelta = 0;
}

/////////////////////////
//  PRIVATE FUNCTIONS  //
/////////////////////////

void updateConstants(Particle * p)
{
  p->radius = Particle_getRadius(p);
  p->surfaceArea = 4 * PI * p->radius * p->radius;
  p->volume = (4 * PI * p->radius * p->radius * p->radius) / 3.0;
  p->crossSectionalArea = PI * p->radius * p->radius;
}
