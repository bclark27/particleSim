#ifndef TEXT_DISPLAY_H_
#define TEXT_DISPLAY_H_

#include "Common.h"

#include "UniversalConstants.h"
#include "ParticleManager.h"
#include "Vector.h"
#include "Camera.h"
#include "Render.h"

///////////////
//  DEFINES  //
///////////////

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
  char ** text;
  char ** textColor;
  int textLen;

} TextDisplay;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

TextDisplay * TextDisplay_init(void);
void TextDisplay_free(TextDisplay * td);

void TextDisplay_display(TextDisplay * td, Render * render);
#endif
