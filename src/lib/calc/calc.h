typedef enum TokenType {
  OP = 1,
  DIGIT = 2,
  SEMI = 3,
  ERR = -1,
  END = -2
} TokenType;

typedef struct Token {
  union {
    char op;
    double digit;
    const char *error;
  };
  TokenType type;
} Token;

Token read_token(const char *, int *);
char is_op(const char);
char is_unary_op(const char);
char is_semi(const char);
char is_digit(const char);
void read_digit(const char *, char *, int *);
Token calc(const double, const double, const char);
