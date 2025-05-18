#ifndef LIST_H_
#define LIST_H_

#ifndef ITEM_T
#define ITEM_T int // Значение по умолчанию
#endif

typedef ITEM_T item;
typedef struct node node;
typedef node *link;

link create(const item);
void free_list(link);
void free_node(link);
void insert_next(link, const item);
link delete_curr(link);
link delete_next(link);
link next(const link);
item get_item(const link);
void swap(link, link);

#endif
