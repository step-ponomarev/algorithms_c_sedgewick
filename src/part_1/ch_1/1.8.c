#include "../../lib/graph_link.h"
#include <stdio.h>

// use src/lib/graph_link_quck_union

static int id[MAX_VAL];
static int sz[MAX_VAL];

int main(const int argc, char *argv[]) {
  for (int i = 0; i < MAX_VAL; i++) {
    id[i] = i;
    sz[i] = 1;
  }

  int p, q;
  while (scanf("%d %d\\n", &p, &q) == 2) {
    if (!is_valid(p) || !is_valid(q)) {
      fprintf(stderr, "Values must be < %d, p: %d, q: %d\n", MAX_VAL, p, q);
      return -1;
    }

    // one root -> one union
    const int pV = find_root_with_compression(id, p);
    const int pQ = find_root_with_compression(id, q);
    if (pV == pQ) {
      continue;
    }

    if (sz[pV] > sz[pQ]) { // присоеденяем к большему меньший
      merge_unions(id, pQ, pV);
      sz[pV] += sz[pQ];
    } else {
      merge_unions(id, pV, pQ);
      sz[pQ] += sz[pV];
    }

    for (int i = 0; i < MAX_VAL; i++) {
      printf("%d ", id[i]);
    }
    printf("\n");
  }

  return 0;
}
