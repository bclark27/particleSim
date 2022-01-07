#ifndef TEXT_DISPLAY_H_
#define TEXT_DISPLAY_H_

#include "Common.h"

#include "UniversalConstants.h"
#include "ParticleManager.h"
#include "Vector.h"
#include "Camera.h"

///////////////
//  DEFINES  //
///////////////

#define PIXEL_DIM 1000

/////////////
//  TYPES  //
/////////////

typedef struct TextDisplay
{
  //row = w.ws_row
  //col = w.ws_col
  struct winsize w;

  int width;
  int height;
  unsigned char pixels[PIXEL_DIM][PIXEL_DIM];
  double zbuffer[PIXEL_DIM][PIXEL_DIM];
  unsigned char ** text;
  int textLen;

} TextDisplay;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

TextDisplay * TextDisplay_init(void);
void TextDisplay_free(TextDisplay * td);

void TextDisplay_display(TextDisplay * td, ParticleManager * pm, Camera * cam);
#endif
