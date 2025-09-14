#ifndef SORT_H_
#define SORT_H_

#include "../collections/comparator.h"
#include <stddef.h>
#include <stdint.h>

// sorting in range [l,r)
void sort(void *arr, uint32_t l, uint32_t r, Comparator *comparator,
          size_t entry_size);

#endif
