#include "../../lib/io_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_CAPACITY 2
#define GROW_KOEF 2

int compare(const void *, const void *);

int main(const int argc, const char *argv[]) {
  int line_capacity = INIT_CAPACITY;
  char **lines = malloc(sizeof(char *) * line_capacity);

  int len = 0;
  String str;
  while ((str = read_line(stdin)).len > 0) {
    if (len == line_capacity - 1) {
      lines = realloc(lines, sizeof(char *) * (line_capacity *= GROW_KOEF));
    }

    lines[len++] = str.str;
  }

  int (*cmp)(const void *, const void *) = &compare;
  qsort(lines, len, sizeof(char *), cmp);

  for (int j = 0; j < len; j++) {
    printf("%s\n", lines[j]);
    free(lines[j]);
  }
  free(lines);

  return 0;
}

int compare(const void *s1, const void *s2) {
  return strcmp(*(char **)s1, *(char **)s2);
}
