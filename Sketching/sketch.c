#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "display.h"

enum { DX = 0, DY = 1, DT = 2, PEN = 3, CLEAR = 4, KEY = 5, COLOUR = 6 };

struct state {int x; int y; int prevX; int prevY; bool pen; int rgba;};
typedef struct state state;

// TODO: upgrade the run function, adding functions to support it.

unsigned char unpackOpcode(unsigned char byte, unsigned char opcode)
{
  opcode = (byte >> 6);

  if (opcode == 3)
  {
    opcode = (byte & 0x0F);
    return opcode;
  }
  return opcode;
}

int convertToNegative(int opcode, int operand, int extraOperandBytes)
 {
  if ((opcode != 2) && (opcode != 6))
  {
    if ((operand > 127) && (extraOperandBytes == 1))
    {
      operand = operand - 256;
      return operand;
    }
    else if ((operand > 32767) && (extraOperandBytes == 2))
    {
      operand = operand - 65536;
      return operand;
    }
    else if ((operand > 8388607) && (extraOperandBytes == 3))
    {
      operand = operand - 16777216;
      return operand;
    }
    else if ((operand > 32767) && (extraOperandBytes == 4))
    {
      operand = operand - 65536;
      return operand;
    }
  }
  return operand;
 }

int extend(state *s, unsigned char byte, FILE *in, int operand, int opcode)
{
  operand = 0;
  int n = byte & 0x3F;
  int extraOperandBytes = n >> 4;

  if (extraOperandBytes == 3)
    extraOperandBytes++;

  if (extraOperandBytes > 0)
  {
    for (int i = 0; i < extraOperandBytes; i++)
    {
      unsigned char nextExtensionByte = fgetc(in);
      if (i == 0)
      {
        operand = nextExtensionByte;
      }
      else
      {
        operand = (operand << 8) | nextExtensionByte;
      }
    }
   }
   operand = convertToNegative(opcode, operand, extraOperandBytes);
   return operand;
}


int unpackOperand(state *s, unsigned char byte, int operand, unsigned char opcode, FILE *in)
{
  //find out extra operand bytes
  if ( (byte >> 6 == 3))
  {
    operand = extend(s, byte, in, operand, opcode);
    return operand;
  }
  operand = (byte & 0x3F);

  //ensure numbers are made negative is most significant bit is 1
  if ((operand > 31) && ((byte >> 6) != 2))
  {
    operand = operand - 64;
    return operand;
  }
  return operand;
}

void doDX(state *s, display *d, int dx)
{
  s->prevX = s->x;
  s->x = s->x + dx;
}

void doDY(state *s, display *d, int dy)
{
  s->prevY = s->y;
  s->y = s->y + dy;
  if ((s->pen) && (s->rgba == 255))
    line(d, s->prevX, s->prevY, s->x, s->y);
  else if ((s->pen) && (s->rgba != 255))
    cline(d, s->prevX, s->prevY, s->x, s->y, s->rgba);

  s->prevX = s->x;
}

void doTime(state *s, display *d, int ms)
{
  pause(d, ms);
}

void doPen(state *s)
{
  s->pen = ! s->pen;
}

void doClear(display *d)
{
  clear(d);
}

void doKey(display *d)
{
  key(d);
}

void doColour(display *d, state *s, int operand)
{
    s->rgba = operand;
}

void execute(display *d, state *s, unsigned char opcode, int operand)
{
  switch (opcode)
  {
    case DX:
      doDX(s, d, operand);
      break;
    case DY:
      doDY(s, d, operand);
      break;
    case DT: doTime(s, d, operand);
      break;
    case PEN: doPen(s);
      break;
    case CLEAR: doClear(d);
      break;
    case KEY: doKey(d);
      break;
    case COLOUR:
      doColour(d, s, operand);
      break;
  }
}

