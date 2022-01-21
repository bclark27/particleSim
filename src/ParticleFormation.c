#include "Common.h"

#include "ParticleFormation.h"
#include "UniversalConstants.h"
#include "PhysicsUtils.h"

/////////////
//  TYPES  //
/////////////


/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

void setParticleArraySize(ParticleFormation * pf, unsigned int size);
void generateOribitingPair(Particle * p1, Particle * p2, double distance, double m1, double m2);

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

ParticleFormation * ParticleFormation_init()
{
  ParticleFormation * pf = malloc(sizeof(ParticleFormation));
  memset(pf, 0, sizeof(ParticleFormation));

  return pf;
}

void ParticleFormation_free(ParticleFormation * pf)
{
  free(pf->particles);
  free(pf);
}

void ParticleFormation_cloudFormation(ParticleFormation * pf, unsigned int particleCount, Vec3 pos, Vec3 vel, double avgParticleMass, double avgDensity, double radius, double velRand, double massRand, double densityRand)
{
  if (particleCount == 0) return;

  setParticleArraySize(pf, particleCount);
  for (unsigned int i = 0; i < particleCount; i++)
  {
    Particle_init(&(pf->particles[i]));

    pf->particles[i].position.x = pos.x + (RAND_DOUBLE * radius);
    pf->particles[i].position.y = pos.y + (RAND_DOUBLE * radius);
    pf->particles[i].position.z = pos.z + (RAND_DOUBLE * radius);

    pf->particles[i].velocity.x = vel.x + (RAND_DOUBLE * velRand);
    pf->particles[i].velocity.y = vel.y + (RAND_DOUBLE * velRand);
    pf->particles[i].velocity.z = vel.z + (RAND_DOUBLE * velRand);

    Particle_setMass(&pf->particles[i], avgParticleMass + (RAND_DOUBLE * avgParticleMass * massRand));
    Particle_setDensity(&pf->particles[i], avgDensity + (RAND_DOUBLE * avgDensity * densityRand));
  }

}

void ParticleFormation_singularity(ParticleFormation * pf, double x, double y, double z, double mass)
{
  Particle p;
  Particle_init(&p);

  p.position.x = x;
  p.position.y = y;
  p.position.z = z;

  Particle_setMass(&p, mass);
  Particle_setDensity(&p, 500);

  setParticleArraySize(pf, 1);

  p.fixed = true;

  pf->particles[0] = p;
}

void ParticleFormation_singleParticle(ParticleFormation * pf, Vec3 pos, double mass, double density)
{
  Particle p;
  Particle_init(&p);

  p.position.x = pos.x;
  p.position.y = pos.y;
  p.position.z = pos.z;

  Particle_setMass(&p, mass);
  Particle_setDensity(&p, density);

  setParticleArraySize(pf, 1);

  p.fixed = false;

  pf->particles[0] = p;
}

void ParticleFormation_blackHole(ParticleFormation * pf, Vec3 pos, Vec3 vel, double mass)
{
  Particle p;
  Particle_init(&p);

  p.position.x = pos.x;
  p.position.y = pos.y;
  p.position.z = pos.z;

  Particle_setMass(&p, mass);
  double shwartzRadius = PhysicsUtils_calculateSchwarzschildRadius(mass);
  Particle_setDensity(&p, mass / ((4 * PI * shwartzRadius * shwartzRadius * shwartzRadius) / 3));

  setParticleArraySize(pf, 1);

  p.fixed = false;

  pf->particles[0] = p;
}

/////////////////////////
//  PRIVATE FUNCTIONS  //
/////////////////////////

void setParticleArraySize(ParticleFormation * pf, unsigned int size)
{
  if (pf->particles) free(pf->particles);
  pf->particles = malloc(sizeof(Particle) * size);
  pf->length = size;
}
