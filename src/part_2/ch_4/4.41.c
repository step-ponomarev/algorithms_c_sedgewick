#include "../../collections/deque.h"
#include "string.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

bool _is_valid_ch(char ch) { return isalpha(ch) || ch == '*' || ch == '+'; }

bool _is_cap(char ch) { return ch >= 'A' && ch <= 'Z'; }

int main(const int argc, const char *argv[]) {
  Deque *deque = dequeue_create(sizeof(char));

  int ch;
  while ((ch = getchar()) != '\n') {
    if (!_is_valid_ch(ch)) {
      fprintf(stderr, "Unexpected char '%c'\n", ch);
      return -1;
    }

    if (!isalpha(ch)) {
      char res;
      ch == '*' ? dequeue_poll_last(deque, &res)
                : dequeue_poll_first(deque, &res);
      printf("%c", res);

      continue;
    }

    _is_cap(ch) ? dequeue_add_first(deque, &ch) : dequeue_add_last(deque, &ch);
  }

  dequeue_destroy(deque);

  return 0;
}
