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

  //4 byte
  float mass;
  float density;
  float heatJoules;

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

#endif
