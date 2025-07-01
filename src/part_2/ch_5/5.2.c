#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint64_t get_remainder(const uint16_t n, const uint16_t m) {
  if (n >= m) {
    return 0;
  }

  if (n <= 1) {
    return 1 % m;
  }

  return (n % m) * (get_remainder(n - 1, m));
}

int main(const int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "N,M expected\n");
    return 1;
  }

  const uint16_t N = atoi(argv[1]);
  const uint16_t M = atoi(argv[2]);
  fprintf(stdout, "N: %d, M: %d\n", N, M);

  const uint64_t res = get_remainder(N, M);
  fprintf(stdout, "Result: %llu\n", res);

  return 0;
}
