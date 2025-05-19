#ifndef MULTI_LIST_H_
#define MULTI_LIST_H_

#ifndef MULTI_ITEM_TYPE
#define MULTI_ITEM_TYPE int
#endif

typedef struct multi_node multi_node;
typedef struct multi_list multi_list;

struct multi_node {
  MULTI_ITEM_TYPE item;
  multi_node *right;
  multi_node *bottom;
};

struct multi_list {
  multi_node **rows;
  int row_count;
  multi_node **cols;
  int col_count;
};

multi_list multi_list_create(const MULTI_ITEM_TYPE **, int, int);
void multi_list_free(multi_list);

#endif
