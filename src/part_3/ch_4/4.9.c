#include "../../collections/stack.h"
#include <stddef.h>
#include <stdio.h>

int main(const int argc, const char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Expected expression\n");
    return -1;
  }

  Stack *stack = stack_create(sizeof(char));

  const char *expression = argv[1];
  size_t p = 0;
  for (size_t p = 0; expression[p] != '\0'; p++) {
    if (expression[p] == '(') {
      continue;
    }

    if (expression[p] >= '0' && expression[p] <= '9') {
      printf("%c ", expression[p]);
      continue;
    }

    if (expression[p] == ')') {
      char op;
      stack_pop(stack, &op);

      printf("%c ", op);
      continue;
    }

    stack_push(stack, &expression[p]);
  }
  printf("\n");

  stack_destroy(stack);

  return 0;
}
