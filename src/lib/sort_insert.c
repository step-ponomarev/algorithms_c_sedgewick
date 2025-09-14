#include "sort.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void sort(void *arr, uint32_t l, uint32_t r, Comparator *comparator,
          size_t entry_size) {

  uint8_t *base = arr;
  void *curr_elem = NULL;
  void *min_ptr = NULL;
  while (l < r) {
    curr_elem = &base[l * entry_size];
    min_ptr = curr_elem;

    for (uint32_t l_tmp = l; l_tmp < r; l_tmp++) {
      void *curr = NULL;
      if ((*comparator)(curr = &base[l_tmp * entry_size], min_ptr) >= 0) {
        continue;
      }

      min_ptr = curr;
    }

    uint8_t tmp[entry_size];
    memcpy(tmp, min_ptr, entry_size);
    memcpy(min_ptr, curr_elem, entry_size);
    memcpy(curr_elem, tmp, entry_size);
    l++;
  }
}
