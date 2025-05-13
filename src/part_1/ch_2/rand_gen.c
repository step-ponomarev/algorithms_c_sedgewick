#include <stdio.h>
#include <stdlib.h>

// [0, to)
int rand_val(int);

int main(const int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Expected arg count: %d, got %d", 2, argc);
    return -1;
  }

  const int max_val = atoi(argv[1]);
  while (1) {
    int p = rand_val(max_val);
    int q;
    while ((q = rand_val(max_val)) == p) {
      q = rand_val(max_val);
    }

    printf("%d %d\n", p, q);
  }

  return 0;
}

int rand_val(int to) { return rand() % to; }
