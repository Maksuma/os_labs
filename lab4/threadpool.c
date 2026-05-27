/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 4

#define TRUE 1

// this represents work that has to be
// completed by a thread in the pool
typedef struct
{
    void (*function)(void *p);
    void *data;
} task;

// circular array queue
static task queue[QUEUE_SIZE];
static int head = 0;
static int tail = 0;
static int queue_count = 0;

// the worker bees
static pthread_t bees[NUMBER_OF_THREADS];

// mutex protects the queue from race conditions
static pthread_mutex_t mutex;

// semaphore signals workers that work is available;
// sem_wait() is a cancellation point, enabling pool_shutdown()
static sem_t sem;

// insert a task into the queue
// returns 0 if successful, 1 if queue is full
int enqueue(task t)
{
    if (queue_count == QUEUE_SIZE)
        return 1;

    queue[tail] = t;
    tail = (tail + 1) % QUEUE_SIZE;
    queue_count++;
    return 0;
}

// remove a task from the front of the queue
task dequeue()
{
    task t = queue[head];
    head = (head + 1) % QUEUE_SIZE;
    queue_count--;
    return t;
}

// each worker thread loops: waits for work, dequeues, executes
void *worker(void *param)
{
    while (TRUE) {
        // block until work is available; this is a cancellation point
        sem_wait(&sem);

        pthread_mutex_lock(&mutex);
        task work = dequeue();
        pthread_mutex_unlock(&mutex);

        execute(work.function, work.data);
    }

    pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool.
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

/**
 * Submits work to the pool.
 * Returns 0 on success, 1 if the queue is full.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
    task work;
    work.function = somefunction;
    work.data = p;

    pthread_mutex_lock(&mutex);
    int result = enqueue(work);
    pthread_mutex_unlock(&mutex);

    if (result == 0)
        sem_post(&sem);  // wake up one waiting worker

    return result;
}

// initialize the thread pool: mutex, semaphore, and worker threads
void pool_init(void)
{
    pthread_mutex_init(&mutex, NULL);
    sem_init(&sem, 0, 0);

    for (int i = 0; i < NUMBER_OF_THREADS; i++)
        pthread_create(&bees[i], NULL, worker, NULL);
}

// shutdown the thread pool: cancel workers, then wait for them to finish
void pool_shutdown(void)
{
    for (int i = 0; i < NUMBER_OF_THREADS; i++)
        pthread_cancel(bees[i]);

    for (int i = 0; i < NUMBER_OF_THREADS; i++)
        pthread_join(bees[i], NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&sem);
}
