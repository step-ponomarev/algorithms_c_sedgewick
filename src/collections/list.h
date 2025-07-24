#ifndef LIST_H_
#define LIST_H_

#include <stdbool.h>
#include <stddef.h>

typedef struct List List;

List *list_create(const size_t element_size);
void list_destroy(List *list);

void list_add(List *list, const void *elem);
void list_get_item(const List *list, const size_t index, void *dest);
void list_remove_item(List *list, const size_t index, void *dest);
size_t list_size(const List *list);
bool list_is_empty(const List *list);

#endif
