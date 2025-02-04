#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

/* Create an empty queue */
static element_t *e_new(char *s)
{
    element_t *element = (element_t *) malloc(sizeof(element_t));

    if (element) {
        INIT_LIST_HEAD(&(element->list));
        element->value = NULL;

        if (s) {
            size_t len = strlen(s) + 1;
            char *value = (char *) malloc(len);

            if (!value) {
                free(element);
                element = NULL;
            } else {
                strncpy(value, s, len);
                element->value = value;
            }
        }
    }

    return element;
}


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *result =
        (struct list_head *) malloc(sizeof(struct list_head));

    if (result) {
        INIT_LIST_HEAD(result);
    }

    return result;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (head) {
        element_t *iter = NULL;
        element_t *tmp = NULL;

        list_for_each_entry_safe (iter, tmp, head, list) {
            list_del_init(&iter->list);
            free(iter->value);
            free(iter);
        }
        free(head);
    }
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    bool result = false;
    if (head) {
        element_t *element = e_new(s);
        if (element) {
            list_add(&element->list, head);
            result = true;
        }
    }

    return result;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    bool result = false;
    if (head) {
        element_t *element = e_new(s);
        if (element) {
            list_add_tail(&element->list, head);
            result = true;
        }
    }

    return result;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    element_t *entry = NULL;
    if (head && (head->next != head)) {  // queue exist and Non empty queue
        entry = (element_t *) list_entry(head->next, element_t, list);
        if (sp && entry && entry->value) {
            int len = strlen(entry->value) + 1;
            if (len > bufsize) {
                strncpy(sp, entry->value, bufsize - 1);
                sp[bufsize - 1] = '\0';
            } else {
                strncpy(sp, entry->value, len);
            }
        }
        list_del_init(head->next);  // unlink and reset the removed node
    }
    return entry;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    element_t *entry = NULL;
    if (head && (head->prev != head)) {  // queue exist and Non empty queue
        entry = (element_t *) list_entry(head->prev, element_t, list);
        if (sp && entry && entry->value) {
            int len = strlen(entry->value) + 1;
            if (len > bufsize) {
                strncpy(sp, entry->value, bufsize - 1);
                sp[bufsize - 1] = '\0';
            } else {
                strncpy(sp, entry->value, len);
            }
        }
        list_del_init(head->prev);  // unlink and reset the removed node
    }
    return entry;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    int len = 0;

    if (head) {
        struct list_head *li = NULL;

        list_for_each (li, head) {
            if (li != NULL) {
                len++;
            } else {
                break;
            }
        }
    }

    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{  // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    bool result = true;

    if (head && (head->next != head)) {
        struct list_head *slow = head->next;
        struct list_head *fast = head->next;
        struct list_head *prev = head;
        while ((fast != head) && (fast->next != head)) {  // 1 3 4 7 1 2 6
            fast = fast->next->next;
            prev = slow;
            slow = slow->next;
        }

        if (fast != slow) {
            prev->next = slow->next;
            list_del_init(slow);  // unlink and reset the removed node
            element_t *e = container_of(slow, element_t, list);
            free(e->value);
            free(e);

        } else {
            head->next = NULL;
        }
    } else {
        result = false;
    }

    return result;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{  // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (head && (head->next != head)) {  // link list exist and has  entry
        struct list_head *first = head->next;
        struct list_head *second = NULL;
        struct list_head *prev = head;

        while (first && (first->next != head)) {
            second = first->next;

            if (prev) {  // connect previous -> second entry in the pair
                prev->next = second;
                second->prev = prev;
            }

            first->next = second->next;  // //swap first and second's next entry
            second->next = first;        // connect second entry -> first entry
            first->prev = second;        // connect second entry <-  first entry

            prev = first;  // update prev
            if (first->next != head) {
                first = first->next;  // update first entry in the pair
            }
        }

        head->prev = first;  // update tail entry
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (head) {
        struct list_head *curr = head;
        struct list_head *temp = NULL;
        while (curr) {
            temp = curr->next;
            curr->next = curr->prev;
            curr->prev = temp;
            curr = temp;
            if (curr == head) {
                break;
            }
        }
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
