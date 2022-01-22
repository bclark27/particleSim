#include "Common.h"

#include "PhysicsUtils.h"
#include "Vector.h"
#include "UniversalConstants.h"

///////////////
//  DEFINES  //
///////////////

#define MAX_GRAVITY_FORCE 1

/////////////
//  TYPES  //
/////////////

////////////////////
//  PRIVATE VARS  //
////////////////////

Vec3 unitVec1;
Vec3 unitVec2;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

inline double speedOfLightForceCap(double force);
void speedOfLightVelocityCap(Vec3 * vel);

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

void PhysicsUtils_applyGravitationalForcePair(Particle * p1, Particle * p2)
{
  double r = Vector_distance(&p1->position, &p2->position);
  if (p1->radius + p2->radius > r) return;
  double r2 = r * r;

  // F = G * (m1 * m2) / r^2
  // a = F / m

  double a1 = speedOfLightForceCap(BIG_G * (p2->mass / r2));
  double a2 = speedOfLightForceCap(BIG_G * (p1->mass / r2));

  Vector_difference(&unitVec1, &p2->position, &p1->position);

  Vector_normalize(&unitVec1);

  unitVec2.x = -unitVec1.x;
  unitVec2.y = -unitVec1.y;
  unitVec2.z = -unitVec1.z;

  Vector_scale(&unitVec1, a1);
  Vector_scale(&unitVec2, a2);

  Vector_add(&p1->netAddedVelocity, &unitVec1);
  Vector_add(&p2->netAddedVelocity, &unitVec2);
}

void PhysicsUtils_applyGravitationalForceSingle(Particle * p1, Particle * p2)
{
  double r = Vector_distance(&p1->position, &p2->position);
  if (p1->radius + p2->radius > r) return;
  double r2 = r * r;

  // F = G * (m1 * m2) / r^2
  // a = F / m

  double a1 = speedOfLightForceCap(BIG_G * (p2->mass / r2));

  Vector_difference(&unitVec1, &p2->position, &p1->position);
  Vector_normalize(&unitVec1);
  Vector_scale(&unitVec1, a1);
  Vector_add(&p1->netAddedVelocity, &unitVec1);
}

void PhysicsUtils_applyGravitationalForceSingleFast(Particle * p1, Vec3 * vec, double mass)
{
  //if (mass < 0) {printf("mass err\n"); exit(1);}

  double r = Vector_distance(&p1->position, vec);
  if (p1->radius > r) return;
  double r2 = r * r;

  // F = G * (m1 * m2) / r^2
  // a = F / m

  double a1 = speedOfLightForceCap(BIG_G * (mass / r2));

  Vector_difference(&unitVec1, vec, &p1->position);
  Vector_normalize(&unitVec1);
  Vector_scale(&unitVec1, a1);
  Vector_add(&p1->netAddedVelocity, &unitVec1);
}

void PhysicsUtils_updateParticalPosition(Particle * p, double timeStep)
{

  if (p->fixed) return;
  speedOfLightVelocityCap(&p->velocity);
  p->position.x += p->velocity.x * timeStep;
  p->position.y += p->velocity.y * timeStep;
  p->position.z += p->velocity.z * timeStep;
  //printf("%lf\n", p->velocity.x);
}

Vec3 scratch;
Vec3 projAOntoDirection;
Vec3 projBOntoDirection;
void PhysicsUtils_relitiveVelocities(Vec3 * aTob, Vec3 * bToa, Particle * a, Particle * b)
{
  Vector_difference(&scratch, &a->position, &b->position);

  if (Vector_length(&scratch) == 0)
  {
    *aTob = (Vec3){0, 0, 0};
    *bToa = (Vec3){0, 0, 0};
  }

  Vector_projectOntoR1(&projAOntoDirection, &scratch, &a->velocity);
  Vector_projectOntoR1(&projBOntoDirection, &scratch, &b->velocity);

  // Vector_difference(aTob, &projBOntoDirection, &projAOntoDirection);
  // Vector_difference(bToa, &projAOntoDirection, &projBOntoDirection);

  Vector_difference(aTob, &projAOntoDirection, &projBOntoDirection);
  Vector_difference(bToa, &projBOntoDirection, &projAOntoDirection);
}

