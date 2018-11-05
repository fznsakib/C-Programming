#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

void test()
{

}

void sort(list *l)
{

}

int main(int argc, char **argv)
{
  if (argc == 1)
  {
    test();
  }
  else if (argc > 1)
  {
    if (isdigit(argv[2]))
    {
      int bytes = atoi(argv[2]);
      list *l = newList(argv[2]);
      sort(l);
    }
    else
    {
      printf("First argument should be number of bytes that data type takes\n");
      exit(1);
    }
  }
}
