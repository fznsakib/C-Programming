#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

char *binary(int n)
{
  char *string = malloc(33);
  int k;
  printf("%d in binary form is:\n", n);
  for (int shift = 0; shift < 32; shift++)
  {
    k = n >> (31 - shift);
    if (k & 1)
    {
      strcpy(&string[shift], "1");
    }
    else
    {
      strcpy(&string[shift], "0");
    }
  }
  printf("%s\n", string);
  return string;
}

char *hex(int n)
{
  char *string = malloc(9);
  int k;
  char *digits = "0123456789ABCDEF";
  printf("%d in hex form is:\n", n);
  for (int shift = 0; shift < 8; shift++)
  {
    k = (n >> (4 * (7 - shift))) & 0xF;
    string[shift] = digits[k];
  }
  printf("%s\n", string);
  return string;
}

int main(int argc, char* argv[])
{
  int input = atoi(argv[1]);
  binary(input);
  hex(input);
  return 0;
}
