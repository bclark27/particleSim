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
  Vector_zeroize(&p->acceleration);
  Vector_zeroize(&p->netAcceleration);

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

Vec3 netVel;
Vec3 velFinal;
void Particle_updatePosition(Particle * p, double timeStep)
{
  netVel.x = (timeStep * (p->netAcceleration.x - p->acceleration.x) * timeStep + 2 * p->acceleration.x) / 2;
  netVel.y = (timeStep * (p->netAcceleration.y - p->acceleration.y) * timeStep + 2 * p->acceleration.y) / 2;
  netVel.z = (timeStep * (p->netAcceleration.z - p->acceleration.z) * timeStep + 2 * p->acceleration.z) / 2;

  velFinal.x = p->velocity.x + netVel.x;
  velFinal.y = p->velocity.y + netVel.y;
  velFinal.z = p->velocity.z + netVel.z;

  p->position.x = p->position.x + (0.5 * timeStep * (velFinal.x - p->velocity.x)) + timeStep * p->velocity.x;
  p->position.y = p->position.y + (0.5 * timeStep * (velFinal.y - p->velocity.y)) + timeStep * p->velocity.y;
  p->position.z = p->position.z + (0.5 * timeStep * (velFinal.z - p->velocity.z)) + timeStep * p->velocity.z;

  p->velocity = velFinal;
  Vector_add(&p->acceleration, &p->netAcceleration);
}

void Particle_applyForceToNetAccel(Particle * p, Vec3 * direction, double newtons)
{
  double accel = newtons / p->mass;
  Vector_normalize(direction);
  Vector_scale(direction, accel);
  Vector_add(&p->netAcceleration, direction);
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
  Vector_zeroize(&p->netAcceleration);
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