double PhysicsUtils_dragForce(double otherBodyDensity, double areaOfSelfContact, double selfRelVelocity)
{
  return 0.5 * otherBodyDensity * areaOfSelfContact * DRAG_COEFFICIENT * selfRelVelocity * selfRelVelocity;
}

void PhysicsUtils_applyForceToParticle(Particle * p, Vec3 * forceDirection, double newtons)
{
  Vector_normalize(forceDirection);
  // a = F / m
  Vector_scale(forceDirection, newtons / p->mass);
  Vector_add(&p->netAddedVelocity, forceDirection);
}

double PhysicsUtils_calculateEnergyLoss(Vec3 * velocityInitial, Vec3 * velocityFinal, double mass)
{
  double vInit = Vector_length(velocityInitial);
  double vFinal = Vector_length(velocityFinal);
  return 0.5 * mass * ((vInit * vInit) - (vFinal * vFinal));
}

double PhysicsUtils_calcEnergyGainedByParticle(Particle * p, double timeStep)
{
  return (p->heatJoulesDelta * BOLTZMANN_CONSTANT) / (p->mass * p->specificHeatCapacity);
}

double PhysicsUtils_calculateHeatDelta(double coolingConstant, double currentTemp, double surroundingTemp, double timeStep)
{
  double newTemp = PhysicsUtils_calculateHeatChange(PhysicsUtils_joulesToKelvin(currentTemp),
                                                        surroundingTemp,
                                                        coolingConstant,
                                                        timeStep);
  double newJoules = PhysicsUtils_KelvinToJoules(newTemp);

  return newJoules - currentTemp;
}

double PhysicsUtils_calculateRadiationGiveOff(Particle * p, double timeStep)
{
  double kelvin = PhysicsUtils_joulesToKelvin(p->heatJoules);
  return STEFAN_BOLTZMANN_CONSTANT * kelvin * kelvin * kelvin * kelvin;
}

double PhysicsUtils_calculateLumuns(Particle * p)
{
  double surfaceTemp = PhysicsUtils_joulesToKelvin(p->heatJoules);
  return p->surfaceArea * STEFAN_BOLTZMANN_CONSTANT * surfaceTemp * surfaceTemp * surfaceTemp * surfaceTemp;
}

double PhysicsUtils_calculateBrightness(Particle * p, double dist)
{
  return p->luminocity / (4 * PI * dist * dist);
}

double PhysicsUtils_joulesToKelvin(double joules)
{
  return joules / BOLTZMANN_CONSTANT;
}

double PhysicsUtils_KelvinToJoules(double kelvin)
{
  return kelvin * BOLTZMANN_CONSTANT;
}

double PhysicsUtils_calculateHeatChange(double objectTemp, double surroundingTemp, double coolingConst, double timeStep)
{
  return surroundingTemp + (objectTemp - surroundingTemp) * pow(E, -coolingConst * timeStep);
}

double PhysicsUtils_calculateSchwarzschildRadius(double mass)
{
  return (2 * mass * BIG_G) / (SPEED_OF_LIGHT * SPEED_OF_LIGHT);
}

/////////////////////////
//  PRIVATE FUNCTIONS  //
/////////////////////////

inline double speedOfLightForceCap(double force)
{
  if (force < 1) return force;
  return -pow(E, ((-force / SPEED_OF_LIGHT) + SPEED_OF_LIGHT_LN)) + SPEED_OF_LIGHT;
}

void speedOfLightVelocityCap(Vec3 * vel)
{
  double len = Vector_length(vel);
  if (len < SPEED_OF_LIGHT) return;
  len = SPEED_OF_LIGHT;
  Vector_normalize(vel);
  Vector_scale(vel, len);
}
