#ifndef RANDOM
#define RANDOM

#if DEBUG
#define assert(expression) if(!(expression)) { *(int*)0 = 0; }
#else
#define assert(expression)
#endif

int stringsAreEqual(char *a, char *b) {
  while((*a != '\0') && (*a == *b))
    a++, b++;
  return ((*a == '\0') && (*b == '\0'));
}

#endif