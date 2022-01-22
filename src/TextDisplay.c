#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TextDisplay.h"
#include "Render.h"

///////////////
//  DEFINES  //
///////////////

#define MAX_MASS 1000

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
void rendererToText(TextDisplay * td, Render * render);
void printStats(TextDisplay * td, ParticleManager * pm);
void printText(TextDisplay * td);
void changeColor(unsigned char val);

/////////////////////////
//  PRIVATE VARIABLES  //
/////////////////////////

// int textLevelsLen = 67;
// const char textLevels[] = " .\"`^\",:;Il!i~+_-?][}{1)(|/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";

int textLevelsLen = 11;
const char textLevels[] = " .:^/*%&$#@";


char * colors[8] = {
  "\033[0;30m",
  "\033[0;31m",
  "\033[0;32m",
  "\033[0;33m",
  "\033[0;34m",
  "\033[0;35m",
  "\033[0;36m",
  "\033[0;37m",
};


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
  freeText(td);
  free(td);
}

void TextDisplay_display(TextDisplay * td, Render * render)
{
  updateTextWindowSize(td);
  clearText(td);
  rendererToText(td, render);
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
    td->textColor = malloc(sizeof(unsigned char *) * td->height);
    td->textLen = td->height;

    for (int i = 0; i < td->textLen; i++)
    {
      td->text[i] = malloc(td->width);
      td->textColor[i] = malloc(td->width);
    }
  }
}

void freeText(TextDisplay * td)
{
  if (td->text == NULL) return;
  if (td->textColor == NULL) return;
  for (int i = 0; i < td->textLen; i++)
  {
    if (td->text[i] != NULL) free(td->text[i]);
    if (td->textColor[i] != NULL) free(td->textColor[i]);
  }

  free(td->text);
  free(td->textColor);
  td->text = NULL;
  td->textColor = NULL;
}

void clearText(TextDisplay * td)
{
  if (td->text == NULL) return;
  if (td->textColor == NULL) return;

  for (int i = 0; i < td->textLen; i++)
  {
    if (td->text[i] == NULL) return;
    memset(td->text[i], ' ', td->width);

    if (td->textColor[i] == NULL) return;
    memset(td->textColor[i], ' ', td->width);
  }
}

void rendererToText(TextDisplay * td, Render * render)
{
  int realX = td->width;
  int realY = td->height * 2;

  int squareSize = MIN(realX, realY);

  int xAsciiOffset = (realX - squareSize) / 2;
  int yAsciiOffset = (realY - squareSize) / 4;

  int xSideLen = squareSize;
  int ySideLen = squareSize / 2;

  //printf("%i, %i, %i\n", squareSize, xoffset, yoffset);

  int xScale = render->size / squareSize;
  int yScale = xScale * 2;

  double ratio = (double)(textLevelsLen - 1) / 255;

  for (int y = 0; y < ySideLen; y++)//(int y = 0; y < (td->height - yoffset * 2); y++)
  {
    for (int x = 0; x < xSideLen; x++)//(int x = 0; x < (td->width - xoffset * 2); x++)
    {
      double massVal = 0;
      double colorVal = 0;
      for (int i = 0; i < xScale; i++)
      {
        for (int k = 0; k < yScale; k++)
        {
          // int dx = i + x * xScale;
          // int dy = k + y * yScale;
          // int index = dy * xSideLen + dx;
          int pixY = y * yScale + k;
          int pixX = x * xScale + i;

          int index = pixX + render->size * pixY;

          massVal += render->massBuffer[index];
          colorVal += render->brightnessBuffer[index];

        }
      }

      // if (value > 0)
      // {
      //   printf("%lf\n", value);
      //   exit(1);
      // }
      char charOut = ' ';
      massVal /= xScale * yScale;
      massVal = MAX(0, MIN(MAX_MASS - 1, massVal));
      massVal /= MAX_MASS;
      massVal = pow(massVal, 0.5);
      int level = massVal * textLevelsLen;// * ratio;
      charOut = textLevels[level];

      td->text[y + yAsciiOffset][x + xAsciiOffset] = charOut;


      colorVal /= xScale * yScale;
      colorVal *= 255;
      td->textColor[y + yAsciiOffset][x + xAsciiOffset] = colorVal;
    }
  }
}

void printStats(TextDisplay * td, ParticleManager * pm)
{

}

void printText(TextDisplay * td)
{
  if (td->text == NULL) return;

  for (int y = 0; y < td->height; y++)
  {
    if (td->text[y] == NULL)
    {
      printf("\n");
      continue;
    }

    for (int x = 0; x < td->width; x++)
    {
      changeColor(td->textColor[y][x]);
      printf("%c", td->text[y][x]);
    }
    printf("\n");
  }
}


/////////////////////////
//  PRIVATE FUNCTIONS  //
/////////////////////////

void changeColor(unsigned char val)
{
  double color = val / 255.0;
  color *= 7;
  char * strColor = colors[(int)color];
  printf("%s", strColor);
}
