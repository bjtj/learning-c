#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <process.h>
#include "queue.h"

typedef struct _shared_data_t
{
    HANDLE sem;
    queue_t* queue;
} shared_data_t;

UINT WINAPI consumer(void* arg)
{
    shared_data_t* data = (shared_data_t*)arg;
    while (1) {
        int out = 0;
        WaitForSingleObject(data->sem, INFINITE);
        if (data->queue->size > 0) {
            int num = queue_pop(data->queue);
            printf("num: %d\n", num);
            if (num == -1) {
                out = 1;
            }
        }
        ReleaseSemaphore(data->sem, 1, NULL);
        if (out) {
            break;
        }
    }
    return 0;
}

UINT WINAPI producer(void* arg)
{
    int i;
    shared_data_t* data = (shared_data_t*)arg;
    for (i = 0; i < 50; ++i) {
        WaitForSingleObject(data->sem, INFINITE);
        queue_push(data->queue, i);
        ReleaseSemaphore(data->sem, 1, NULL);
        Sleep(10);
    }
    WaitForSingleObject(data->sem, INFINITE);
    queue_push(data->queue, -1);
    ReleaseSemaphore(data->sem, 1, NULL);
    return 0;
}

shared_data_t* shared_data_new(void)
{
    shared_data_t* data = (shared_data_t*)malloc(sizeof(shared_data_t));
    assert(data != NULL);
    memset(data, 0, sizeof(shared_data_t));
    data->sem = CreateSemaphore(NULL, 1, 1, NULL);    
    data->queue = queue_new();
    return data;
}

int main()
{
    HANDLE consumer_pid;
    HANDLE producer_pid;

    shared_data_t* shared_data = shared_data_new();

    consumer_pid = _beginthreadex(NULL, 0, consumer, shared_data, 0, NULL);
    producer_pid = _beginthreadex(NULL, 0, producer, shared_data, 0, NULL);

    WaitForSingleObject(consumer_pid, INFINITE);
    WaitForSingleObject(producer_pid, INFINITE);
        
    CloseHandle(shared_data->sem);    
    queue_free(shared_data->queue);
    free(shared_data);

    return 0;
}
