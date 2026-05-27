/**
 * Shortest-Job-First (SJF) scheduling algorithm.
 * Each iteration picks the task with the smallest CPU burst.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "cpu.h"
#include "schedulers.h"

static struct node *head = NULL;

/* Returns the node containing the task with the smallest burst. */
static struct node *pickShortest() {
    struct node *shortest = head;
    struct node *temp = head->next;
    while (temp != NULL) {
        if (temp->task->burst < shortest->task->burst)
            shortest = temp;
        temp = temp->next;
    }
    return shortest;
}

void add(char *name, int priority, int burst) {
    Task *task = malloc(sizeof(Task));
    task->name     = name;
    task->tid      = 0;
    task->priority = priority;
    task->burst    = burst;
    insert(&head, task);
}

void schedule() {
    while (head != NULL) {
        struct node *node = pickShortest();
        Task *task = node->task;
        run(task, task->burst);
        delete(&head, task);
        free(task);
    }
}
