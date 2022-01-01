#ifndef PARTICLE_TEMPT_H_
#define PARTICLE_TEMPT_H_

#include "Common.h"

#include "Vector.h"

/////////////
//  TYPES  //
/////////////

typedef struct ParticleTemp
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

} ParticleTemp;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

void ParticleTemp_init(ParticleTemp * p);

#endif
