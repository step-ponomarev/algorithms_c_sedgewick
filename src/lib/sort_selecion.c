#include "sort.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void swap(void *a, void *b, size_t entry_size);

void sort(void *arr, uint32_t l, uint32_t r, Comparator *comparator,
          size_t entry_size) {
  uint8_t *base = arr;
  uint8_t *min_ptr = &base[l * entry_size];
  uint32_t curr = r;

  // min to first pos
  for (; curr > l; curr--) {
    uint8_t *curr_ptr = &base[curr * entry_size];
    if ((*comparator)(curr_ptr, min_ptr) >= 0) {
      continue;
    }
    min_ptr = curr_ptr;
  }
  swap(&base[l * entry_size], min_ptr, entry_size);

  curr = l + 1;
  while (curr < r) {
    uint32_t curr_tmp = curr, saved_pos = curr;

    uint8_t buff[entry_size];
    memcpy(&buff, &base[curr * entry_size], entry_size);
    // decrease swap use memcpy for all elemes
    while (curr_tmp > l &&
           (*comparator)(&buff, &base[(curr_tmp - 1) * entry_size]) < 0) {
      curr_tmp--;
    }

    const uint32_t amount = saved_pos - curr_tmp;
    if (amount > 0) {
      memmove(&base[(curr_tmp + 1) * entry_size], &base[(curr_tmp)*entry_size],
              amount * entry_size);

      memmove(&base[curr_tmp * entry_size], &buff, entry_size);
    }

    curr++;
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
