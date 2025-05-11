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
    const int pV = find_root(id, p);
    const int pQ = find_root(id, q);
    if (pV == pQ) {
      continue;
    }

    merge_unions(id, pV, pQ);
    printf("%10d-%d, id[i]x%d\n", p, q, MAX_VAL);
  }

  return 0;
}
