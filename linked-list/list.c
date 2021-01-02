#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void print_list(node_t * ptr)
{
    int i;
    for (i = 0; ptr; i++) {
	printf("[%d] data: %d\n", i, ptr->data);
	ptr = ptr->next;
    }
}

node_t * create_node(int data)
{
    node_t * node1 = malloc(sizeof(node_t));
    if (node1 == NULL) {
	fprintf(stderr, "malloc() failed");
	exit(1);
    }
    node1->data = data;
    node1->next = NULL;
    return node1;
}

node_t * get_tail(node_t * ptr)
{
    while (ptr) {
	if (!ptr->next) {
	    return ptr;
	}
	ptr = ptr->next;
    }
    return NULL;
}

node_t * append_node(node_t * ptr, node_t * node)
{
    if (ptr == NULL) {
	return node;
    }
    node_t * tail = get_tail(ptr);
    if (tail == NULL) {
	return NULL;
    }
    tail->next = node;
    return ptr;
}

node_t * remove_node(node_t * ptr, node_t * node)
{
    node_t * temp_node = ptr;

    if (ptr == NULL) {
	return NULL;
    }

    if (ptr == node) {
	node_t * next = ptr->next;
	free(ptr);
	return next;
    }
    
    for (; temp_node; temp_node = temp_node->next) {
	if (temp_node->next == node) {
	    temp_node->next = node->next;
	    free(node);
	    break;
	}
    }
    return ptr;
}

void destroy_list(node_t * ptr)
{
    while (ptr) {
	node_t * next = ptr->next;
	free(ptr);
	ptr = next;
    }
}

node_t * get_node_by_index(node_t * ptr, int index)
{
    int i = 0;
    for (;ptr && i != index; i++, ptr = ptr->next) {
    }
    return ptr;
}

node_t * insert_node(node_t * ptr, int idx, node_t * node)
{
    if (idx == 0) {
	node->next = ptr;
	return node;
    }
    if (ptr == NULL) {
	return NULL;
    }
    node_t * find = get_node_by_index(ptr, idx - 1);
    if (find == NULL) {
	return NULL;
    }
    node->next = find->next;
    find->next = node;
    return ptr;
}

int list_length(node_t * ptr)
{
    int i;
    for (i = 0; ptr; ++i, ptr = ptr->next) {
    }
    return i;
}

node_t * find_node(node_t * ptr, int match)
{
    while (ptr) {
	if (ptr->data == match) {
	    return ptr;
	}
	ptr = ptr->next;
    }
    return NULL;
}
