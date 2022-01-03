#include <stdio.h>
#include <stdlib.h>

#include "ParticleManager.h"
#include "Camera.h"
#include "TextDisplay.h"

////////////////////
//  PRIVATE VARS  //
////////////////////

//////////////////////
// PUBLIC FUNCTIONS //
//////////////////////

int main()
{
  ParticleManager * pm = ParticleManager_init();
  ParticleManager_setTimeStep(pm, 10);

  Camera * cam = Camera_init();
  Vec3 deltaPos = {0,0,0};
  Camera_moveCameraPosition(cam, &deltaPos);

  TextDisplay * td = TextDisplay_init();


  ParticleFormation * pf = ParticleFormation_init();
  ParticleFormation_cloudFormation(pf, 10000, 0, 0, -10, 1, 10, 0.1, 0.1, 0.1);
  ParticleManager_addFormation(pm, pf);
  ParticleFormation_free(pf);

  //Vector_printVec3(&((Particle64 *)(pm->pf->particle64List->head->data))->particles[0].position);
  //Vector_printVec3(&((Particle64 *)(pm->pf->particle64List->head->data))->particles[1].position);
  unsigned long int i = 0;
  unsigned int skip = 1;
  while(true)//for (int i = 0; i < 10000000; i++)
  {
    ParticleManager_updateParticles(pm);
    if (i % skip == 0)
    {
      printf("%li\n", i / skip);
      //TextDisplay_display(td, pm, cam);
    }
    i++;
  }

  TextDisplay_free(td);
  Camera_free(cam);
  ParticleManager_free(pm);
}
