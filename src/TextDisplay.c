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

/////////////////////////
//  PRIVATE VARIABLES  //
/////////////////////////

//int textLevelsLen = 67;
//const char textLevels[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/|()1{}[]?-_+~i!lI;:,\"^`\". ";
int textLevelsLen = 11;
const char textLevels[] = " .:^/*%&$#@";
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

void clearText(TextDisplay * td)
{
  if (td->text == NULL) return;

  for (int i = 0; i < td->textLen; i++)
  {
    if (td->text[i] == NULL) return;
    memset(td->text[i], ' ', td->width);
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
      double value = 0;

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

          double mass = render->massBuffer[index];
          value += mass;
        }
      }

      char charOut = ' ';
      // value /= xScale * yScale;
      value = MAX(0, MIN(MAX_MASS - 1, value));
      value /= MAX_MASS;
      value = pow(value, 0.5);
      int level = value * textLevelsLen;// * ratio;
      charOut = textLevels[level];

      // if (y == 10 && x == 10) charOut = '&';
      td->text[y + yAsciiOffset][x + xAsciiOffset] = charOut;
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
      printf("%c", td->text[y][x]);
    }
    printf("\n");
  }
}
