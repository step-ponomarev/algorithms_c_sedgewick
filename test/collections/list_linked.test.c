#include "../../src/collections/list.h"
#include <stddef.h>
#include <stdio.h>

int main() {
  const size_t max_size = 200;
  List *list = list_create(sizeof(int));

  for (int i = 0; i < max_size; i++) {
    list_add(list, &i);
  }

  int fromlist;
  for (int i = 0; i < max_size; i++) {
    list_get_item(list, i, &fromlist);
    printf("%d ", fromlist);
  }
  printf("\n");

  printf("List is empty: %d", list_is_empty(list));
  list_destroy(list);

  return 0;
}
