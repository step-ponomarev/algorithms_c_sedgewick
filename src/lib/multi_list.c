#include "multi_list.h"
#include "malloc_utils.h"
#include <stddef.h>
#include <stdlib.h>

multi_node **create_vector(const size_t);
void free_multi_node(multi_node *);

multi_list multi_list_create(const int **matrix, int n, int m) {
  multi_node **rows = create_vector(n);
  multi_node **cols = create_vector(m);

  multi_node *last_in_row[n];
  for (int i = 0; i < n; i++) {
    last_in_row[i] = NULL;
  }

  multi_node *last_in_col[m];
  for (int i = 0; i < m; i++) {
    last_in_col[i] = NULL;
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (matrix[i][j] == 0) {
        continue;
      }

      multi_node *curr = malloc(sizeof(multi_node));
      curr->item = matrix[i][j];

      if (rows[i] == NULL) {
        rows[i] = curr;
      }

      if (cols[j] == NULL) {
        cols[j] = curr;
      }

      if (last_in_row[i] != NULL) {
        last_in_row[i]->right = curr;
      }
      last_in_row[i] = curr;

      if (i > 0 && last_in_col[j] != NULL) {
        last_in_col[j]->bottom = curr;
      }
      last_in_col[j] = curr;
    }
  }

  return (multi_list){
      .rows = rows, .row_count = n, .cols = cols, .col_count = m};
}

multi_node **create_vector(const size_t n) {
  multi_node **v = malloc(sizeof(multi_node *) * n);
  for (int i = 0; i < n; i++) {
    v[i] = NULL;
  }

  return v;
}

void multi_list_free(multi_list ml) {
  for (int i = 0; i < ml.row_count; i++) {
    free_multi_node(ml.rows[i]);
  }

  for (int i = 0; i < ml.col_count; i++) {
    free_multi_node(ml.cols[i]);
  }
}

void free_multi_node(multi_node *node) {
  multi_node *curr = node;
  while (curr != NULL) {
    multi_node *next = node->right;
    free(curr);
    curr = next;
  }
}
