#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

  int P[N + 1];
  P[0] = 0;

  for (int i = 1; i <= N; i++) {
    P[i] = i / 2 + 2 * (P[i / 2]);
  }

  fprintf(stdout, "P[%d] = %d\n", N, P[N]);

  return 0;
}
