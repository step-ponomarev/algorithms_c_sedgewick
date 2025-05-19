#include "malloc_utils.h"
#include <stddef.h>
#include <stdlib.h>

void **malloc2d(const size_t row, const size_t col, const size_t cell_size) {
  void **matrix = malloc(sizeof(void *) * row);
  for (int i = 0; i < row; i++) {
    matrix[i] = malloc(col * cell_size);
  }

  return matrix;
};

void free2d(void **matrix, const size_t row) {
  for (int i = 0; i < row; i++) {
    free(matrix[i]);
  }

  free(matrix);
}

void ***malloc3d(const size_t row, const size_t col, const size_t z,
                 const size_t cell_size) {
  void ***matrix = malloc(sizeof(void *) * row);
  for (int i = 0; i < row; i++) {
    matrix[i] = malloc(sizeof(void *) * col);
    for (int j = 0; i < z; j++) {
      matrix[i][j] = malloc(z * cell_size);
    }
  }

  return matrix;
};

void free3d(void ***matrix, const size_t row, const size_t col) {
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      free(matrix[i][j]);
    }
    free(matrix[i]);
  }

  free(matrix);
}