void interpret(FILE *in, display *d)
{
  state *s = malloc(sizeof(state));
  *s = (state) { 0, 0, 0, 0, false, 255};

  unsigned char byte = 0;
  unsigned char opcode = 0;
  int operand = 0;


  while (! feof(in))
  {
   byte = fgetc(in);
   opcode = unpackOpcode(byte, opcode);
   operand = unpackOperand(s, byte, operand, opcode, in);
   //printf("%x\n", byte);
   printf("%d -- %d\n", opcode, operand);
   execute(d, s, opcode, operand);
  }
  printf("\n");
}

// Read sketch instructions from the given file.  If test is NULL, display the
// result in a graphics window, else check the graphics calls made.
void run(char *filename, char *test[])
{
    FILE *in = fopen(filename, "rb");
    if (in == NULL)
    {
        fprintf(stderr, "Can't open %s\n", filename);
        exit(1);
    }
    display *d = newDisplay(filename, 200, 200, test);
    interpret(in, d);
    end(d);
    fclose(in);
}

// ----------------------------------------------------------------------------
// Nothing below this point needs to be changed.
// ----------------------------------------------------------------------------

// Forward declaration of test data.
char **lineTest, **squareTest, **boxTest, **oxoTest, **diagTest, **crossTest,
     **clearTest, **keyTest, **pausesTest, **fieldTest, **lawnTest;

void testSketches()
{
    // Stage 1
    run("line.sketch", lineTest);
    run("square.sketch", squareTest);
    run("box.sketch", boxTest);
    run("oxo.sketch", oxoTest);

    // Stage 2
    run("diag.sketch", diagTest);
    run("cross.sketch", crossTest);

    // Stage 3
    run("clear.sketch", clearTest);
    run("key.sketch", keyTest);

    // Stage 4
    run("pauses.sketch", pausesTest);
    run("field.sketch", fieldTest);
    run("lawn.sketch", lawnTest);
}

int main(int n, char *args[n])
{
    if (n == 1) testSketches();
    else if (n == 2) run(args[1], NULL);
    else
    {
        fprintf(stderr, "Usage: sketch [file.sketch]");
        exit(1);
    }
}

// Each test is a series of calls, stored in a variable-length array of strings,
// with a NULL terminator.

// The calls that should be made for line.sketch.
char **lineTest = (char *[]) {
    "line(d,30,30,60,30)", NULL
};

// The calls that should be made for square.sketch.
char **squareTest = (char *[]) {
    "line(d,30,30,60,30)", "line(d,60,30,60,60)",
    "line(d,60,60,30,60)","line(d,30,60,30,30)", NULL
};

