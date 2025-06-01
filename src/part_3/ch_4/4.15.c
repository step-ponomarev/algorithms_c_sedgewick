#include "../../collections/stack.h"
#include "../../lib/calc/calc.h"

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIMPLE_EXPRESSION_TEMPLATE "%f %c %f"
#define EXPRESSION_WITH_DIGIT_TEMPLATE "(%s) %c %f"
#define COMPOSITE_EXPRESSION_TEMPLATE "(%s) %c (%s)"
#define RES_SIZE 120

char *create_simple_expression(Stack *, const char);
char *merge_with_digit(const char *, const double, const char);
char *merge_expressions(const char *, const char *, const char);

int main(const int argc, const char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Expected expression\n");
    return -1;
  }

  char res[RES_SIZE];
  int rp = 0;

  const char *expression = argv[1];
  Stack *digits = stack_create(sizeof(double));

  char *expressions[2];
  expressions[0] = NULL;
  expressions[1] = NULL;

  Token token;
  int read_len;
  while ((token = read_token(expression, &read_len)).type != END) {
    if (token.type == ERR) {
      fprintf(stderr, "Unexpected token '%c'", token.op);
      return -1;
    }
    expression += read_len;

    if (token.type != DIGIT && token.type != OP) {
      fprintf(stderr, "Unsupported token type: %d\n", token.type);
      return -1;
    }

    if (token.type == DIGIT) {
      stack_push(digits, &token.digit);
      continue;
    }

    const size_t digit_count = stack_size(digits);
    if (token.type == OP) {
      if (expressions[0] == NULL) {
        expressions[0] = create_simple_expression(digits, token.op);
        continue;
      }

      if (digit_count == 1) {
        double digit;
        stack_pop(digits, &digit);

        char *merged = merge_with_digit(expressions[0], digit, token.op);
        free(expressions[0]);
        expressions[0] = merged;
        continue;
      }

      if (expressions[1] == NULL) {
        expressions[1] = create_simple_expression(digits, token.op);
        continue;
      }

      char *merged =
          merge_expressions(expressions[0], expressions[1], token.op);
      free(expressions[0]);
      free(expressions[1]);

      expressions[0] = merged;
      expressions[1] = NULL;
    }
  }

  printf("%s\n", expressions[0]);
  free(expressions[0]);

  return 0;
}

char *create_simple_expression(Stack *digits, const char op) {
  if (stack_size(digits) < 2) {
    fprintf(stderr, "Unexpected stack size %d, expected min 2\n",
            stack_size(digits));
    exit(-1);
    return NULL;
  }

  char *expression = malloc(sizeof(char) * RES_SIZE);
  double second;
  stack_pop(digits, &second);

  double first;
  stack_pop(digits, &first);

  sprintf(expression, SIMPLE_EXPRESSION_TEMPLATE, first, op, second);

  return expression;
}

char *merge_with_digit(const char *e, const double digit, const char op) {
  if (e == NULL) {
    fprintf(stderr, "Expression is null\n");
    exit(-1);
    return NULL;
  }

  char *merge = malloc(sizeof(char) * (strlen(e) + RES_SIZE));
  sprintf(merge, EXPRESSION_WITH_DIGIT_TEMPLATE, e, op, digit);

  return merge;
}

char *merge_expressions(const char *e1, const char *e2, const char op) {
  if (e1 == NULL || e2 == NULL) {
    fprintf(stderr, "Argument is null\n");
    exit(-1);
    return NULL;
  }

  char *merge = malloc(sizeof(char) * (strlen(e1) + strlen(e2) + 10));
  sprintf(merge, COMPOSITE_EXPRESSION_TEMPLATE, e1, op, e2);

  return merge;
}
