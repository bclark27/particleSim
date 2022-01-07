#ifndef CUBE_UNIT_H_
#define CUBE_UNIT_H_

#include "Common.h"
#include "ParticleArray.h"
#include "Particle.h"

///////////////
//  DEFINES  //
///////////////

#define CUBE_SIZE_LENGTH_METERS

/////////////
//  TYPES  //
/////////////

typedef struct CubeUnit
{
  Vec3 centerOfMassPosition;
  double totalMass;
  ParticleArray * particleArray;
} CubeUnit;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

CubeUnit * CubeUnit_init();
void CubeUnit_free(CubeUnit * cu);

void CubeUnit_addParticle(CubeUnit * cu, Particle * p);
void CubeUnit_updateSubParticleVelocities(CubeUnit * cu, double timestep);
void CubeUnit_updateCubeUnitVelocities(CubeUnit * cu1, CubeUnit * cu2, double timeStep);
void CUbeUnit_updatePatrticlePositions(CubeUnit * cu);
void CubeUnit_updateCenterOfMass(CubeUnit * cu);
bool CubeUnit_particleIsInCubeUnit(CubeUnit * cu, Particle * p);

#endif
