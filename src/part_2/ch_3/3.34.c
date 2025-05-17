#include "../../lib/list.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_VAL 1000000
#define max(a, b) (a > b ? a : b)
#define max_link(l1, l2) (get_item(l1) > get_item(l2) ? l1 : l2)

extern int random_int(int, int);

int main(const int argc, const char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Pass one int arg");
    return -1;
  }

  const int N = atoi(argv[1]);
  int expected_last_element = random_int(0, MAX_VAL);
  link head = create(expected_last_element);

  for (int i = 1; i < N; i++) {
    int next = random_int(0, MAX_VAL);
    expected_last_element = max(expected_last_element, next);
    insert_next(head, next);
  }

  link max_item = head;
  while (next(head) != NULL) {
    max_item = max_link(max_item, head);
    head = next(head);
  }

  delete_curr(max_item);
  insert_next(head, get_item(max_item));

  printf("Expected %d, got: %d", expected_last_element, get_item(next(head)));

  return 0;
}
