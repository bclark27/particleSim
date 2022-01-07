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

void PhysicsUtils_updateParticalPairVelocities(Particle * p1, Particle * p2, double timeStep)
{

  double r = Vector_distance(&p1->position, &p2->position);
  double r2 = r * r;

  //printf("%lf\n", r);

  // F = G * (m1 * m2) / r^2
  // a = F / m

  double a1 = BIG_G * (p2->mass / r2);
  double a2 = BIG_G * (p1->mass / r2);

  Vector_difference(&unitVec1, &p2->position, &p1->position);

  Vector_normalize(&unitVec1);

  unitVec2.x = -unitVec1.x;
  unitVec2.y = -unitVec1.y;
  unitVec2.z = -unitVec1.z;

  Vector_scale(&unitVec1, a1 * timeStep);
  Vector_scale(&unitVec2, a2 * timeStep);

  Vector_add(&p1->velocity, &unitVec1);
  Vector_add(&p2->velocity, &unitVec2);

  //Vector_print(&p1->velocity);
}

void PhysicsUtils_updateParticalPosition(Particle * p)
{
  if (p->fixed) return;
  p->position.x += p->velocity.x;
  p->position.y += p->velocity.y;
  p->position.z += p->velocity.z;
  //printf("%lf\n", p->velocity.x);
}
