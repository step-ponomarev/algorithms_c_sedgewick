#ifndef MALLOC_UTILS_H_
#define MALLOC_UTILS_H_

#include <stddef.h>

void **malloc2d(const size_t, const size_t, const size_t);
void free2d(void **, const size_t);
void ***malloc3d(const size_t, const size_t, const size_t, const size_t);
void free3d(void ***, const size_t, const size_t);

#endif
