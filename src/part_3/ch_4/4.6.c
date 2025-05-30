#include "../../collections/stack.h"
#include <stdio.h>
#include <string.h>

#define STRING "EAS*Y*QUE***ST***IO*N***"

int main() {
  Stack *stack = stack_create(sizeof(char));

  for (size_t i = 0; STRING[i] != '\0'; i++) {
    if (STRING[i] == '*') {
      char ch;
      stack_pop(stack, &ch);
      printf("%c", ch);
      continue;
    }

    stack_push(stack, &STRING[i]);
  }
  printf("\n");

  stack_destroy(stack);
}
