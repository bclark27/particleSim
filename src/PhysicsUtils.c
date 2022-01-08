#include "Common.h"

#include "PhysicsUtils.h"
#include "Vector.h"
#include "UniversalConstants.h"

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

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

void PhysicsUtils_updateParticalPairVelocities(Particle * p1, Particle * p2)
{
  double r = Vector_distance(&p1->position, &p2->position);
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

  Vector_add(&p1->velocity, &unitVec1);
  Vector_add(&p2->velocity, &unitVec2);

}

void PhysicsUtils_updateSingleParticalVelocity(Particle * p1, Particle * p2)
{
  double r = Vector_distance(&p1->position, &p2->position);
  double r2 = r * r;

  // F = G * (m1 * m2) / r^2
  // a = F / m

  double a1 = BIG_G * (p2->mass / r2);

  Vector_difference(&unitVec1, &p2->position, &p1->position);
  Vector_normalize(&unitVec1);
  Vector_scale(&unitVec1, a1);
  Vector_add(&p1->velocity, &unitVec1);
}

void PhysicsUtils_updateSingleParticalVelocityFast(Particle * p1, Vec3 * vec, double mass)
{
  double r = Vector_distance(&p1->position, vec);
  double r2 = r * r;

  // F = G * (m1 * m2) / r^2
  // a = F / m

  double a1 = BIG_G * (mass / r2);

  Vector_difference(&unitVec1, vec, &p1->position);
  Vector_normalize(&unitVec1);
  Vector_scale(&unitVec1, a1);
  Vector_add(&p1->velocity, &unitVec1);
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

  Vector_difference(aTob, &projBOntoDirection, &projAOntoDirection);
  Vector_difference(bToa, &projAOntoDirection, &projBOntoDirection);
}

double PhysicsUtils_dragForce(double otherBodyDensity, double areaOfSelfContact, double selfRelVelocity)
{
  
}
