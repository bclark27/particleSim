#ifndef PARTICLE_PORMATION_H_
#define PARTICLE_PORMATION_H_

#include "Common.h"
#include "List.h"
#include "Particle.h"

/////////////
//  TYPES  //
/////////////

typedef struct ParticleFormation
{
  Particle * particles;
  unsigned int length;
} ParticleFormation;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

ParticleFormation * ParticleFormation_init(void);
void ParticleFormation_free(ParticleFormation * pf);

//TODO: add funcs for basic formation creation
void ParticleFormation_appendParticles(ParticleFormation * dest, ParticleFormation * src);
void ParticleFormation_cloudFormation(ParticleFormation * pf, unsigned int particleCount, double x, double y, double z, double avgParticleMass, double avgDensity, double radius, double velRand, double massRand, double densityRand);
void ParticleFormation_orbit(ParticleFormation * pf, double distance, double x, double y, double z, double m1, double m2);
#endif
