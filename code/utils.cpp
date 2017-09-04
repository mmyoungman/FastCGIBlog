//#ifndef UTILS_H
//#define UTILS_H

#if DEBUG
#define assert(expression) if(!(expression)) { *(int*)0 = 0; }
#else
#define assert(expression)
#endif

int stringLength(char* str) {
  char* strPtr = str;
  while(*strPtr != '\0') { strPtr++; }
  return strPtr - str;
}

char* stringCopy(char *s) {
  char* copy = (char*)malloc(sizeof(char)*(stringLength(s)+1));
  char* sPtr = s;
  char* copyPtr = copy;
  while(*sPtr != '\0') {
    *copyPtr = *sPtr;
    copyPtr++, sPtr++;
  }
  *copyPtr = '\0';
  return copy;
}

int stringsAreEqual(char *a, char *b) {
  while((*a != '\0') && (*a == *b))
    a++, b++;
  return ((*a == '\0') && (*b == '\0'));
}

int stringBeginsWith(char* str, char *start) {
  char* strPtr = str;
  while((*start != '\0') && (*start == *strPtr))
    strPtr++, start++;
  return *start == '\0';
}

char** stringSplit(char* str, char c, int* size) {
  int numStrs = 1;
  char* strPtr = str;
  while(*strPtr != '\0') {
    if(*strPtr == c) {
      *strPtr = '\0';
      numStrs++;
    }
    strPtr++;
  }
  char** result = (char**)malloc(sizeof(char*)*numStrs);
  char* strStart = str;
  strPtr = strStart;
  int i = 0;
  while(numStrs > 0) {
    if(*strPtr == '\0') {
      numStrs--;
      result[i] = strStart;
      strStart = strPtr + 1;
      i++;
    }
    strPtr++;
  }
  *size = i;
  return result;
}

void stringConcat(char* str, char* addition) {
  int newLength = stringLength(str) + stringLength(addition) + 1;
  str = (char*)realloc(str, sizeof(char) * newLength);
  char* strPtr = str;
  while(*strPtr != '\0') { strPtr++; }
  char* addPtr = addition;
  while(*addPtr != '\0') {
    *strPtr = *addPtr;
    strPtr++, addPtr++;
  }
  *strPtr = '\0';
}

//#endif