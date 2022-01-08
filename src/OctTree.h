#ifndef OCT_TREE_H_
#define OCT_TREE_H_

#include "Common.h"
#include "Vector.h"
#include "Particle.h"
#include "ParticleList.h"

/////////////
//  TYPES  //
/////////////

typedef struct OctTree
{
  struct OctTree * childA;
  struct OctTree * childB;
  struct OctTree * childC;
  struct OctTree * childD;
  struct OctTree * childE;
  struct OctTree * childF;
  struct OctTree * childG;
  struct OctTree * childH;

  Particle * p;

  Vec3 COM;
  Vec3 position;

  double sideLength;
  double totalMass;

  bool split;
} OctTree;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

OctTree * OctTree_init(Vec3 * pos, double sideLength);
void OctTree_free(OctTree* ot, bool destroyParticleData);

void OctTree_printParticles(OctTree * ot);
bool OctTree_insertParticle(OctTree * ot, Particle * p);
void OctTree_updateVelocitySingle(OctTree * ot, double theta, double timeStep, Particle * p);
void OctTree_updatePositions(OctTree * ot, double timeStep);

void OctTree_particleAreaStatsQueery(OctTree * ot, Vec3 * queeryOrigin, double queerySideLength, unsigned int * particleCount, double * totalMass, double * totalHeatJoules);
void OctTree_queeryParticlesInArea(OctTree * ot, ParticleList * pl,  Vec3 * queeryOrigin, double queerySideLength);

#endif
