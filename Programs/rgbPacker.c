#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int pack(int r, int g, int b, int o)
{
  int packed = (r << 24 | g << 16 | b << 8 | o);
  printf("%x\n", packed);
  return packed;
}

void unpack(int input)
{
  int rgba[4];
  rgba[0] = ((input >> 24) & 0xFF);
  rgba[1] = ((input >> 16) & 0xFF);
  rgba[2] = ((input >> 8) & 0xFF);
  rgba[3] = ((input) & 0xFF);
}


int main(int argc, char* argv[])
{
  if (argc == 5)
  {
    int r = atoi(argv[1]);
    int g = atoi(argv[2]);
    int b = atoi(argv[3]);
    int o = atoi(argv[4]);
    pack(r, g, b, o);
  }
  else if (argc == 2)
  {
    int toUnpack = atoi(argv[1]);
    unpack(toUnpack);
  }
  return 0;
}
