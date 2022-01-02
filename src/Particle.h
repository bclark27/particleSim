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
  Vector position;
  Vector velocity;

  //4 byte
  float mass;
  float density;
  float temp;

  //2 byte

  //1 byte
  bool fixed;
  bool inUse;

} Particle;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

void Particle_init(Particle * p);

#endif
