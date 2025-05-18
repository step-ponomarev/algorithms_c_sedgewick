#ifndef IO_UTILS_H
#define IO_UTILS_H

#include <stdio.h>

typedef struct str {
  char *str;
  size_t len;
} String;

String read_line(FILE *);

#endif
