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

void updateTextWindowSize(TextDisplay * td);
void freeText(TextDisplay * td);
void clearPixels(TextDisplay * td);
void clearText(TextDisplay * td);
void pixelsToText(TextDisplay * td);
void printText(TextDisplay * td);

/////////////////////////
//  PRIVATE VARIABLES  //
/////////////////////////

int textLevelsLen = 70;
const char textLevels[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/|()1{}[]?-_+~i!lI;:,\"^`\". ";

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

TextDisplay * TextDisplay_init(void)
{
  TextDisplay * td = malloc(sizeof(TextDisplay));
  memset(td, 0, sizeof(TextDisplay));

  ioctl(STDOUT_FILENO, TIOCGWINSZ, &(td->w));
  td->width = td->w.ws_col;
  td->height = td->w.ws_row;
  td->text = NULL;
  td->textLen = 0;

  updateTextWindowSize(td);

  return td;
}

void TextDisplay_free(TextDisplay * td)
{
  if (td->text != NULL)
  {

  }

  free(td);
}

void TextDisplay_display(TextDisplay * td, ParticleManager * pm, Camera * cam)
{
  updateTextWindowSize(td);
  clearPixels(td);

  ParticleManager_loopInit(pm);
  Particle * p = ParticleManager_loopNext(pm);
  Vec3 ans;
  int pixX;
  int pixY;

  while (p)
  {
    if (Camera_projectVec3Point(cam, &p->position, &ans))
    {
      //Vector_printVec3(&ans);
      pixX = (0.5 * (ans.x + 1)) * PIXEL_DIM;
      pixY = (1 - (ans.y + 1) * 0.5) * PIXEL_DIM;

      td->pixels[pixY][pixX] = 255;
    }
    p = ParticleManager_loopNext(pm);
  }

  pixelsToText(td);
  printText(td);
}

/////////////////////////
//  PRIVATE FUNCTIONS  //
/////////////////////////

void updateTextWindowSize(TextDisplay * td)
{
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &(td->w));
  if (td->width == td->w.ws_col && td->height == td->w.ws_row && td->text != NULL) return;

  freeText(td);

  if (td->text == NULL)
  {
    td->text = malloc(sizeof(unsigned char *) * td->height);
    td->textLen = td->height;

    for (int i = 0; i < td->textLen; i++)
    {
      td->text[i] = malloc(td->width);
    }
  }
}

void freeText(TextDisplay * td)
{
  if (td->text == NULL) return;
  for (int i = 0; i < td->textLen; i++)
  {
    if (td->text[i] != NULL) free(td->text[i]);
  }

  free(td->text);
  td->text = NULL;
}

void clearPixels(TextDisplay * td)
{
  memset(&td->pixels, 0, PIXEL_DIM * PIXEL_DIM);
}

void clearText(TextDisplay * td)
{
  if (td->text == NULL) return;

  for (int i = 0; i < td->textLen; i++)
  {
    if (td->text[i] == NULL) return;
    memset(td->text[i], 0, td->width);
  }
}

void pixelsToText(TextDisplay * td)
{

}

void printText(TextDisplay * td)
{
  if (td->text == NULL) return;

  for (int i = 0; i < td->textLen; i++)
  {
    if (td->text[i] == NULL)
    {
      printf("\n");
      continue;
    }

    for (int k = 0; k < td->width; k++)
    {
      int level = (255 - td->text[i][k]) * (((double) textLevelsLen - 1) / 255);
      printf("%c", textLevels[level]);
    }
    printf("\n");
  }
}
