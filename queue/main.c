#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef struct _queue_node_t
{
    int data;
    struct _queue_node_t * next;
} queue_node_t;

typedef struct _queue_t
{
    queue_node_t * front;
    queue_node_t * rear;
    int size;
} queue_t;


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

int main(int argc, char *argv[])
{
    queue_t * queue = queue_new();
    queue_push(queue, 1);
    queue_push(queue, 2);
    queue_push(queue, 3);
    queue_push(queue, 4);
    queue_push(queue, 5);

    assert(queue->size == 5);

    assert(queue_pop(queue) == 1);
    assert(queue_pop(queue) == 2);
    assert(queue_pop(queue) == 3);
    assert(queue_pop(queue) == 4);
    assert(queue_pop(queue) == 5);

    assert(queue->size == 0);

    queue_free(queue);
    
    return 0;
}
