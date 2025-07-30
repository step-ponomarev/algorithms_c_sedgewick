#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int calcC(int *arr, int N) {
  if (arr[N] > 0) {
    return arr[N];
  }

  int cN = 0;
  for (int k = 1; k < N; k++) {
    cN += arr[k - 1] + (N - k) < k ? arr[N - k] : calcC(arr, N - k);
  }

  return arr[N] = N + (cN / N);
}

int main(const int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Expected 1 <= N\n");
    return 1;
  }

  const int N = atoi(argv[1]);
  if (N < 1) {
    fprintf(stderr, "Expected 1 <= N\n");
    return 1;
  }

  int C[N + 1];
  C[0] = 1;
  for (int i = 1; i < N + 1; i++) {
    C[i] = 0;
  }

  const int Cn = calcC(C, N);
  fprintf(stdout, "C[%d] = %d\n", N, Cn);

  return 0;
}
