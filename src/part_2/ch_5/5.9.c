#include <stdio.h>
#include <string.h>

#define IS_DIGIT(a) (a >= '0' && a <= '9')

int calc_postfix(char *expression, size_t *I) {
  while (expression[*I] == ' ') {
    (*I)--;
  }

  if (!IS_DIGIT(expression[*I])) {
    const char op = expression[(*I)--];

    const int d2 = calc_postfix(expression, I);
    const int d1 = calc_postfix(expression, I);
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
  }

  int x = 0;
  int i = 1;
  while (IS_DIGIT(expression[*I])) {
    x = (expression[(*I)--] - '0') * (1 * i) + x;
    i *= 10;
  }

  return x;
}

int main(const int argc, char *argv[]) {
  char expression[] = "1 2 + 22 * 2 /";
  size_t I = strlen(expression) - 1;

  printf("res: %d", calc_postfix(expression, &I));

  return 0;
}
