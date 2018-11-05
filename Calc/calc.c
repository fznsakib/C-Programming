#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

//check if operand is a number
bool isNumber(char input)
{
  if (isdigit(input) != 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool isOperator(char input)
{
  char *allowed = "+-x/";

  if (strchr(allowed, input) == NULL)
  {
    return false;
  }
  else
  {
    return true;
  }
}

void pushNumber(stack *s, char input)
{
  input = atoi(&input);
  push(s, input);
}

void executeOperation(stack *s, char input)
{
  int x, y;
  x = pop(s);
  y = pop(s);

  if (input == '+') push(s, (x + y));
  else if (input == '-') push(s, (y - x));
  else if (input == 'x') push(s, (x * y));
  else if (input == '/') push(s, (y / x));
}

void run(int n, char const *expression[])
{
  stack *s = create();
  int i = 1;
  while (i < n)
  {
    if (isNumber(*expression[i])) pushNumber(s, *expression[i]);
    if (isOperator(*expression[i])) executeOperation(s, *expression[i]);
    i++;
  }
  printf("%d\n", top(s));
}


int main(int n, char const *argv[])
{
  if (n == 1)
  {
    printf("Please enter an expression\n");
    exit(1);
  }
  else
  {
    run(n, argv);
  }
  return 0;
}
