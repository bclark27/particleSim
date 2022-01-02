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

void PhysicsUtils_updateParticalPairVelocities(Particle * p1, Particle * p2, double timseStep);
void PhysicsUtils_updateParticalPosition(Particle * p, double timseStep);

#endif
