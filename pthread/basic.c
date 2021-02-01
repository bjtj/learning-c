#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>


void * worker(void * arg)
{
    printf("Hello in worker\n");
    return 0;
}

void start_thread(pthread_t * pid)
{
    assert(pthread_create(pid, NULL, worker, NULL) == 0);
}

void start_thread_detached(pthread_t * pid)
{
    pthread_attr_t attr;
    assert(pthread_attr_init(&attr) == 0);
    assert(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) == 0);
    assert(pthread_create(pid, &attr, worker, NULL) == 0);
    assert(pthread_attr_destroy(&attr) == 0);
}

int main()
{
    pthread_t pid1;
    pthread_t pid2;

    start_thread(&pid1);

    start_thread_detached(&pid2);

    pthread_join(pid1, NULL);
    
    return 0;
}
