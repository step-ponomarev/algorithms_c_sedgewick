#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(const int argc, const char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Expected 2 char seq\n");
    return -1;
  }

  const char *s1 = argv[1];
  const char *s2 = argv[2];

  char *res_seq = malloc(sizeof(char) * (strlen(s1) + strlen(s2)));
  size_t p = 0;

  int star_count = 0;
  while (*s1 != '\0') {
    if (*s1 == *s2) {
      for (int i = 0; i < star_count; i++) {
        res_seq[p++] = '*';
      }
      res_seq[p++] = *(s1++);
      s2++;

      star_count = 0;
      continue;
    }

    res_seq[p++] = *(s1++);
    star_count++;
  }

  if (*s2 != '\0') {
    printf("'%s' cannot be converted to '%s' by stack\n", argv[1], argv[2]);
    free(res_seq);
    return 0;
  }

  for (int i = 0; i < star_count; i++) {
    res_seq[p++] = '*';
  }
  res_seq[p] = '\0';

  printf("Use '%s' with stack to get '%s'\n", res_seq, argv[2]);
  free(res_seq);

  return 0;
}
