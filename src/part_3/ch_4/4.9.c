#include "../../collections/stack.h"
#include "../../lib/calc/calc.h"
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>

int handle_expression(const char *, Stack *, int *, const char);
void print_op(Stack *);

int main(const int argc, const char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Expected expression\n");
    return -1;
  }

  const char *expression = argv[1];
  Stack *stack = stack_create(sizeof(char));
  int read_symbol_cound = 0;

  if (handle_expression(expression, stack, &read_symbol_cound, 0) != 0) {
    return -1;
  }

  while (!stack_is_empty(stack)) {
    print_op(stack);
  }

  return 0;
}

int handle_expression(const char *expression, Stack *stack, int *read_len,
                      const char invert_digits) {
  const char *start_ptr = expression;

  Token token;
  int len;

  char op_added = 0;
  while ((token = read_token(expression, &len)).type != END) {
    if (token.type == ERR) {
      fprintf(stderr, "Unexpected error\n");
      return -1;
    }

    expression += len;
    if (token.type == DIGIT) {
      printf("%f ", !invert_digits ? token.digit : token.digit * -1);
      continue;
    }

    if (token.type == SEMI) {
      // nested expretssion
      if (token.op == '(') {
        handle_expression(expression, stack, &len, invert_digits);
        expression += len;
        continue;
      }

      // end expression
      *read_len = expression - start_ptr;
      for (int i = 0; i < op_added; i++) {
        print_op(stack);
      }

      return 0;
    }

    if (token.type == OP && is_unary_op(token.op) && (*expression == '(')) {
      handle_expression(++expression, stack, &len, token.op == '-');
      expression += len;

      if (token.op == '$') {
        printf("%c ", token.op);
      }
      continue;
    }

    stack_push(stack, &token.op);
    op_added++;
  }

  *read_len = expression - start_ptr;
  return 0;
}

void print_op(Stack *stack) {
  if (stack_is_empty(stack)) {
    return;
  }

  char op;
  stack_pop(stack, &op);
  printf("%c ", op);
}
