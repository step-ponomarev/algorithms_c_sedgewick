#include "sort.h"
#include <string.h>

// [l, r)
uint32_t partition(uint8_t *arr, uint32_t l, uint32_t r, Comparator *comparator,
                   const size_t enrty_size);

// [l, r)
void sort(void *arr, uint32_t l, uint32_t r, Comparator *comparator,
          size_t entry_size) {
  if (r - l <= 1) {
    return;
  }

  uint8_t *base = arr;

  uint32_t i = partition(base, l, r, comparator, entry_size);
  sort(base, l, i, comparator, entry_size);
  sort(base, i + 1, r, comparator, entry_size);
}

void swap(uint8_t *a, uint8_t *b, size_t entry_size);

uint32_t partition(uint8_t *arr, uint32_t l, uint32_t r, Comparator *comparator,
                   const size_t enrty_size) {
  if (r - l == 2) {
    if ((*comparator)(&arr[l * enrty_size], &arr[(r - 1) * enrty_size]) > 0) {
      swap(&arr[l * enrty_size], &arr[(r - 1) * enrty_size], enrty_size);
    }

    return r - 1;
  }

  uint8_t partitioner[enrty_size];
  memcpy(partitioner, &arr[(r - 1) * enrty_size], enrty_size);

  uint32_t i = l;
  uint32_t j = r - 1;

  while (1) {
    while (i < j && (*comparator)(&arr[i * enrty_size], partitioner) < 0) {
      i++;
    }

    while (j > i && (*comparator)(&arr[j * enrty_size], partitioner) >= 0) {
      j--;
    }

    if (j == i) {
      break;
    }

    swap(&arr[(i++) * enrty_size], &arr[j * enrty_size], enrty_size);
  }

  swap(&arr[(r - 1) * enrty_size], &arr[j * enrty_size], enrty_size);

  return j;
}

void swap(uint8_t *a, uint8_t *b, size_t entry_size) {
  uint8_t tmp[entry_size];

  memcpy(tmp, a, entry_size);
  memcpy(a, b, entry_size);
  memcpy(b, tmp, entry_size);
}
