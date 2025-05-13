#include "../../lib/graph_link.h"
#include <stdio.h>

static int roots[MAX_VAL]; // 3N data
static int *id[MAX_VAL];
static int sz[MAX_VAL];

int main(const int argc, char *argv[]) {
  for (int i = 0; i < MAX_VAL; i++) {
    roots[i] = i;
    id[i] = &roots[i];
    sz[i] = 1;
  }

  int p, q;
  while (scanf("%d %d\\n", &p, &q) == 2) {
    if (!is_valid(p) || !is_valid(q)) {
      fprintf(stderr, "Values must be < %d, p: %d, q: %d\n", MAX_VAL, p, q);
      return -1;
    }

    // find O(1)
    if (*id[p] == *id[q]) {
      continue;
    }

    // union ~O(1)
    if (sz[p] < sz[q]) {
      *id[p] = *id[q];
      id[p] = id[q];
      sz[q] += sz[p];
    } else {
      *id[q] = *id[p];
      id[q] = id[p];
      sz[p] += sz[q];
    }

    for (int i = 0; i < MAX_VAL; i++) {
      printf("%d ", *id[i]);
    }
    printf("\n");
  }

  return 0;
}
