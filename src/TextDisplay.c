#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TextDisplay.h"

/////////////
//  TYPES  //
/////////////

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

TextDisplay * TextDisplay_init(void)
{
  TextDisplay * td = malloc(sizeof(TextDisplay));
  memset(td, 0, sizeof(TextDisplay));

  ioctl(STDOUT_FILENO, TIOCGWINSZ, &(td->w));
  td->unitWidth = 1;
  td->unitHeight = 1;
}

void TextDisplay_free(TextDisplay * td)
{
  free(td);
}

void TextDisplay_display(ParticleManager * pm)
{

}
