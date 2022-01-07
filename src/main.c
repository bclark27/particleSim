#include <stdio.h>
#include <stdlib.h>

#include "ParticleManager.h"
#include "Camera.h"
#include "TextDisplay.h"
#include "CubeUnit.h"

////////////////////
//  PRIVATE VARS  //
////////////////////

//////////////////////
// PUBLIC FUNCTIONS //
//////////////////////

int main()
{

  ParticleManager * pm = ParticleManager_init();
  ParticleManager_setTimeStep(pm, 0.5);

  Camera * cam = Camera_init();
  Vec3 deltaPos = {0, 0, 40};
  Camera_moveCameraPosition(cam, &deltaPos);
  Vec3 angles = {0, 0, 0};
  //Camera_setRotation(cam, &angles);
  Camera_setNearFarPlane(cam, 0.01, 1000);

  TextDisplay * td = TextDisplay_init();


  ParticleFormation * pf = ParticleFormation_init();
  ParticleFormation_cloudFormation(pf, 10, 0, 0, 0, 1000, 500, 25, 0, 0.3, 0.3);
  //ParticleFormation_orbit(pf, 20, 0, 0, 0, 500, 500);
  ParticleManager_addFormation(pm, pf);
  ParticleFormation_free(pf);

  //Vector_printVec3(&((Particle64 *)(pm->pf->particle64List->head->data))->particles[0].position);
  //Vector_printVec3(&((Particle64 *)(pm->pf->particle64List->head->data))->particles[1].position);
  unsigned long int i = 0;
  unsigned int skip = 100;

  double theta = 0;
  double dist = 70;
  double rotation = 0.000;

  while(true)//for (int i = 0; i < 10000000; i++)
  {
    ParticleManager_updateParticles(pm);
    if (i % skip == 0)
    {
      //printf("%li\n", i / skip);
      double x = sin(theta);
      double z = cos(theta);
      double angle = RAD_TO_DED(theta);//(theta) * 180 / PI;//(PI * 3 / 2) +

      angles.y = angle;
      deltaPos.z = z * dist;
      deltaPos.x = x * dist;
      theta += rotation;

      Camera_setPosition(cam, &deltaPos);
      Camera_setRotation(cam, &angles);
      TextDisplay_display(td, pm, cam);
    }
    i++;
  }

  TextDisplay_free(td);
  Camera_free(cam);
  ParticleManager_free(pm);
}
