#ifndef LIST_H
#define LIST_H

typedef struct _node_t {
    int data;
    struct _node_t * next;
} node_t;

void print_list(node_t * ptr);
node_t * create_node(int data);
node_t * get_tail(node_t * ptr);
node_t * append_node(node_t * ptr, node_t * node);
node_t * remove_node(node_t * ptr, node_t * node);
void destroy_list(node_t * ptr);
node_t * get_node_by_index(node_t * ptr, int index);
node_t * insert_node(node_t * ptr, int idx, node_t * node);
int list_length(node_t * ptr);
node_t * find_node(node_t * ptr, int match);

#endif /* LIST_H */
