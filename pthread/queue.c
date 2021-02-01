#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "queue.h"


queue_t * queue_new(void)
{
    queue_t * queue = (queue_t*)malloc(sizeof(queue_t));
    memset(queue, 0, sizeof(queue_t));
    return queue;
}

queue_node_t * queue_node_new(int data)
{
    queue_node_t * node = (queue_node_t*)malloc(sizeof(queue_node_t));
    memset(node, 0, sizeof(queue_node_t));
    node->data = data;
    return node;
}

void queue_push(queue_t * queue, int data)
{
    if (queue->size == 0) {
	queue->front = queue->rear = queue_node_new(data);
    } else {
	queue_node_t * node = queue_node_new(data);
	queue->rear->next = node;
	queue->rear = node;
    }
    queue->size++;
}

int queue_pop(queue_t * queue)
{
    if (queue->size == 0) {
	return -1;
    }

    if (queue->size == 1) {
	queue_node_t * node = queue->front;
	int ret = node->data;
	free(node);
	queue->front = queue->rear = NULL;
	queue->size--;
	return ret;
    }

    queue_node_t * node = queue->front;
    int ret = node->data;
    queue->front = node->next;
    free(node);
    queue->size--;
    return ret;
}

void queue_free(queue_t * queue)
{
    queue_node_t * ptr = queue->front;
    while (ptr) {
	queue_node_t * node = ptr;
	ptr = ptr->next;
	free(node);
    }
    free(queue);
}

