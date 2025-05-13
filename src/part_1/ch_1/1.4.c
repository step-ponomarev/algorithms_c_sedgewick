#include "../../lib/graph_link.h"
#include <stdio.h>

static int id[MAX_VAL];

int main(const int argc, char *argv[]) {
  for (int i = 0; i < MAX_VAL; i++) {
    id[i] = i;
  }

  int p, q;
  while (scanf("%d %d\\n", &p, &q) == 2) {
    if (!is_valid(p) || !is_valid(q)) {
      fprintf(stderr, "Values must be < %d, p: %d, q: %d\n", MAX_VAL, p, q);
      return -1;
    }

    // one root -> one union
    if (find_root(id, p) == find_root(id, q)) {
      continue;
    }

    merge_unions(id, p, q);
    for (int i = 0; i < MAX_VAL; i++) {
      printf("%d ", id[i]);
    }
    printf("\n");
  }

  return 0;
}
