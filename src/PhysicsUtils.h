#ifndef PHYSICS_UTILS_H_
#define PHYSICS_UTILS_H_

#include "Vector.h"
#include "Particle.h"

/////////////
//  TYPES  //
/////////////

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

void PhysicsUtils_updateParticalPairVelocities(Particle * p1, Particle * p2, double timeStep);
void PhysicsUtils_updateSingleParticalVelocity(Particle * p1, Particle * p2, double timeStep);
void PhysicsUtils_updateSingleParticalVelocityFast(Particle * p1, Vec3 * vec, double mass, double timeStep);
void PhysicsUtils_updateParticalPosition(Particle * p, double timeStep);

#endif
