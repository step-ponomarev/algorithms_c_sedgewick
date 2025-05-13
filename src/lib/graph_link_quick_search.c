#include "graph_link.h"

int find_root(const int graph[MAX_VAL], const int v) { return graph[v]; }

void merge_unions(int graph[MAX_VAL], const int p, const int q) {
  const int tmp = graph[p];
  for (int i = 0; i < MAX_VAL; i++) {
    if (graph[i] != tmp) {
      continue;
    }

    graph[i] = graph[q];
  }
};
