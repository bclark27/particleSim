#include "OctTree.h"

#include "Particle.h"
#include "PhysicsUtils.h"

/////////////
//  TYPES  //
/////////////

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

OctTree * initNode(double x, double y, double z, double sideLength);
bool contains(OctTree * ot, Vec3 pos);
void splitNode(OctTree * ot);
void updateCOMWithChildren(OctTree * ot);
void updateHelperMaster(OctTree * otHead, OctTree * otThis, double theta, double timeStep);
void updateWalkHelper(OctTree * ot, double theta, double timeStep, Particle * p);
bool boxesIntersect(Vec3 * v1, double len1, Vec3 * v2, double len2);

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

OctTree * OctTree_init(Vec3 * pos, double sideLength)
{
  Vec3 posAjusted = *pos;
  Vec3 temp = {-sideLength / 2, -sideLength / 2, -sideLength / 2};
  Vector_add(&posAjusted, &temp);
  return initNode(posAjusted.x, posAjusted.y, posAjusted.z, sideLength);
}

void OctTree_free(OctTree* ot, bool destroyParticleData)
{
  if (ot->split)
  {
    //kill children
    OctTree_free(ot->childA, destroyParticleData);
    OctTree_free(ot->childB, destroyParticleData);
    OctTree_free(ot->childC, destroyParticleData);
    OctTree_free(ot->childD, destroyParticleData);
    OctTree_free(ot->childE, destroyParticleData);
    OctTree_free(ot->childF, destroyParticleData);
    OctTree_free(ot->childG, destroyParticleData);
    OctTree_free(ot->childH, destroyParticleData);
  }

  if (destroyParticleData && ot->p != NULL)
  {
    free(ot->p);
  }

  free(ot);
}

void OctTree_printParticles(OctTree * ot)
{
  if (ot->p)
  {
    printf("x: %lf, y: %lf, z: %lf\n", ot->p->position.x, ot->p->position.y, ot->p->position.z);
  }

  if (ot->split)
  {
    OctTree_printParticles(ot->childA);
    OctTree_printParticles(ot->childB);
    OctTree_printParticles(ot->childC);
    OctTree_printParticles(ot->childD);
    OctTree_printParticles(ot->childE);
    OctTree_printParticles(ot->childF);
    OctTree_printParticles(ot->childG);
    OctTree_printParticles(ot->childH);
  }
}

bool OctTree_insertParticle(OctTree * ot, Particle * p)
{
  if (!contains(ot, p->position)) return false;

  //if its not split then this cell is empty and can take a particle
  if (!ot->split && ot->p == NULL)
  {
    ot->p = p;
    ot->totalMass = p->mass;
    ot->COM = p->position;
    return true;
  }
  //if the cell is full htne split the cell and insert new particle **AND RE INSERT THE OLD ONE**
  else
  {
    if (!ot->split) splitNode(ot);

    //insert new node
    switch (1)
    {
      case 1: { if (OctTree_insertParticle(ot->childA, p)) break; }
      case 2: { if (OctTree_insertParticle(ot->childB, p)) break; }
      case 3: { if (OctTree_insertParticle(ot->childC, p)) break; }
      case 4: { if (OctTree_insertParticle(ot->childD, p)) break; }
      case 5: { if (OctTree_insertParticle(ot->childE, p)) break; }
      case 6: { if (OctTree_insertParticle(ot->childF, p)) break; }
      case 7: { if (OctTree_insertParticle(ot->childG, p)) break; }
      case 8: { if (OctTree_insertParticle(ot->childH, p)) break; }
    }

    //insert the existing node
    if (ot->p != NULL)
    {
      switch (1)
      {
        case 1: { if (OctTree_insertParticle(ot->childA, ot->p)) break; }
        case 2: { if (OctTree_insertParticle(ot->childB, ot->p)) break; }
        case 3: { if (OctTree_insertParticle(ot->childC, ot->p)) break; }
        case 4: { if (OctTree_insertParticle(ot->childD, ot->p)) break; }
        case 5: { if (OctTree_insertParticle(ot->childE, ot->p)) break; }
        case 6: { if (OctTree_insertParticle(ot->childF, ot->p)) break; }
        case 7: { if (OctTree_insertParticle(ot->childG, ot->p)) break; }
        case 8: { if (OctTree_insertParticle(ot->childH, ot->p)) break; }
      }
    }

    ot->p = NULL;

    ot->totalMass += p->mass;
    updateCOMWithChildren(ot);

    return true;
  }
}

