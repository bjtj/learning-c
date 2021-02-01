#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include "queue.h"

typedef struct _shared_data_t
{
    sem_t sem;
    queue_t * queue;
} shared_data_t;

void * consumer(void * arg)
{
    shared_data_t * data = (shared_data_t*)arg;
    while (1) {
	int out = 0;
	sem_wait(&data->sem);
	if (data->queue->size > 0) {
	    int num = queue_pop(data->queue);
	    printf("num: %d\n", num);
	    if (num == -1) {
		out = 1;
	    }
	}
	sem_post(&data->sem);
	if (out) {
	    break;
	}
    }
    return 0;
}

void * producer(void * arg)
{
    int i;
    shared_data_t * data = (shared_data_t*)arg;
    for (i = 0; i < 50; ++i) {
	sem_wait(&data->sem);
	queue_push(data->queue, i);
	sem_post(&data->sem);
	usleep(100);
    }
    sem_wait(&data->sem);
    queue_push(data->queue, -1);
    sem_post(&data->sem);
    return 0;
}

shared_data_t * shared_data_new(void)
{
    shared_data_t * data = (shared_data_t *)malloc(sizeof(shared_data_t));
    assert(data != NULL);
    memset(data, 0, sizeof(shared_data_t));
    sem_init(&data->sem, 0, 1);
    data->queue = queue_new();
    return data;
}

int main()
{
    pthread_t consumer_pid;
    pthread_t producer_pid;

    shared_data_t * shared_data = shared_data_new();
    
    assert(pthread_create(&consumer_pid, NULL, consumer, shared_data) == 0);
    assert(pthread_create(&producer_pid, NULL, producer, shared_data) == 0);

    pthread_join(consumer_pid, NULL);
    pthread_join(producer_pid, NULL);

    sem_destroy(&shared_data->sem);
    queue_free(shared_data->queue);
    free(shared_data);
    
    return 0;
}
