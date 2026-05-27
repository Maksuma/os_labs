/**
 * Priority + Round-Robin scheduling algorithm.
 * Tasks are grouped by priority (highest first).
 * Within each priority group, Round-Robin is applied with QUANTUM time slices.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "cpu.h"
#include "schedulers.h"

static struct node *head = NULL;

/* Returns the maximum priority value currently in the list. */
static int maxPriority() {
    int max = head->task->priority;
    struct node *temp = head->next;
    while (temp != NULL) {
        if (temp->task->priority > max)
            max = temp->task->priority;
        temp = temp->next;
    }
    return max;
}

/* Returns 1 if any task with the given priority is still in the list. */
static int hasTaskWithPriority(int priority) {
    struct node *temp = head;
    while (temp != NULL) {
        if (temp->task->priority == priority)
            return 1;
        temp = temp->next;
    }
    return 0;
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
        int prio = maxPriority();

        /* Round-Robin loop for all tasks at this priority level. */
        while (hasTaskWithPriority(prio)) {
            struct node *temp = head;
            while (temp != NULL) {
                struct node *next = temp->next;
                Task *task = temp->task;

                if (task->priority == prio) {
                    int slice = (task->burst > QUANTUM) ? QUANTUM : task->burst;
                    run(task, slice);
                    task->burst -= slice;

                    if (task->burst == 0) {
                        delete(&head, task);
                        free(task);
                    }
                }
                temp = next;
            }
        }
    }
}
