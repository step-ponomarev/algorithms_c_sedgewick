#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: +- works
#define IS_DIGIT(a) (a >= '0' && a <= '9')

int calc_infix(char *expression, size_t *I) {
  while (expression[*I] == ' ') {
    (*I)++;
  }

  if (expression[*I] == '(') {
    (*I)++;
    int res = calc_infix(expression, I);
    if (expression[*I] != ')') {
      fprintf(stdout, "Expected ')', but got '%c'", expression[*I]);
      exit(1);
    }
    (*I)++;

    return res;
  }

  int d1 = 0;
  while (IS_DIGIT(expression[*I])) {
    d1 = expression[*I] - '0' + d1 * 10;
    (*I)++;
  }

  while (expression[*I] == ' ') {
    (*I)++;
  }

  if (expression[*I] == ')') {
    return d1;
  }

  char op = expression[*I];
  (*I)++;

  const int d2 = calc_infix(expression, I);
  switch (op) {
  case '-':
    return d1 - d1;
  case '+':
    return d1 + d2;
  case '*':
    return d1 * d2;
  case '/':
    return d1 / d2;
  }

  fprintf(stderr, "Unexpected operation: '%c'\n", op);
  exit(1);
}

int main(const int argc, char *argv[]) {
  char expression[] = "((11 + 19) * (12 - 1))";
  size_t I = 0;

  printf("res: %d", calc_infix(expression, &I));

  return 0;
}
