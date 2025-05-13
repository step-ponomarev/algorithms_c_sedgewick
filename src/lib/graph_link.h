#ifndef GRAPH_LINK_H_
#define GRAPH_LINK_H_

#define MAX_VAL 10

#define is_valid(v) (v >= 0 && v < MAX_VAL)

int find_root(const int[MAX_VAL], int);
int find_root_with_compression(int[MAX_VAL], int);
void merge_unions(int[MAX_VAL], const int, const int);

#endif
