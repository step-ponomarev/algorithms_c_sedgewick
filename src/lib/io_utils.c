#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_STR_LEN 100
#define GROW_KOEF 1.5

typedef struct str {
  char *str;
  size_t len;
} String;

String read_line(FILE *file) {
  int allocated = INIT_STR_LEN;
  char *line = malloc(sizeof(char) * allocated);

  int i = 0;
  int ch;
  while ((ch = fgetc(file)) != EOF && ch != '\n') {
    if (i == allocated - 1) {
      line = realloc(line, allocated *= GROW_KOEF);
    }

    line[i++] = ch;
  }

  if (i == 0) {
    return (String){.str = NULL, .len = 0};
  }

  line[i] = '\0';

  char *res_line = malloc(i * sizeof(char));
  memcpy(res_line, line, i * sizeof(char));

  free(line);

  return (String){.str = res_line, .len = i};
}
