#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(const int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "N, M expected");
    return 1;
  }

  const uint16_t N = atoi(argv[1]);
  const uint16_t M = atoi(argv[2]);

  uint64_t f1 = 1;
  uint64_t f2 = 1;
  for (int i = 2; i < N; i++) {
    int tmp = (f1 + f2) % M;
    f1 = f2;
    f2 = tmp;
  }

  printf("F(%u) mod %u = %llu\n", N, M, f2);

  return 0;
}
