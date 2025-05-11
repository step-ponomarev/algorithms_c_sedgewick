#include <stdio.h>

#define MAX_VAL 1000

static int id[MAX_VAL];

int main(const int argc, char *argv[]) {
  for (int i = 0; i < MAX_VAL; i++) {
    id[i] = i;
  }

  int p, q;
  while (scanf("%d %d\\n", &p, &q) == 2) {
    if (p >= MAX_VAL || q >= MAX_VAL) {
      fprintf(stdout, "Values must be < %d, p: %d, q: %d\n", MAX_VAL, p, q);
      return -1;
    }

    // one root -> one union
    if (id[p] == id[q]) {
      printf("id[i]x%d\n", 1);
      continue;
    }

    const int tmp = id[p];
    for (int i = 0; i < MAX_VAL; i++) {
      if (id[i] != tmp) {
        continue;
      }

      id[i] = id[q];
    }

    printf("%10d-%d, id[i]x%d\n", p, q, MAX_VAL);
  }

  return 0;
}