void OctTree_updateVelocitySingle(OctTree * ot, double theta, double timeStep, Particle * p)
{
  if (ot->split && ot->p == NULL)
  {
    updateWalkHelper(ot->childA, theta, timeStep, p);
    updateWalkHelper(ot->childB, theta, timeStep, p);
    updateWalkHelper(ot->childC, theta, timeStep, p);
    updateWalkHelper(ot->childD, theta, timeStep, p);
    updateWalkHelper(ot->childE, theta, timeStep, p);
    updateWalkHelper(ot->childF, theta, timeStep, p);
    updateWalkHelper(ot->childG, theta, timeStep, p);
    updateWalkHelper(ot->childH, theta, timeStep, p);
  }

  if (ot->p != NULL && ot->p != p)
  {
    PhysicsUtils_updateSingleParticalVelocity(p, ot->p, timeStep);
  }
}

void OctTree_particleAreaStatsQueery(OctTree * ot, Vec3 * queeryOrigin, double queerySideLength, unsigned int * particleCount, double * totalMass, double * totalHeatJoules)
{
  if (!boxesIntersect(queeryOrigin, queerySideLength, &ot->position, ot->sideLength)) return;

  if (ot->split)
  {
    OctTree_particleAreaStatsQueery(ot->childA, queeryOrigin, queerySideLength, particleCount, totalMass, totalHeatJoules);
    OctTree_particleAreaStatsQueery(ot->childB, queeryOrigin, queerySideLength, particleCount, totalMass, totalHeatJoules);
    OctTree_particleAreaStatsQueery(ot->childC, queeryOrigin, queerySideLength, particleCount, totalMass, totalHeatJoules);
    OctTree_particleAreaStatsQueery(ot->childD, queeryOrigin, queerySideLength, particleCount, totalMass, totalHeatJoules);
    OctTree_particleAreaStatsQueery(ot->childE, queeryOrigin, queerySideLength, particleCount, totalMass, totalHeatJoules);
    OctTree_particleAreaStatsQueery(ot->childF, queeryOrigin, queerySideLength, particleCount, totalMass, totalHeatJoules);
    OctTree_particleAreaStatsQueery(ot->childG, queeryOrigin, queerySideLength, particleCount, totalMass, totalHeatJoules);
    OctTree_particleAreaStatsQueery(ot->childH, queeryOrigin, queerySideLength, particleCount, totalMass, totalHeatJoules);
    return;
  }

  if (ot->p != NULL && contains(ot, ot->p->position))
  {
    *particleCount += 1;
    *totalMass += ot->p->mass;
    *totalHeatJoules += ot->p->heatJoules;
  }
}

void OctTree_queeryParticlesInArea(OctTree * ot, ParticleList * pl,  Vec3 * queeryOrigin, double queerySideLength)
{
  if (!boxesIntersect(queeryOrigin, queerySideLength, &ot->position, ot->sideLength)) return;

  if (ot->split)
  {
    OctTree_queeryParticlesInArea(ot->childA, pl, queeryOrigin, queerySideLength);
    OctTree_queeryParticlesInArea(ot->childB, pl, queeryOrigin, queerySideLength);
    OctTree_queeryParticlesInArea(ot->childC, pl, queeryOrigin, queerySideLength);
    OctTree_queeryParticlesInArea(ot->childD, pl, queeryOrigin, queerySideLength);
    OctTree_queeryParticlesInArea(ot->childE, pl, queeryOrigin, queerySideLength);
    OctTree_queeryParticlesInArea(ot->childF, pl, queeryOrigin, queerySideLength);
    OctTree_queeryParticlesInArea(ot->childG, pl, queeryOrigin, queerySideLength);
    OctTree_queeryParticlesInArea(ot->childH, pl, queeryOrigin, queerySideLength);
    return;
  }

  if (ot->p != NULL && contains(ot, ot->p->position))
  {
    ParticleList_append(pl, ot->p);
  }
}

