#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_THREADS  10

struct ThreadArgs
{
    pthread_t tid;
    int id;
    int count;
};

void*
thread_func(void * arg)
{
    struct ThreadArgs* args = (struct ThreadArgs*)arg;
    int i;
    for(i =0; i <= args->count;i++)
    {
        printf("Thread id: %-4d,Count Value: %-5d\n", args->id, i);
    }
    return NULL;
}

int
main(int argc, char const *argv[])
{
    struct ThreadArgs thread_info[MAX_THREADS];
    int i, ret_val;
    for(i=0; i < MAX_THREADS;i++)
    {
        thread_info[i].id = i+1;
        thread_info[i].count = 10-i;
        ret_val = pthread_create(&thread_info[i].tid, NULL, thread_func, (void *)&thread_info[i]);
        if(ret_val < 0)
        {
            perror("Error creating thread..");
            return -2;
        }
    }
    
    for(i = 0; i < MAX_THREADS; i++)
    {
        ret_val = pthread_join(thread_info[i].tid, NULL);
        if(ret_val)
        {
            perror("Error joining thread: ");
            return -3;
        }
    }
    printf("Main Has Joined All Threads\n");
    return 0;
}
