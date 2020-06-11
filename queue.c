#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/* GUARD of NULL pointer */
#define NULL_PTR_GUARD(q) \
    do {                  \
        if (!q)           \
            return 0;     \
    } while (0)


/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* TODO: What if malloc returned NULL? */
    NULL_PTR_GUARD(q);

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;

    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;

    list_ele_t *tmp;

    for (tmp = q->head; tmp;) {
        /* Store the next element */
        q->head = tmp->next;
        free(tmp->value);
        free(tmp);
        tmp = q->head;
    }

    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    /* Guard of NULL pointer */
    size_t len = strlen(s);
    NULL_PTR_GUARD(q);
    NULL_PTR_GUARD(len);

    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    NULL_PTR_GUARD(newh);

    newh->value = malloc(len + 1);
    if (!newh->value) {
        free(newh);
        return false;
    }

    strncpy(newh->value, s, len);
    newh->value[len] = '\0';

    newh->next = q->head;
    q->head = newh;

    if (!q->tail)
        q->tail = newh;

    q->size++;

    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    NULL_PTR_GUARD(q);
    size_t len = strlen(s);

    list_ele_t *newt = malloc(sizeof(list_ele_t));
    NULL_PTR_GUARD(newt);
    newt->next = NULL;
    newt->value = malloc(len + 1);
    if (!newt->value) {
        free(newt->value);
        return false;
    }

    strncpy(newt->value, s, len);
    newt->value[len] = '\0';

    if (!q->tail) {  // empty queue
        q->tail = newt;
        q->head = newt;
    } else {
        q->tail->next = newt;
        q->tail = q->tail->next;
    }

    q->size++;

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    list_ele_t *old_h;

    if (!q || !q->head || !sp)
        return false;
    /* FIXME: verify if bufsize == strlen(q->head->value) */
    size_t ncopy = bufsize >= strlen(q->head->value) ? strlen(q->head->value)
                                                     : bufsize - 1;
    strncpy(sp, q->head->value, ncopy);
    sp[ncopy] = '\0';

    old_h = q->head;
    q->head = q->head->next;

    free(old_h->value);
    free(old_h);

    q->size--;

    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    NULL_PTR_GUARD(q);
    return q->size;
    // return q ? q->size : 0;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || q->size == 1)
        return;

    /* Set the pos of q->tail and tmp */
    list_ele_t *tmp;
    tmp = q->head->next;
    q->tail = tmp->next;
    q->head->next = NULL;

    /* Do reverse, tmp always point to the list element we attempt
     * to move
     */
    for (; tmp;) {
        tmp->next = q->head;
        q->head = tmp;
        tmp = q->tail;
        /* TODO: set the tail to NULL after reverse,
         * so we don't need this if statement
         */
        if (tmp)
            q->tail = tmp->next;
    }
    return;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
}
