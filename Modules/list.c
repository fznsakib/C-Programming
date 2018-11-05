#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct node
{
  struct node *previous;
  struct node *next;
  void *item;
};
typedef struct node node;

struct list
{
  node *first;
  node *last;
  node *current;
  int length;
  int dataSize;
  //int capacity;
};
//typedef struct list list;

list *newList(int b)
{
  //Create a sentinel nodes and split them into half nodes that point to each
  //other. Initialise rest of parameters of list.
  list *l = malloc(sizeof(list));
  node *sentinel = malloc(sizeof(node));
  sentinel->previous = sentinel;
  sentinel->next = sentinel;
  sentinel->item = NULL;
  l->dataSize = b;
  l->first = sentinel;
  l->last = sentinel;
  l->current = sentinel;
  l->length = 0;
  return l;
}

void start(list *l)
{
  //As I am pointing after the nodes, the start of the list will be the second
  //half of the gap.
  l->current = l->first->next;
}

void end(list *l)
{
  l->current = l->last->previous;
}

bool atStart(list *l)
{
  if (l->current == l->first->next)
    return true;
  else
    return false;
}

bool atEnd(list *l)
{
  if (l->current == l->last->previous)
    return true;
  else
    return false;
}

void forward(list *l)
{
  if (!atEnd(l))
    l->current = l->current->next;

  else
  {
    printf("Error: Already at end of list.\n");
    exit(0);
  }
}

void backward(list *l)
{
  if (!atStart(l))
    l->current = l->current->previous;

  else
  {
    printf("Error: Already at start of list.\n");
    exit(0);
  }
}

void insertBefore(list *l, void *p)
{
  //Initialise left, middle and right nodes for readability. Also a buffer
  //pointer to copy the data to before adding to the node.
  l->length++;
  node *left = l->current->previous;
  node *mid = malloc(sizeof(node));
  node *right = l->current;
  void *newItem = malloc(l->dataSize);

  //Rearrange pointers left and right to the new node. Make middle node
  //pointers point to either nodes on the sides.
  left->next = mid;
  right->previous = mid;
  mid->previous = left;
  mid->next = right;
  memcpy(newItem, p, l->dataSize);
  mid->item = newItem;
}

void insertAfter(list *l, void *p)
{
  l->length++;
  node *left = l->current;
  node *mid = malloc(sizeof(node));
  node *right = l->current->next;
  void *newItem = malloc(l->dataSize);

  left->next = mid;
  right->previous = mid;
  mid->previous = left;
  mid->next = right;
  memcpy(newItem, p, l->dataSize);
  mid->item = newItem;
}

void getBefore(list *l, void *p)
{
  if (!atStart(l))
  {
    memcpy(p, l->current->previous->item, l->dataSize);
  }
  else
  {
    printf("Error: Already at start of list.\n");
    exit(0);
  }
}

void getAfter(list *l, void *p)
{
  if (!atEnd(l))
  {
    memcpy(p, l->current->next->item, l->dataSize);
  }
  else
  {
    printf("Error: Already at end of list.\n");
    exit(0);
  }
}

void setBefore(list *l, void *p)
{
  if (!atStart(l) && (l->current->previous != NULL))
  {
    memcpy(l->current->previous->item, p, l->dataSize);
  }
  else
  {
    printf("Error: Already at start of list.\n");
    exit(0);
  }
}

void setAfter(list *l, void *p)
{
  if (!atEnd(l) && (l->current->next != NULL))
  {
    memcpy(l->current->next->item, p, l->dataSize);
  }
  else
  {
    printf("Error: Already at end of list.\n");
    exit(0);
  }
}

void deleteBefore(list *l)
{
  if (!atStart(l) && (l->current->previous != NULL))
  {
    l->length--;
    node *left = l->current->previous->previous;
    node *right = l->current;
    node *mid = l->current->previous;
    left->next = right;
    right->previous = left;
    free(mid);
  }
  else if (atStart(l))
  {
    printf("Error: Already at start of list.");
    exit(0);
  }
}

void deleteAfter(list *l)
{
  if (!atEnd(l) && (l->current->next != NULL))
  {
    l->length--;
    node *left = l->current;
    node *right = l->current->next->next;
    node *mid = l->current->next;
    left->next = right;
    right->previous = left;
    free(mid);
  }
  else if (atEnd(l))
  {
    printf("Error: Already at start of list.\n");
    exit(0);
  }
}

int length(list *l)
{
  return l->length;
}

bool eqb(int n, int m)
{
  if (n == m)
    return true;
  else
    return false;
}

