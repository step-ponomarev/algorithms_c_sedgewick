#include "../../lib/rand_utils.h"
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define CALC_K(n) (1 << ((size_t)(log2(n) - 1)))

// [l, r)
int max(int *arr, size_t l, size_t r);

int main(const int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Expected 1 arg\n");
    return 1;
  }

  const size_t N = atoi(argv[1]);
  int arr[N];
  for (size_t i = 0; i < N; i++) {
    arr[i] = random_int(0, 10);
    printf("%d ", arr[i]);
  }
  printf("\n");

  fprintf(stdout, "Max: %d\n", max(arr, 0, N));

  return 0;
}

int max(int *arr, size_t l, size_t r) {
  if (l + 1 == r) {
    return MAX(arr[l], arr[r]);
  }

  const size_t n = r - l;
  const size_t k = CALC_K(n);
  const int left_max = max(arr, l, k);
  const int right_max = max(arr, k, r);

  return MAX(left_max, right_max);
}
