#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX(a, b) (a > b ? a : b)

// arr[l, r];
int get_max_element(const int *arr, size_t l, size_t r) {
  if (l == r) {
    return MAX(arr[l], arr[r]);
  }

  size_t m = (l + r) >> 1;
  const int lmax = get_max_element(arr, l, m);
  const int rmax = get_max_element(arr, m + 1, r);

  return MAX(lmax, rmax);
}

int main(const int argc, char *argv[]) {
  const int vals[] = {
      1025, 1, 12, 1000, 122, -122, 1024,
  };
  printf("Max: %d\n", get_max_element(vals, 0, 7));

  return 0;
}
