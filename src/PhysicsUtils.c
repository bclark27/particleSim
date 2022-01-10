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

double calculateHeatLoss(double objectTemp, double surroundingTemp, double coolingConst, double timeStep);

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

  double a1 = BIG_G * (p2->mass / r2);
  double a2 = BIG_G * (p1->mass / r2);

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

  double a1 = BIG_G * (p2->mass / r2);

  Vector_difference(&unitVec1, &p2->position, &p1->position);
  Vector_normalize(&unitVec1);
  Vector_scale(&unitVec1, a1);
  Vector_add(&p1->netAddedVelocity, &unitVec1);

  if (Vector_length(&p1->netAddedVelocity) > SPEED_OF_LIGHT)
  {
    printf("2:%lf, %lf\n", Vector_length(&unitVec1), a1);
    exit(1);
  }
}

void PhysicsUtils_applyGravitationalForceSingleFast(Particle * p1, Vec3 * vec, double mass)
{
  double r = Vector_distance(&p1->position, vec);
  double r2 = r * r;

  // F = G * (m1 * m2) / r^2
  // a = F / m

  double a1 = BIG_G * (mass / r2);

  Vector_difference(&unitVec1, vec, &p1->position);
  Vector_normalize(&unitVec1);
  Vector_scale(&unitVec1, a1);
  Vector_add(&p1->netAddedVelocity, &unitVec1);

  if (Vector_length(&p1->netAddedVelocity) > SPEED_OF_LIGHT)
  {
    printf("1:%lf, %lf\n", Vector_length(&unitVec1), r2);
    exit(1);
  }
}

void PhysicsUtils_updateParticalPosition(Particle * p, double timeStep)
{

  if (p->fixed) return;
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
  return 0.5 * mass * (vInit * vInit - vFinal * vFinal);
}

void PhysicsUtils_updateHeatEnergy(Particle * p, double timeStep)
{
  p->lostHeatJoules = PhysicsUtils_calculateRadiationGiveOff(p, timeStep);
  p->heatJoules -= p->lostHeatJoules;
}

double PhysicsUtils_calculateRadiationGiveOff(Particle * p, double timeStep)
{
  double kelvin = PhysicsUtils_joulesToKelvin(p->heatJoules);
  double newTemp = calculateHeatLoss(kelvin, 0, p->coolingConstant, timeStep);
  return PhysicsUtils_KelvinToJoules(kelvin - newTemp);
}

double PhysicsUtils_joulesToKelvin(double joules)
{
  return joules / BOLTZMANN_CONSTANT;
}

double PhysicsUtils_KelvinToJoules(double kelvin)
{
  return kelvin * BOLTZMANN_CONSTANT;
}

/////////////////////////
//  PRIVATE FUNCTIONS  //
/////////////////////////

double calculateHeatLoss(double objectTemp, double surroundingTemp, double coolingConst, double timeStep)
{
  return surroundingTemp + (objectTemp - surroundingTemp) * pow(E, -coolingConst * timeStep);
}
