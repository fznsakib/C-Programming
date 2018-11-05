#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int pack(int x, int y, int z)
{
  int packed = ((x & 0x3FF) << 20 | (y & 0x3FF) << 10 | (z & 0x3FF));
  return packed;
}

void unpack(int input)
{
  int xyz[3];
  //each coordinate is ten bits long
  int x = ((input >> 20) & 0x3FF);
  //check if left most bit is 1, if it is, we must adapt the byte to be negative
  if ((x & 0x200) != 0)
    x = (0xFFFFFC00 | x);
  xyz[0] = x;
  int y = ((input >> 10) & 0x3FF);
  if ((y & 0x200) != 0)
    y = (0xFFFFFC00 | y);
  xyz[1] = y;
  int z = (input & 0x3FF);
  if ((z & 0x200) != 0)
    z = (0xFFFFFC00 | z);
  xyz[2] = z;
}


int main(int argc, char* argv[])
{
  if (argc == 4)
  {
    int x = atoi(argv[1]);
    int y = atoi(argv[2]);
    int z = atoi(argv[3]);
    pack(x, y, z);
  }
  else if (argc == 2)
  {
    int toUnpack = atoi(argv[1]);
    unpack(toUnpack);
  }
  return 0;
}
