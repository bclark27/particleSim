#include <stdio.h>
#include <stdlib.h>

#include "ParticleManager.h"

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

  ParticleFormation * pf = ParticleFormation_init();
  ParticleFormation_cloudFormation(pf, 3, 0, 0, 0, 10, 100, 0.1, 0.1, 0.1);
  ParticleManager_addFormation(pm, pf);
  ParticleFormation_free(pf);

  Vector_print(&((Particle64 *)(pm->pf->particle64List->head->data))->particles[0].position);
  Vector_print(&((Particle64 *)(pm->pf->particle64List->head->data))->particles[1].position);
  unsigned long int i = 0;
  unsigned int skip = 100000;
  while(true)//for (int i = 0; i < 10000000; i++)
  {
    ParticleManager_updateParticles(pm);
    if (i % skip == 0)
    {
      printf("%li | ", i / skip);
      Vector_print(&((Particle64 *)(pm->pf->particle64List->head->data))->particles[0].position);
    }
    i++;
  }

  ParticleManager_free(pm);
}