// The calls that should be made for box.sketch.
char **boxTest = (char *[]) {
    "line(d,30,30,32,30)", "pause(d,10)", "line(d,32,30,34,30)", "pause(d,10)",
    "line(d,34,30,36,30)", "pause(d,10)", "line(d,36,30,38,30)", "pause(d,10)",
    "line(d,38,30,40,30)", "pause(d,10)", "line(d,40,30,42,30)", "pause(d,10)",
    "line(d,42,30,44,30)", "pause(d,10)", "line(d,44,30,46,30)", "pause(d,10)",
    "line(d,46,30,48,30)", "pause(d,10)", "line(d,48,30,50,30)", "pause(d,10)",
    "line(d,50,30,52,30)", "pause(d,10)", "line(d,52,30,54,30)", "pause(d,10)",
    "line(d,54,30,56,30)", "pause(d,10)", "line(d,56,30,58,30)", "pause(d,10)",
    "line(d,58,30,60,30)", "pause(d,10)", "line(d,60,30,60,32)", "pause(d,10)",
    "line(d,60,32,60,34)", "pause(d,10)", "line(d,60,34,60,36)", "pause(d,10)",
    "line(d,60,36,60,38)", "pause(d,10)", "line(d,60,38,60,40)", "pause(d,10)",
    "line(d,60,40,60,42)", "pause(d,10)", "line(d,60,42,60,44)", "pause(d,10)",
    "line(d,60,44,60,46)", "pause(d,10)", "line(d,60,46,60,48)", "pause(d,10)",
    "line(d,60,48,60,50)", "pause(d,10)", "line(d,60,50,60,52)", "pause(d,10)",
    "line(d,60,52,60,54)", "pause(d,10)", "line(d,60,54,60,56)", "pause(d,10)",
    "line(d,60,56,60,58)", "pause(d,10)", "line(d,60,58,60,60)", "pause(d,10)",
    "line(d,60,60,58,60)", "pause(d,10)", "line(d,58,60,56,60)", "pause(d,10)",
    "line(d,56,60,54,60)", "pause(d,10)", "line(d,54,60,52,60)", "pause(d,10)",
    "line(d,52,60,50,60)", "pause(d,10)", "line(d,50,60,48,60)", "pause(d,10)",
    "line(d,48,60,46,60)", "pause(d,10)", "line(d,46,60,44,60)", "pause(d,10)",
    "line(d,44,60,42,60)", "pause(d,10)", "line(d,42,60,40,60)", "pause(d,10)",
    "line(d,40,60,38,60)", "pause(d,10)", "line(d,38,60,36,60)", "pause(d,10)",
    "line(d,36,60,34,60)", "pause(d,10)", "line(d,34,60,32,60)", "pause(d,10)",
    "line(d,32,60,30,60)", "pause(d,10)", "line(d,30,60,30,58)", "pause(d,10)",
    "line(d,30,58,30,56)", "pause(d,10)", "line(d,30,56,30,54)", "pause(d,10)",
    "line(d,30,54,30,52)", "pause(d,10)", "line(d,30,52,30,50)", "pause(d,10)",
    "line(d,30,50,30,48)", "pause(d,10)", "line(d,30,48,30,46)", "pause(d,10)",
    "line(d,30,46,30,44)", "pause(d,10)", "line(d,30,44,30,42)", "pause(d,10)",
    "line(d,30,42,30,40)", "pause(d,10)", "line(d,30,40,30,38)", "pause(d,10)",
    "line(d,30,38,30,36)", "pause(d,10)", "line(d,30,36,30,34)", "pause(d,10)",
    "line(d,30,34,30,32)", "pause(d,10)", "line(d,30,32,30,30)", "pause(d,10)",
    NULL
};

// The calls that should be made for box.sketch.
char **oxoTest = (char *[]) {
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "line(d,30,40,60,40)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "line(d,30,50,60,50)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "line(d,40,30,40,60)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "line(d,50,30,50,60)", NULL
};

// The calls that should be made for diag.sketch.
char **diagTest = (char *[]) {
    "line(d,30,30,60,60)", NULL
};

// The calls that should be made for cross.sketch.
char **crossTest = (char *[]) {
    "line(d,30,30,60,60)", "line(d,60,30,30,60)", NULL
};

// The calls that should be made for clear.sketch.
char **clearTest = (char *[]) {
    "line(d,30,40,60,40)", "line(d,30,50,60,50)", "line(d,40,30,40,60)",
    "line(d,50,30,50,60)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "clear(d)", "line(d,30,30,60,60)",
    "line(d,60,30,30,60)", NULL
};

// The calls that should be made for key.sketch.
char **keyTest = (char *[]) {
    "line(d,30,40,60,40)", "line(d,30,50,60,50)", "line(d,40,30,40,60)",
    "line(d,50,30,50,60)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "key(d)", "clear(d)", "line(d,30,30,60,60)",
    "line(d,60,30,30,60)", NULL
};

// The calls that should be made for diag.sketch.
char **pausesTest = (char *[]) {
    "pause(d,0)", "pause(d,0)", "pause(d,127)", "pause(d,128)", "pause(d,300)",
    "pause(d,0)", "pause(d,71469)", NULL
};

// The calls that should be made for field.sketch.
char **fieldTest = (char *[]) {
    "line(d,30,30,170,30)", "line(d,170,30,170,170)",
    "line(d,170,170,30,170)", "line(d,30,170,30,30)", NULL
};

// The calls that should be made for field.sketch.
char **lawnTest = (char *[]) {
    "cline(d,30,30,170,30,16711935)", "cline(d,170,30,170,170,16711935)",
    "cline(d,170,170,30,170,16711935)", "cline(d,30,170,30,30,16711935)",
    NULL
};