////////////////////////
//  PRIVATE FUNCTIONS //
////////////////////////

OctTree * initNode(double x, double y, double z, double sideLength)
{
  OctTree * ot = calloc(1, sizeof(OctTree));
  Vector_zeroize(&ot->COM);
  Vec3 pos = {x, y, z};
  memcpy(&ot->position, &pos, sizeof(Vec3));
  ot->sideLength = sideLength;
  ot->totalMass = 0;
  ot->split = false;

  return ot;
}

bool contains(OctTree * ot, Vec3 pos)
{
  return pos.x >= ot->position.x &&
        pos.y >= ot->position.y &&
        pos.z >= ot->position.z &&
        pos.x < ot->position.x + ot->sideLength &&
        pos.y < ot->position.y + ot->sideLength &&
        pos.z < ot->position.z + ot->sideLength;
}

void splitNode(OctTree * ot)
{
  ot->split = true;

  double len = ot->sideLength / 2;
  ot->childA = initNode(ot->position.x, ot->position.y, ot->position.z, len);
  ot->childB = initNode(ot->position.x, ot->position.y, ot->position.z + len, len);
  ot->childC = initNode(ot->position.x, ot->position.y + len, ot->position.z, len);
  ot->childD = initNode(ot->position.x, ot->position.y + len, ot->position.z + len, len);
  ot->childE = initNode(ot->position.x + len, ot->position.y, ot->position.z, len);
  ot->childF = initNode(ot->position.x + len, ot->position.y, ot->position.z + len, len);
  ot->childG = initNode(ot->position.x + len, ot->position.y + len, ot->position.z, len);
  ot->childH = initNode(ot->position.x + len, ot->position.y + len, ot->position.z + len, len);
}

void updateCOMWithChildren(OctTree * ot)
{
      Vec3 com;
      Vec3 temp;
      Vector_zeroize(&com);

      temp = ot->childA->COM;
      Vector_scale(&temp, ot->childA->totalMass);
      Vector_add(&com, &temp);

      temp = ot->childB->COM;
      Vector_scale(&temp, ot->childB->totalMass);
      Vector_add(&com, &temp);

      temp = ot->childC->COM;
      Vector_scale(&temp, ot->childC->totalMass);
      Vector_add(&com, &temp);

      temp = ot->childD->COM;
      Vector_scale(&temp, ot->childD->totalMass);
      Vector_add(&com, &temp);

      temp = ot->childE->COM;
      Vector_scale(&temp, ot->childE->totalMass);
      Vector_add(&com, &temp);

      temp = ot->childF->COM;
      Vector_scale(&temp, ot->childF->totalMass);
      Vector_add(&com, &temp);

      temp = ot->childG->COM;
      Vector_scale(&temp, ot->childG->totalMass);
      Vector_add(&com, &temp);

      temp = ot->childH->COM;
      Vector_scale(&temp, ot->childH->totalMass);
      Vector_add(&com, &temp);

      Vector_scale(&com, 1 / ot->totalMass);

      ot->COM = com;
}

void updateWalkHelper(OctTree * ot, double theta, double timeStep, Particle * p)
{
  //if it is an empty cell just return
  if (!ot->split && ot->p == NULL) return;

  double dist = Vector_distance(&p->position, &ot->COM);
  if (theta > ot->sideLength / dist)
  {
    PhysicsUtils_updateSingleParticalVelocityFast(p, &ot->COM, ot->totalMass, timeStep);
  }
  else
  {
    OctTree_updateVelocitySingle(ot, theta, timeStep, p);
  }
}

bool boxesIntersect(Vec3 * v1, double len1, Vec3 * v2, double len2)
{
   return !(v1->x        > v2->x + len2 ||
            v1->x + len1 < v2->x        ||

            v1->y        > v2->y + len2 ||
            v1->y + len1 < v2->y        ||

            v1->z        > v2->z + len2 ||
            v1->z + len1 < v2->z        );
}
