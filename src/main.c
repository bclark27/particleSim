#include <stdio.h>
#include <stdlib.h>

#include "ParticleManager.h"
#include "Particle.h"
#include "Camera.h"
#include "TextDisplay.h"
#include "OctTree.h"

////////////////////
//  PRIVATE VARS  //
////////////////////

//////////////////////
// PUBLIC FUNCTIONS //
//////////////////////

int main()
{
  ParticleManager * pm = ParticleManager_init(10000, 0.9);
  ParticleManager_setTimeStep(pm, 1);

  Camera * cam = Camera_init();
  Vec3 camPos = {0, 0, 200};
  Camera_moveCameraPosition(cam, &camPos);
  Vec3 angles = {0, 0, 0};
  //Camera_setRotation(cam, &angles);
  Camera_setNearFarPlane(cam, 0.01, 1000);

  TextDisplay * td = TextDisplay_init();


  ParticleFormation * pf = ParticleFormation_init();

  ParticleFormation_cloudFormation(pf, 1000, 0, 0, 0, 100, 500, 100, 0.0, 0, 0);
  ParticleManager_addFormation(pm, pf);

  // ParticleFormation_singularity(pf, 0, 0, 0, 100000);
  // ParticleManager_addFormation(pm, pf);

  ParticleFormation_free(pf);

  //Vector_printVec3(&((Particle64 *)(pm->pf->particle64List->head->data))->particles[0].position);
  //Vector_printVec3(&((Particle64 *)(pm->pf->particle64List->head->data))->particles[1].position);
  unsigned long int i = 0;
  unsigned int skip = 1;

  double theta = 0;
  double dist = 20;
  double rotation = 0.000;

  while(true)//for (int i = 0; i < 10000000; i++)
  {
    ParticleManager_updateParticles(pm);
    if (i % skip == 0)
    {
      /*
      double x = sin(theta);
      double z = cos(theta);
      double angle = RAD_TO_DED(theta);//(theta) * 180 / PI;//(PI * 3 / 2) +

      angles.y = angle;
      deltaPos.z = z * dist;
      deltaPos.x = x * dist;
      theta += rotation;
      Camera_setRotation(cam, &angles);
      */
      // camPos = pm->COM;
      // camPos.z += dist;
      // Camera_setPosition(cam, &camPos);
      //Vector_printVec3(&camPos);

      TextDisplay_display(td, pm, cam);
    }
    i++;
  }

  TextDisplay_free(td);
  Camera_free(cam);
  ParticleManager_free(pm);
}
