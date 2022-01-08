#include "Common.h"

#include "ParticleFormation.h"
#include "UniversalConstants.h"

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

void ParticleFormation_cloudFormation(ParticleFormation * pf, unsigned int particleCount, double x, double y, double z, double avgParticleMass, double avgDensity, double radius, double velRand, double massRand, double densityRand)
{
  if (particleCount == 0) return;

  setParticleArraySize(pf, particleCount);
  for (unsigned int i = 0; i < particleCount; i++)
  {
    Particle_init(&(pf->particles[i]));

    pf->particles[i].position.x = x + (RAND_DOUBLE * radius);
    pf->particles[i].position.y = y + (RAND_DOUBLE * radius);
    pf->particles[i].position.z = z + (RAND_DOUBLE * radius);

    pf->particles[i].velocity.x = x + (RAND_DOUBLE * velRand);
    pf->particles[i].velocity.y = y + (RAND_DOUBLE * velRand);
    pf->particles[i].velocity.z = z + (RAND_DOUBLE * velRand);

    pf->particles[i].mass = avgParticleMass + (RAND_DOUBLE * avgParticleMass * massRand);

    pf->particles[i].density = avgDensity + (RAND_DOUBLE * avgDensity * densityRand);

  }
}

void ParticleFormation_orbit(ParticleFormation * pf, double distance, double x, double y, double z, double m1, double m2)
{
  Particle p1;
  Particle p2;

  setParticleArraySize(pf, 2);
  generateOribitingPair(&p1, &p2, distance, m1, m2);
  pf->particles[0] = p1;
  pf->particles[1] = p2;
}

void ParticleFormation_singularity(ParticleFormation * pf, double x, double y, double z, double mass)
{
  Particle p;
  Particle_init(&p);

  p.position.x = x;
  p.position.y = y;
  p.position.z = z;

  p.mass = mass;

  p.density *= 200;

  setParticleArraySize(pf, 1);

  p.fixed = true;

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

void generateOribitingPair(Particle * p1, Particle * p2, double distance, double m1, double m2)
{
  double baryCenter1 = distance / (1 + (m1 / m2));
  double baryCenter2 = distance / (1 + (m2 / m1));

  Particle_init(p1);
  Particle_init(p2);

  p1->mass = m1;
  p2->mass = m2;

  p1->position.x = -baryCenter1;
  p2->position.x = baryCenter2;

  p1->velocity.z = -(BIG_G * m2 / (distance * distance));
  p1->velocity.z = (BIG_G * m1 / (distance * distance));
}
