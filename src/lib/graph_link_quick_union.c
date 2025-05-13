#include "graph_link.h"
#include <stdio.h>

int find_root(const int graph[MAX_VAL], const int v) {
  int i;
  for (i = graph[v]; i != graph[i]; i = graph[i])
    ;
  return i;
}

void merge_unions(int graph[MAX_VAL], const int p, const int q) {
  graph[p] = q;
}