void testLists()
{
  int passed = 0;

  //Test 1 - atStart
  printf("Test 1:\n");
  list *l = newList(4);
  if (atStart(l) == true)
  {
    printf("atStart = Passed\n");
    passed++;
  }
  else
  {
    printf("atStart = Failed\n");
  }

  //Test 2 - atEnd
  printf("Test 2:\n");
  l = newList(4);
  int num = 42;
  insertAfter(l, &num);
  forward(l);
  if (atEnd(l) == true)
  {
    printf("insertAfter (42) + forward + atEnd = Passed\n");
    passed++;
  }
  else
  {
    printf("insertAfter (42) + forward + atEnd = Failed\n");
  }


  //Test 3 - insertAfter/getAfter
  printf("Test 3:\n");
  l = newList(4);
  num = 42;
  insertAfter(l, &num);
  num = 0;
  getAfter(l, &num);
  if (eqb(num, 42) == true)
  {
    printf("insertAfter (42) + getAfter (42) = Passed\n");
    passed++;
  }
  else
  {
    printf("insertAfter (42) + getAfter (42) = Failed\n");
  }

  //Test 4 - insertBefore/getBefore
  printf("Test 4:\n");
  l = newList(4);
  num = 42;
  insertBefore(l, &num);
  num = 0;
  getBefore(l, &num);
  if (eqb(num, 42) == true)
  {
    printf("insertBefore (42) + getBefore (42) = Passed\n");
    passed++;
  }
  else
  {
    printf("insertBefore (42) + getBefore (42) = Failed\n");
  }

  //Test 5 - forward
  printf("Test 5:\n");
  l = newList(4);
  num = 42;
  insertAfter(l, &num);
  num = 22;
  insertAfter(l, &num);
  num = 0;
  forward(l);
  getAfter(l, &num);
  if (eqb(num, 42) == true)
  {
    printf("insertAfter (42) + insertAfter (22) + forward + getAfter (42) = Passed\n");
    passed++;
  }
  else
  {
    printf("insertAfter (42) + insertAfter (22) + getAfter (22) = Failed\n");
  }

  //Test 5 - backward
  printf("Test 6:\n");
  l = newList(4);
  num = 42;
  insertBefore(l, &num);
  num = 22;
  insertBefore(l, &num);
  num = 0;
  backward(l);
  getBefore(l, &num);
  if (eqb(num, 42) == true)
  {
    printf("insertBefore (42) + insertBefore (22) + backward + getBefore (42) = Passed\n");
    passed++;
  }
  else
  {
    printf("insertBefore (42) + insertBefore (22) + backward + getBefore (42) = Failed\n");
  }

  //Test 7 - setAfter
  printf("Test 7:\n");
  l = newList(4);
  num = 42;
  insertAfter(l, &num);
  num = 22;
  setAfter(l, &num);
  num = 0;
  getAfter(l, &num);
  if (eqb(num, 22) == true)
  {
    printf("insertAfter (42) + setAfter (22) + getAfter (22) = Passed\n");
    passed++;
  }
  else
  {
    printf("insertAfter (42) + setAfter (22) + getAfter (22) = Failed\n");
  }

  //Test 8 - setBefore
  printf("Test 8:\n");
  l = newList(4);
  num = 42;
  insertBefore(l, &num);
  num = 22;
  setBefore(l, &num);
  num = 0;
  getBefore(l, &num);
  if (eqb(num, 22) == true)
  {
    printf("insertBefore (42) + setBefore (22) + getBefore (22) = Passed\n");
    passed++;
  }
  else
  {
    printf("insertBefore (42) + setBefore (22) + getBefore (22) = Failed\n");
  }

  //Test 9 - deleteAfter
  printf("Test 9:\n");
  l = newList(4);
  num = 42;
  insertAfter(l, &num);
  num = 22;
  insertAfter(l, &num);
  deleteAfter(l);
  num = 0;
  getAfter(l, &num);
  if (eqb(num, 42) == true)
  {
    printf("insertAfter (42) + insertAfter (22) + deleteAfter + getAfter (42) = Passed\n");
    passed++;
  }
  else
  {
    printf("insertAfter (42) + insertAfter (22) + deleteAfter + getAfter (42) = Failed\n");
  }

  //Test 10 - deleteBefore
  printf("Test 0:\n");
  l = newList(4);
  num = 42;
  insertBefore(l, &num);
  num = 22;
  insertBefore(l, &num);
  deleteBefore(l);
  num = 0;
  getBefore(l, &num);
  if (eqb(num, 42) == true)
  {
    printf("insertBefore (42) + insertBefore (22) + deleteBefore + getBefore (42) = Passed\n");
    passed++;
  }
  else
  {
    printf("insertBefore (42) + insertBefore (22) + deleteBefore + getBefore (42) = Failed\n");
  }

  //Test 11 - try with characters
  printf("Test 11:\n");
  l = newList(1);
  char character = 'c';
  insertBefore(l, &character);
  character = 'f';
  insertBefore(l, &character);
  deleteBefore(l);
  character = '0';
  getBefore(l, &character);
  if (eqb(character, 'c') == true)
  {
    printf("insertBefore ('c') + insertBefore ('f') + deleteBefore + getBefore ('c') = Passed\n");
    passed++;
  }
  else
  {
    printf("insertBefore ('c') + insertBefore ('f') + deleteBefore + getBefore ('c') = Failed\n");
  }

  //Test 12 - add nodes then go forward to the end
  printf("Test 12:\n");
  l = newList(4);
  num = 42;
  insertAfter(l, &num);
  insertAfter(l, &num);
  insertAfter(l, &num);
  forward(l);
  forward(l);
  forward(l);
  if (atEnd(l) == true)
  {
    printf("insertAfter x3 + forward x3 + atEnd = Passed\n");
    passed++;
  }
  else
  {
    printf("insertAfter x3 + forward x3 + atEnd = Failed\n");
  }

  //Test 13 - add nodes then go forward, then back to the start
  printf("Test 13:\n");
  l = newList(4);
  num = 42;
  insertAfter(l, &num);
  insertAfter(l, &num);
  insertAfter(l, &num);
  forward(l);
  forward(l);
  forward(l);
  backward(l);
  backward(l);
  if (atStart(l) == true)
  {
    printf("insertAfter x3 + forward x3 + backward x2 + atStart = Passed\n");
    passed++;
  }
  else
  {
    printf("insertAfter x3 + forward x3 + backward x2 + atStart = Failed\n");
  }

  //Test 14 - add nodes then go forward, then delete and check if at start
  printf("Test 14:\n");
  l = newList(4);
  num = 42;
  insertAfter(l, &num);
  insertAfter(l, &num);
  insertAfter(l, &num);
  forward(l);
  forward(l);
  forward(l);
  deleteBefore(l);
  deleteBefore(l);

  if (atStart(l) == true)
  {
    printf("insertAfter x3 + forward x3 + deleteBefore x2 + atStart = Passed\n");
    passed++;
  }
  else
  {
    printf("insertAfter x3 + forward x3 + deleteBefore x2 + atStart = Failed\n");
  }

  //Test 15 - add nodes then go forward, then delete and check if empty
  printf("Test 15:\n");
  l = newList(4);
  num = 42;
  insertAfter(l, &num);
  insertAfter(l, &num);
  insertAfter(l, &num);
  deleteAfter(l);
  deleteAfter(l);
  deleteAfter(l);
  if (length(l) == 0)
  {
    printf("insertAfter x3 + deleteAfter x3 = Passed\n");
    printf("Length is %d, therefore list is empty.\n", length(l));
    passed++;
  }
  else
  {
    printf("insertAfter x3 + deleteAfter x3 = Failed\n");
  }

  //Test 16 - traverse forward until end of list
  printf("Test 16:\n");
  l = newList(4);
  num = 42;
  insertAfter(l, &num);
  num = 22;
  insertAfter(l, &num);
  num = 15;
  insertAfter(l, &num);
  num = 61;
  insertAfter(l, &num);
  num = 0;
  int expected[4] = {61, 15, 22, 42};
  printf("insertAfter (42) + insertAfter (22) + insertAfter (15) + insertAfter (61) + ((getAfter + forward) x 4)\n");
  int i = 0;
  int j = 0;
  while (!atEnd(l))
  {
    getAfter(l, &num);
    if (eqb(num, expected[i]))
    {
      printf("Value as expected: %d\n", expected[i]);
      j++;
    }
    else
    {
      printf("Wrong Value. Expected: %d, Output: %d\n", expected[i], num);
    }
    i++;
    forward(l);
  }
  if (j == 4) passed++;
  printf("Array traversed forward until end of list\n");

  //Test 17 - traverse backward until end of list
  printf("Test 17:\n");
  l = newList(4);
  num = 31;
  insertBefore(l, &num);
  num = 28;
  insertBefore(l, &num);
  num = 7;
  insertBefore(l, &num);
  num = 78;
  insertBefore(l, &num);
  num = 0;
  int expected2[4] = {78, 7, 28, 31};
  printf("insertBefore (31) + insertBefore (28) + insertBefore (7) + insertBefore (78) + ((getBefore + backward) x 4)\n");
  i = 0;
  j = 0;
  while (!atStart(l))
  {
    getBefore(l, &num);
    if (eqb(num, expected2[i]))
    {
      printf("Value as expected: %d\n", expected2[i]);
      j++;
    }
    else
    {
      printf("Wrong Value. Expected: %d, Output: %d\n", expected2[i], num);
    }
    i++;
    backward(l);
  }
  if (j == 4) passed++;
  printf("Array traversed backward until start of list\n");


  printf("\nTests passed: %d out of 17\n", passed);
}
