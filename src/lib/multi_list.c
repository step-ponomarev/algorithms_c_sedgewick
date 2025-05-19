#include "multi_list.h"
#include "malloc_utils.h"
#include <stddef.h>
#include <stdio.h>
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
      curr->i = i;
      curr->j = j;

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

multi_list multi_list_multipy(const multi_list *f, const multi_list *s) {
  if (f == NULL || s == NULL || f->col_count != s->row_count) {
    return (multi_list){
        .col_count = 0, .row_count = 0, .rows = NULL, .cols = NULL};
  }

  multi_node **res_rows = create_vector(f->row_count);
  multi_node **res_cols = create_vector(s->col_count);

  multi_node *last_in_row[f->row_count];
  for (int i = 0; i < f->row_count; i++) {
    last_in_row[i] = NULL;
  }

  multi_node *last_in_col[s->col_count];
  for (int i = 0; i < s->col_count; i++) {
    last_in_col[i] = NULL;
  }

  for (int i = 0; i < f->row_count; i++) {
    for (int j = 0; j < s->col_count; j++) {
      multi_node *a_i0 = f->rows[i];
      multi_node *b_0j = s->cols[j];

      int c_ij_val = 0;
      while (a_i0 != NULL && b_0j != NULL) {
        if (a_i0->j == b_0j->i) {
          c_ij_val += a_i0->item * b_0j->item;
          a_i0 = a_i0->right;
          b_0j = b_0j->bottom;
          continue;
        }

        if (a_i0->j > b_0j->i) {
          b_0j = b_0j->bottom;
        } else {
          a_i0 = a_i0->right;
        }
      }

      if (c_ij_val == 0) {
        continue;
      }

      multi_node *c_ij = malloc(sizeof(multi_node));
      c_ij->item = c_ij_val;
      c_ij->i = i;
      c_ij->j = j;

      if (res_rows[i] == NULL) {
        res_rows[i] = c_ij;
      }

      if (res_cols[j] == NULL) {
        res_cols[j] = c_ij;
      }

      if (last_in_row[i] != NULL) {
        last_in_row[i]->right = c_ij;
      }
      last_in_row[i] = c_ij;

      if (last_in_col[j] != NULL) {
        last_in_col[j]->bottom = c_ij;
      }
      last_in_col[j] = c_ij;
    }
  }

  return (multi_list){.rows = res_rows,
                      .row_count = f->row_count,
                      .cols = res_cols,
                      .col_count = s->col_count};
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
