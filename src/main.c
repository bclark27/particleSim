#include <stdio.h>
#include <stdlib.h>

#include "PhysicsUtils.h"
#include "ParticleManager.h"
#include "Particle.h"
#include "Camera.h"
#include "TextDisplay.h"
#include "OctTree.h"
#include "Render.h"

////////////////////
//  PRIVATE VARS  //
////////////////////

//////////////////////
// PUBLIC FUNCTIONS //
//////////////////////

int main()
{

  ParticleManager * pm = ParticleManager_init(10e20, 0.9);
  ParticleManager_setTimeStep(pm, 100000000);

  Camera * cam = Camera_init();
  Camera_setNearFarPlane(cam, 0.01, 10000000);

  TextDisplay * td = TextDisplay_init();
  Render * render = Render_init(1000);

  ParticleFormation * pf = ParticleFormation_init();

  int pcount = 3000;
  int countPerCloud = pcount / 3;
  Vec3 startPos = {-0, -0, 0};
  Vec3 startVel = {0, 0, 0};
  double cloudRadius = 400;
  double totalMass = 10e8;
  double massPerParticle = 1000;//totalMass / pcount;

  ParticleFormation_cloudFormation(pf, countPerCloud, startPos, startVel, massPerParticle, .4513, cloudRadius, 0, 0, 0);
  ParticleManager_addFormation(pm, pf);

  ParticleFormation_cloudFormation(pf, countPerCloud, startPos, startVel, massPerParticle, .5513, cloudRadius, 0, 0, 0);
  ParticleManager_addFormation(pm, pf);

  ParticleFormation_cloudFormation(pf, countPerCloud, startPos, startVel, massPerParticle, .3513, cloudRadius, 0, 0, 0);
  ParticleManager_addFormation(pm, pf);

  // neut star
  // ParticleFormation_singleParticle(pf, 10000, 0, 0, 10e29, 10e17);
  // ParticleFormation_singleParticle(pf, (Vec3){0, 0, 0}, 10e31, 10e17);
  // ParticleManager_addFormation(pm, pf);

  // point r=10m
  // ParticleFormation_singleParticle(pf, 0, 0, 0, 18550319, 7873);
  // ParticleManager_addFormation(pm, pf);

  // earth
  // ParticleFormation_singleParticle(pf, 0, 0, 0, 10e27, 100);
  // pf->particles[0].velocity.y = 200000;
  // ParticleManager_addFormation(pm, pf);

  ParticleFormation_free(pf);

  unsigned long int i = 0;
  unsigned int skip = 1;

  double dist = 1000;
  Vec3 camPos = {0, 0, dist};
  Camera_setPosition(cam, &camPos);
  Vec3 angles = {0, 0, 0};
  double theta = 0;
  double rotation = 0.0;

  while(true)//for (int i = 0; i < 10000000; i++)
  {
    ParticleManager_updateParticles(pm);

    if (i % skip == 0)
    {


      double x = sin(theta);
      double z = cos(theta);
      double angle = RAD_TO_DED(theta);//(theta) * 180 / PI;//(PI * 3 / 2) +

      angles.y = angle;
      camPos.z = z * dist;
      camPos.x = x * dist;
      camPos.y = x * 50;
      theta += rotation;
      Camera_setRotation(cam, &angles);
      Camera_setPosition(cam, &camPos);


      // Vector_printVec3(&pm->particles[0].position);
      Render_renderBuffers(render, pm, cam);
      TextDisplay_display(td, render);
    }
    i++;
  }

  Render_free(render);
  TextDisplay_free(td);
  Camera_free(cam);
  ParticleManager_free(pm);
}
