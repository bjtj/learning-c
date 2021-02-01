#ifndef QUEUE_H
#define QUEUE_H

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


queue_t * queue_new(void);
queue_node_t * queue_node_new(int data);
void queue_push(queue_t * queue, int data);
int queue_pop(queue_t * queue);
void queue_free(queue_t * queue);

#endif /* QUEUE_H */
