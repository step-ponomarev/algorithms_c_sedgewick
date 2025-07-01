#include "../../collections/stack.h"
#include "../../lib/calc/calc.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int main(const int argc, const char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Expected expression\n");
    return -1;
  }

  const char *expression = argv[1];
  Stack *stack = stack_create(sizeof(double));

  Token token;
  int read_len;

  while ((token = read_token(expression, &read_len)).type != END) {
    if (token.type == ERR) {
      fprintf(stderr, "Unexpected token '%c'", token.op);
      return -1;
    }
    expression += read_len;

    if (token.type == DIGIT) {
      stack_push(stack, &token.digit);
      continue;
    }

    if (token.type == OP) {
      double second_digit;
      stack_pop(stack, &second_digit);

      Token res;
      if (token.op == '$') {
        res = calc(second_digit, 0.0, token.op);
        stack_push(stack, &res.digit);
        continue;
      }

      double first_digit;
      stack_pop(stack, &first_digit);

      res = calc(first_digit, second_digit, token.op);
      if (res.type == ERR) {
        fprintf(stderr, "Unexpected op '%c'", res.op);
        return -1;
      }

      stack_push(stack, &res.digit);
    }
  }

  double res;
  stack_pop(stack, &res);
  printf("Result: %f\n", res);

  stack_destroy(stack);

  return 0;
}
