#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint64_t get_remainder(uint16_t n) {
  if (n <= 1) {
    return 1;
  }

  return n * get_remainder(n - 1);
}

int main(const int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "N expected\n");
    return 1;
  }

  const uint64_t res = get_remainder(atoi(argv[1]));
  fprintf(stdout, "Result: %llu\n", res);

  return 0;
}
