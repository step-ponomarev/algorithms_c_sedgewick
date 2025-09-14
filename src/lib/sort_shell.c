#include "sort.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void swap(void *a, void *b, size_t entry_size);

static const uint8_t STEP = 3;

void sort(void *arr, uint32_t l, uint32_t r, Comparator *comparator,
          size_t entry_size) {
  int8_t *base = (int8_t *)arr;
  int32_t h = 1;
  // fing max gap
  for (; h < (r - l) / 9; h = STEP * h + 1)
    ;

  for (; h > 0; h /= STEP) {
    for (uint32_t i = l + h; i < r; i++) {
      int8_t tmp[entry_size];
      memcpy(&tmp, &base[i * entry_size], entry_size);

      uint32_t j = i;
      for (; j >= l + h && (*comparator)(&tmp, &base[(j - h) * entry_size]) < 0;
           j -= h) {
        memcpy(&base[j * entry_size], &base[(j - h) * entry_size], entry_size);
      }
      memcpy(&base[j * entry_size], &tmp, entry_size);
    }
  }
}

void swap(void *a, void *b, size_t entry_size) {
  if (a == b) {
    return;
  }

  uint8_t tmp[entry_size];
  memcpy(tmp, a, entry_size);
  memcpy(a, b, entry_size);
  memcpy(b, tmp, entry_size);
}
