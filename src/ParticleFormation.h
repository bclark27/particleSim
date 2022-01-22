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
void ParticleFormation_cloudFormation(ParticleFormation * pf, unsigned int particleCount, Vec3 pos, Vec3 vel, double avgParticleMass, double avgDensity, double radius, double velRand, double massRand, double densityRand, double heatInit);
void ParticleFormation_singularity(ParticleFormation * pf, double x, double y, double z, double mass);
void ParticleFormation_singleParticle(ParticleFormation * pf, Vec3 pos, double mass, double density);
void ParticleFormation_blackHole(ParticleFormation * pf, Vec3 pos, Vec3 vel, double mass);

#endif
