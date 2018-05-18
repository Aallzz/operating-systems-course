#ifndef MY_LIST
#define MY_LIST

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct list_node;
typedef struct list_node list_node;

struct list_node {
  char* path;
  list_node *next; 
};

typedef struct list {
  list_node *data;
} list;

void push_front_list(list* lst, const char* str);
void pop_front_list(list* lst);

#define filter_list(lst, arg, predicate)  \
  (\
    {\
      list* new_list = make_list(); \
      list_node* cur_node = lst->data; \
      while (cur_node) { \
        if (predicate(cur_node->path, arg)) { \
          push_front_list(new_list, cur_node->path); \
        } \
        cur_node = cur_node->next; \
      } \
      swap_lists(lst, new_list); \
      delete_list(new_list); \
    } \
  )

list* make_list();
void delete_list(list* lst);

void print_list(const list* lst, FILE* out);
void swap_lists(list* l1, list*l2);
#endif 
