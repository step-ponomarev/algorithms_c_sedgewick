#include <stddef.h>
#include <stdlib.h>

void **malloc2d(size_t row, size_t col, size_t cell_size) {
  void **matrix = malloc(sizeof(void *) * row);
  for (int i = 0; i < row; i++) {
    matrix[i] = malloc(col * cell_size);
  }

  return matrix;
};

void ***malloc3d(size_t row, size_t col, size_t z, size_t cell_size) {
  void ***matrix = malloc(sizeof(void *) * row);
  for (int i = 0; i < row; i++) {
    matrix[i] = malloc(sizeof(void *) * col);
    for (int j = 0; i < z; j++) {
      matrix[i][j] = malloc(z * cell_size);
    }
  }

  return matrix;
};
