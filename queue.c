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

/* Actual function that sort the elements in queue. */
static void merge_sort(queue_t *q);
static void selection_sort(queue_t *q);
static void bubble_sort(queue_t *q);

/* Array of function pointer which points to the actual sort function */
void (*sort_func[SORT_METHOD_NUM])(queue_t *q) = {
    merge_sort,
    selection_sort,
    bubble_sort,
};


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
    size_t ncopy = (bufsize - 1) >= strlen(q->head->value)
                       ? strlen(q->head->value)
                       : bufsize - 1;
    strncpy(sp, q->head->value, ncopy);
    sp[ncopy] = '\0';

    old_h = q->head;
    q->head = q->head->next;

    /* Update q->tail if q->size == 1 */
    /* TODO: Apply Good-Taste to the update of q->tail */
    if (q->size == 1)
        q->tail = NULL;

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
    if (!q || q->size <= 1)
        return;
    list_ele_t *pre, *cur, *nxt;

    /* Swap q->head and q->tail */
    pre = q->head;
    q->head = q->tail;
    q->tail = pre;

    /* reverse the linked list */
    /* FIXME: Dereferencing NULL pointer */
    for (q->head->next = q->tail, pre = q->tail, cur = pre->next,
        nxt = cur->next;
         ;) {
        /* reverse */
        cur->next = pre;

        /* update the pointer */
        pre = cur;
        if (cur == q->head) {
            q->tail->next = NULL;
            return;  // means we're done
        } else {
            cur = nxt;
            nxt = nxt->next;
        }
    }
}

static void merge_sort(queue_t *q)
{
    if (!q || q->size <= 1)
        return;
}


static void selection_sort(queue_t *q)
{
    if (!q || q->size <= 1)
        return;
}

static void bubble_sort(queue_t *q)
{
    list_ele_t *pre, *cur, *nxt;

    if (!q || q->size <= 1)
        return;
    for (int i = 1; i <= q->size; i++) {
        pre = NULL;
        cur = q->head;
        nxt = q->head->next;
        /* TODO: Change upper bound to j < q->size - i ?
         * Ref: Rosen Discrete Math p.197 */
        for (int j = 1; j <= q->size - i; j++) {
            if (strcmp(cur->value, nxt->value) > 0) {  // need swap
                cur->next = nxt->next;
                nxt->next = cur;

                /* Update pre, cur, nxt */
                if (cur == q->head) {  // if dealing with q->head
                    pre = nxt;
                    q->head = nxt;
                } else {
                    pre->next = nxt;
                    pre = pre->next;
                }

                cur = pre->next;
                nxt = cur->next;
            } else {  // no need to swap
                /* Update pre, cur, nxt */
                pre = cur;
                cur = cur->next;
                nxt = cur->next;
            }
        }
    }
}

/*
 * Register the sorting method.
 */
void q_sort_register_method(int sort_method)
{
    /* Sanity check */
    if (sort_method < MERGE_SORT || sort_method >= SORT_METHOD_NUM)
        sort_method = MERGE_SORT;

    q_sort = sort_func[sort_method];
}
