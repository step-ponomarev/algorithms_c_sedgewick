#include "calc.h"
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

#define MAX_DIGIT_CHAR_LEN 120

const static char *UNSUPPORTED_OP_ERROR = "Unsupported operation";
const static char *UNSUPPORTED_TOKEN_ERROR = "Unsupported token";
const static char *UNHANLDED_OP_ERROR = "Unhandled operation";

Token calc(const double a, const double b, const char op) {
  if (!is_op(op)) {
    return (Token){.error = UNSUPPORTED_OP_ERROR, .type = ERR};
  }

  double res;
  switch (op) {
  case '-':
    res = a - b;
    break;
  case '+':
    res = a + b;
    break;
  case '*':
    res = a * b;
    break;
  case '/':
    res = a / b;
    break;
  case '$':
    res = sqrt(a);
    break;

    return (Token){.error = UNHANLDED_OP_ERROR, .type = ERR};
  }

  return (Token){.digit = res, .type = DIGIT};
}

Token read_token(const char *src, int *read_len) {
  *read_len = 0;

  int ch;
  do {
    ch = *(src++);
    (*read_len)++;
  } while (ch != '\0' && isspace(ch));

  if (ch == '\0') {
    return (Token){.type = END};
  }

  if (is_digit(ch)) {
    char digit[MAX_DIGIT_CHAR_LEN];
    digit[0] = ch;
    read_digit(src, digit + 1, read_len);

    return (Token){.digit = atof(digit), .type = DIGIT};
  }

  if (ch == '-' && is_digit(*src)) {
    char digit[MAX_DIGIT_CHAR_LEN];
    digit[0] = ch;
    read_digit(src, digit + 1, read_len);

    return (Token){.digit = atof(digit), .type = DIGIT};
  }

  if (is_op(ch)) {
    return (Token){.op = ch, .type = OP};
  }

  if (is_semi(ch)) {
    return (Token){.op = ch, .type = SEMI};
  }

  return (Token){.error = UNSUPPORTED_OP_ERROR, .type = ERR};
}

char is_semi(const char ch) { return ch == '(' || ch == ')'; }

char is_digit(const char ch) { return (ch >= '0' && ch <= '9') || ch == '.'; }

char is_op(const char ch) {
  return is_unary_op(ch) || ch == '+' || ch == '/' || ch == '*';
}

char is_unary_op(const char ch) { return ch == '-' || ch == '$'; }

void read_digit(const char *src, char *dest, int *read_len) {
  int ch;
  do {
    ch = *(src++);

    if (!is_digit(ch)) {
      break;
    }

    *(dest++) = ch;
    (*read_len)++;
  } while (ch != '\0');

  *dest = '\0';
}
