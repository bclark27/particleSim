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

int textLevelsLen = 67;
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


  ParticleManager_loopInit(pm);
  Particle * p = ParticleManager_loopNext(pm);
  Vec3 ans;
  int pixX;
  int pixY;

  clearPixels(td);

  while (p)
  {
    if (Camera_projectVec3Point(cam, &p->position, &ans))
    {
      //Vector_printVec3(&ans);
      pixX = (0.5 * (ans.x + 1)) * PIXEL_DIM;
      pixY = (1 - (ans.y + 1) * 0.5) * PIXEL_DIM;
      //printf("%i, %i\n", pixX, pixY);
      td->pixels[pixY][pixX] = 255;
    }
    p = ParticleManager_loopNext(pm);
  }

  clearText(td);
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

  td->height = td->w.ws_row;
  td->width = td->w.ws_col;

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
  int realX = td->width;
  int realY = td->height * 2;

  int squareSize = MIN(realX, realY);

  int xAsciiOffset = (realX - squareSize) / 2;
  int yAsciiOffset = (realY - squareSize) / 4;

  int xSideLen = squareSize;
  int ySideLen = squareSize / 2;

  //printf("%i, %i, %i\n", squareSize, xoffset, yoffset);

  int value;
  int xScale = PIXEL_DIM / squareSize;
  int yScale = xScale * 2;

  for (int y = 0; y < ySideLen; y++)//(int y = 0; y < (td->height - yoffset * 2); y++)
  {
    for (int x = 0; x < xSideLen; x++)//(int x = 0; x < (td->width - xoffset * 2); x++)
    {
      value = 0;

      for (int i = 0; i < xScale; i++)
      {
        for (int k = 0 ; k < yScale; k++)
        {
          int dx = i + x * xScale;
          int dy = k + y * yScale;

          value += td->pixels[dy][dx];
        }
      }

      value = MIN(255, value);

      td->text[y + yAsciiOffset][x + xAsciiOffset] = value;
    }
  }
}

void printText(TextDisplay * td)
{
  if (td->text == NULL) return;

  double ratio = (double)(textLevelsLen - 1) / 255;
  int realX = td->width;
  int realY = td->height * 2;

  int squareSize = MIN(realX, realY);

  int xAsciiOffset = (realX - squareSize) / 2;
  int yAsciiOffset = (realY - squareSize) / 4;

  int xSideLen = squareSize;
  int ySideLen = squareSize / 2;

  for (int y = 0; y < td->height; y++)
  {
    if (td->text[y] == NULL)
    {
      printf("\n");
      continue;
    }

    for (int x = 0; x < td->width; x++)
    {
      if (x >= xAsciiOffset && x < xAsciiOffset + xSideLen &&
          y >= yAsciiOffset && y < yAsciiOffset + ySideLen)
      {
        int level = (textLevelsLen - 1) - td->text[y][x] * ratio;
        char c = textLevels[level];
        printf("%c", c);
      }
      else
      {
        printf(" ");
      }
    }
    printf("\n");
  }
}
