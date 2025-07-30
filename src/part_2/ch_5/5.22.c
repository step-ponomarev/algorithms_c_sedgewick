#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void rule(int *arr, int l, int r, int h);

int main(const int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "N and i expected");
    return 1;
  }

  const uint16_t N = atoi(argv[1]);
  const int size = (1 << N) - 1;
  int marks[size];
  rule(marks, 0, size - 1, N);

  const uint16_t i = atoi(argv[2]);
  printf("Res: %d\n", marks[i]);

  return 0;
}

void rule(int *arr, int l, int r, int h) {
  if (h <= 0) {
    return;
  }

  const int m = (l + r) >> 1;
  rule(arr, l, m, h - 1);
  arr[m] = h;
  rule(arr, m, r, h - 1);
}
