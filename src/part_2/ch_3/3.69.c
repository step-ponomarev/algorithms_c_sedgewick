#include "../../lib/malloc_utils.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int main(const int argc, const char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Expected int N count of pairs\n");
    return -1;
  }

  const int N = atoi(argv[1]);
  if (N <= 0) {
    fprintf(stderr, "N must be in range (0, %d]\n", INT_MAX);
    return -1;
  }

  int **graph = (int **)malloc2d(N, N, sizeof(int));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      graph[i][j] = 0;

      if (i == j) {
        graph[i][j] = 1;
      }
    }
  }

  int p, q;
  while (scanf("%d %d", &p, &q) == 2) {
    graph[p][q] = (graph[q][p] = 1);
  }

  for (int i = 0; i < N; i++) {
    for (int j = i + 1; j < N; j++) {
      if (graph[i][j] == 0) {
        continue;
      }

      printf("%d %d\n", i, j);
    }
  }

  free2d((void **)graph, N);

  return 0;
}
