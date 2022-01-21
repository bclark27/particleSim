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

void PhysicsUtils_applyGravitationalForcePair(Particle * p1, Particle * p2);
void PhysicsUtils_applyGravitationalForceSingle(Particle * p1, Particle * p2);
void PhysicsUtils_applyGravitationalForceSingleFast(Particle * p1, Vec3 * vec, double mass);
void PhysicsUtils_updateParticalPosition(Particle * p, double timeStep);
void PhysicsUtils_relitiveVelocities(Vec3 * aTob, Vec3 * bToa, Particle * a, Particle * b);
double PhysicsUtils_dragForce(double otherBodyDensity, double areaOfSelfContact, double selfRelVelocity);
void PhysicsUtils_applyForceToParticle(Particle * p, Vec3 * forceDirection, double newtons);
double PhysicsUtils_calculateEnergyLoss(Vec3 * velocityInitial, Vec3 * velocityFinal, double mass);
void PhysicsUtils_updateHeatEnergy(Particle * p, double timeStep);
double PhysicsUtils_calculateHeatDelta(double coolingConstant, double currentTemp, double surroundingTemp, double timeStep);
double PhysicsUtils_calculateRadiationGiveOff(Particle * p, double timeStep);
double PhysicsUtils_joulesToKelvin(double joules);
double PhysicsUtils_KelvinToJoules(double kelvin);
double PhysicsUtils_calculateHeatChange(double objectTemp, double surroundingTemp, double coolingConst, double timeStep);
double PhysicsUtils_calculateSchwarzschildRadius(double mass);
#endif
