#include "../../lib/malloc_utils.h"
#include "../../lib/multi_list.h"
#include "../../lib/rand_utils.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

void fill(int **, const int, const int);

int main(const int argc, const char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Expected int N and int M matrix size\n");
    return -1;
  }

  const int N = atoi(argv[1]);
  const int M = atoi(argv[2]);

  if (N <= 0 || M <= 0) {
    fprintf(stderr, "N and int M must be in range (0, %d]\n", INT_MAX);
    return -1;
  }

  int **matrix = (int **)malloc2d(N, M, sizeof(int));
  fill(matrix, N, M);

  multi_list multi_list = multi_list_create(matrix, N, M);

  for (int i = 0; i < N; i++) {
    multi_node *curr = multi_list.rows[i];
    while (curr != NULL) {
      printf("%d", curr->item);

      if ((curr = curr->right) != NULL) {
        printf(",");
      }
    }
    printf("\n");
  }

  return 0;
}

void fill(int **matrix, const int n, const int m) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      matrix[i][j] = random_int(0, 10);
    }
  }
}
