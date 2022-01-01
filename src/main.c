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
  ParticleManager_addParticlesZeroized(pm, 10);
  ParticleManager_addParticlesZeroized(pm, 54);
  printf("%li\n", pm->particlesNotInUse);
  ParticleManager_free(pm);
}
