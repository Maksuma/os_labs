/**
 * Priority scheduling algorithm.
 * Each iteration picks the task with the highest priority value (10 = highest).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "cpu.h"
#include "schedulers.h"

static struct node *head = NULL;

/* Returns the node containing the task with the highest priority. */
static struct node *pickHighestPriority() {
    struct node *highest = head;
    struct node *temp = head->next;
    while (temp != NULL) {
        if (temp->task->priority > highest->task->priority)
            highest = temp;
        temp = temp->next;
    }
    return highest;
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
        struct node *node = pickHighestPriority();
        Task *task = node->task;
        run(task, task->burst);
        delete(&head, task);
        free(task);
    }
}
