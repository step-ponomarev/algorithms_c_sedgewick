#include "../../lib/rand_utils.h"
#include <_stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Item {
  size_t val;
  size_t size;
} Item;

size_t get_best_fit(Item *items, size_t item_count, int *best_fit,
                    size_t curr_capacity) {
  if (curr_capacity == 0) {
    return 0;
  }

  if (best_fit[curr_capacity] >= 0) {
    return best_fit[curr_capacity];
  }

  size_t max = 0;
  for (size_t i = 0; i < item_count; i++) {
    const int diff = (int)curr_capacity - (int)items[i].size;
    if (diff < 0) {
      continue;
    }

    const size_t curr_val =
        items[i].val + get_best_fit(items, item_count, best_fit, diff);
    if (max < curr_val) {
      max = curr_val;
    }
  }

  return (best_fit[curr_capacity] = max);
}

int main(const int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Expected 2 args, N = item count, C = capacity\n");
    return 1;
  }

  const size_t N = atoi(argv[1]);
  const size_t C = atoi(argv[2]);
  Item items[N];
  for (int i = 0; i < N; i++) {
    items[i] = (Item){.val = random_int(1, 101), .size = random_int(1, C + 1)};
  }

  int best_fit[C];
  for (size_t i = 0; i < C + 1; i++) {
    best_fit[i] = -1;
  }

  fprintf(stdout, "item count: %lu, capacity: %lu, best fit: %lu\n", N, C,
          get_best_fit(items, N, best_fit, C));

  return 0;
}
