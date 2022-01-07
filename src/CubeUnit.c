#include "CubeUnit.h"
#include "PhysicsUtils.h"

///////////////
//  DEFINES  //
///////////////

/////////////
//  TYPES  //
/////////////

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

void addVecToAllVelocities(CubeUnit * cu, Vec3 * velocity);

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

CubeUnit * CubeUnit_init()
{
  CubeUnit * cu = malloc(sizeof(CubeUnit));
  memset(cu, 0, sizeof(CubeUnit));

  cu->particleArray = ParticleArray_init();
  Vector_zeroize(&cu->centerOfMassPosition);
  cu->totalMass = 0;

  return cu;
}

void CubeUnit_free(CubeUnit * cu)
{
  ParticleArray_free(cu->particleArray);
  free(cu);
}

void CubeUnit_addParticle(CubeUnit * cu, Particle * p)
{
  cu->totalMass += p->mass;
  ParticleArray_addParticle(cu->particleArray, p);
}

void CubeUnit_updateSubParticleVelocities(CubeUnit * cu, double timestep)
{
  unsigned int pCount1 = 0;
  unsigned int pCount2;
  for (unsigned int i = 0; i < cu->particleArray->arraySize && pCount1 < cu->particleArray->elementCount; i++)
  {
    if (!cu->particleArray->particleArray[i].inUse) continue;
    pCount2 = 0;
    for (unsigned int k = 0; k < cu->particleArray->arraySize && pCount2 < cu->particleArray->elementCount; k++)
    {
      if (!cu->particleArray->particleArray[k].inUse) continue;
      PhysicsUtils_updateParticalPairVelocities(&cu->particleArray->particleArray[i], &cu->particleArray->particleArray[k], timestep);
      pCount2++;
    }
    pCount1++;
  }
}

void CubeUnit_updateCubeUnitVelocities(CubeUnit * cu1, CubeUnit * cu2, double timeStep)
{
  Particle p1;
  Particle p2;

  Particle_init(&p1);
  Particle_init(&p2);

  p1.mass = cu1->totalMass;
  p2.mass = cu2->totalMass;

  memcpy(&p1.position, &cu1->centerOfMassPosition, sizeof(Vec3));
  memcpy(&p2.position, &cu2->centerOfMassPosition, sizeof(Vec3));

  PhysicsUtils_updateParticalPairVelocities(&p1, & p2, timeStep);

  addVecToAllVelocities(cu1, &p1.velocity);
  addVecToAllVelocities(cu2, &p2.velocity);
}

void CUbeUnit_updatePatrticlePositions(CubeUnit * cu)
{
  unsigned int pCount = 0;
  for (unsigned int k = 0; k < cu->particleArray->arraySize && pCount < cu->particleArray->elementCount; k++)
  {
    if (!cu->particleArray->particleArray[k].inUse) continue;

    PhysicsUtils_updateParticalPosition(&cu->particleArray->particleArray[k]);

    if (!CubeUnit_particleIsInCubeUnit(cu, &cu->particleArray->particleArray[k]))
    {
      //cu->moveParticleFunc(&cu->particleArray->particleArray[k]);
      //TODO : tell the cube space that a particle needs to move somehow
      ParticleArray_removeAtIndex(cu->particleArray, k);
      cu->totalMass -= cu->particleArray->particleArray[k].mass;
    }

    pCount++;
  }
}


void CubeUnit_updateCenterOfMass(CubeUnit * cu)
{
  unsigned int pCount = 0;
  Vec3 centerPos;
  Vec3 temp;
  double mass;
  Vector_zeroize(&centerPos);
  cu->totalMass = 0;

  for (unsigned int k = 0; k < cu->particleArray->arraySize && pCount < cu->particleArray->elementCount; k++)
  {
    if (!cu->particleArray->particleArray[k].inUse) continue;
    //add v2 to v1
    mass = cu->particleArray->particleArray[k].mass;
    cu->totalMass += mass;

    temp = cu->particleArray->particleArray[k].position;
    centerPos.x += temp.x * mass;
    centerPos.y += temp.y * mass;
    centerPos.z += temp.z * mass;

    pCount++;
  }

  centerPos.x /= cu->totalMass;
  centerPos.y /= cu->totalMass;
  centerPos.z /= cu->totalMass;

  cu->centerOfMassPosition = centerPos;
}

bool CubeUnit_particleIsInCubeUnit(CubeUnit * cu, Particle * p)
{

}

////////////////////////
//  PRIVATE FUNCTIONS //
////////////////////////

void addVecToAllVelocities(CubeUnit * cu, Vec3 * velocity)
{
  unsigned int pCount = 0;
  for (unsigned int k = 0; k < cu->particleArray->arraySize && pCount < cu->particleArray->elementCount; k++)
  {
    if (!cu->particleArray->particleArray[k].inUse) continue;
    //add v2 to v1
    Vector_add(&cu->particleArray->particleArray[k].velocity, velocity);
    pCount++;
  }
}
