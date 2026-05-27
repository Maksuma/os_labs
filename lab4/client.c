/**
 * Example client program that uses thread pool.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h> 
#include "threadpool.h"

struct data
{
    int a;
    int b;
};

void add(void *param)
{
    struct data *temp;
    temp = (struct data*)param;

    printf("I add two values %d and %d result = %d\n",temp->a, temp->b, temp->a + temp->b);
}

int main(void)
{
 
    // initialize the thread pool
    pool_init();

    // submit the work to the queue
    // create some work to do
    srand(time(NULL));
    for (int i = 0; i < 10; i++){
        struct data *work = malloc(sizeof(struct data));  
        work->a = rand() % 100;
        work->b = rand() % 100;
        pool_submit(&add, work);
    }

    // may be helpful 
    sleep(3);

    pool_shutdown();

    return 0;
}
