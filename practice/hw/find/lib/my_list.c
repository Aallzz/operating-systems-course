#include "my_list.h"

list* make_list() {
  list* cur;
  cur = (list*)malloc(sizeof(list));
  cur->data = NULL;
  return cur;
}

void delete_list(list* lst) {
  if (lst == NULL) {
    return ;
  }
  while (lst->data) {
    list_node *cur_node = lst->data;
    lst->data = lst->data->next;
    free(cur_node->path);
    free(cur_node);
  }
  free(lst);
}

void push_front_list(list* lst, const char* str) {  
  if (lst == NULL) {
    lst = make_list();
  }
  list_node *nnode = (list_node*)malloc(sizeof(list_node));
  nnode->path = strdup(str);
  nnode->next = lst->data;
  lst->data = nnode;
}

void pop_front_list(list *lst) {
  if (lst == NULL || lst->data == NULL) {
    return ;
  }
  list_node *dnode = lst->data;
  lst->data = dnode->next;
  free(dnode->path);
  free(dnode);
}

void print_list(const list* lst, FILE* out) {
  if (lst == NULL) {
    fprintf(out, "List doesn't exist");
    return ;
  }
  if (lst->data == NULL) {
    fprintf(out, "List is empty");
    return ;
  }
  list_node *cur_node = lst->data;
  int pos = 0;
  while (cur_node) {
    fprintf(out, "%d: %s\n", pos, cur_node->path);
    cur_node = cur_node->next;
    pos++;
  }
}

void swap_lists(list *l1, list* l2) {
  list_node* n3;
  n3 = l1->data;
  l1->data  = l2->data;
  l2->data = n3;
}

