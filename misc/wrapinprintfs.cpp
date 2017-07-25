#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  FILE *source = fopen("index.html", "r");
  FILE *result = fopen("index-wrapped.txt", "a+");

  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  while( (read = getline(&line, &len, source)) != -1 ) {
    //printf("Retrieved line of length %zu :\n", read);
    //printf("%s", line);

    //char *src, *dst;
    //for(src = dst = line; *src != '\0'; src++) {
    //  *dst = *src;
    //  if(*dst != '\r' || *dst != '\n') dst++;
    //}
    //*dst = '\0';

    //char *dst;
    //strncpy(dst, line, strlen(line)-1);

    line[ strlen(line) - 1 ] = '\0';

    fputs("printf(\"", result);
    fputs(line, result);
    fputs("\\n\");\n", result);
  }

  fclose(source);
  fclose(result);

  if(line)
    free(line);

  return 0;

}