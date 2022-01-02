#ifndef TEXT_DISPLAY_H_
#define TEXT_DISPLAY_H_

#include "Common.h"

#include "UniversalConstants.h"
#include "ParticleManager.h"
#include "Vector.h"

/////////////
//  TYPES  //
/////////////

typedef struct TextDisplay
{
  struct winsize w;
  double unitWidth;
  double unitHeight;

  Vector position;
  Vector facing;
  Vector pojectionPlane[2];
  //row = w.ws_row
  //col = w.ws_col
} TextDisplay;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

TextDisplay * TextDisplay_init(void);
void TextDisplay_free(TextDisplay * td);

void TextDisplay_display(ParticleManager * pm);
#endif
