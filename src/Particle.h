#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "Common.h"

#include "Vector.h"

/////////////
//  TYPES  //
/////////////

typedef struct Particle
{
  //8 byte
  Vec3 position;
  Vec3 velocity;
  Vec3 acceleration;
  Vec3 netAcceleration;
  double heatJoules;
  double heatJoulesDelta;
  double luminocity; // transparencyLevel(0 - 1) / depth(meters)

  //4 byte
  float mass;
  float density;
  float surfaceArea;
  float crossSectionalArea;
  float radius;
  float volume;
  float coolingConstant;
  float specificHeatCapacity;

  //2 byte

  //1 byte
  bool fixed;
  bool inUse;

} Particle;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

void Particle_init(Particle * p);
double Particle_getRadius(Particle * p);
void Particle_updatePosition(Particle * p, double timeStep);
void Particle_applyForceToNetAccel(Particle * p, Vec3 * direction, double newtons);
double Particle_getBrightness(Particle * p, double dist);
void Particle_setMass(Particle * p, double mass);
void Particle_setDensity(Particle * p, double density);
void Particle_resetDeltaStates(Particle * p);

#endif
