/**
 * Copyright © 2016 Ashutosh Grewal. All rights reserved.
 *
 * @file queue.c
 * @author Ashutosh Grewal 
 * @date 12/24/16.
 * 
 * @brief This file implements the queue data structure.
 *
 * @details
 * A queue data structure is a type oflinked list where all new elements are
 * pushed to the back while all deletions happen at the front. This is like a
 * queue of people waiting in line to get into a bus.
 * This is implemented as a structure of nodes where we track the first and
 * last element and each node is linked to the next via pointer.
 */
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"
#include "public.h"
#include "list_private.h"

/**
 * @brief The queue data structure.
 */
struct queue_s {
    node_t *first; /**< Pointer to the first element of the queue. */
    node_t *last; /**< Pointer to the last element of the queue. */
};

/**
 * @brief Create and initialize the queue data structure.
 * 
 * @return Pointer to the queue data structure if successful, NULL if memory
 *         allocation failed.
 */
queue_t *create_queue (void)
{
    queue_t *queue;
    
    queue = (queue_t *) malloc (sizeof(queue_t));
    if (queue != NULL) {
        queue->first = queue->last = NULL;
    }
    
    return queue;
}

/**
 * @brief Push an element to the back of the queue.
 *
 * @param[in, out] queue The queue data structure.
 * @param[in] data The opaque data queue's element is asked to store.
 *
 * @return TRUE if successful, FALSE otherwise.
 */
boolean push_to_queue (queue_t *queue, void *data)
{
    node_t *node;
    
    node = (node_t *) malloc (sizeof(node_t));
    if (node == NULL) {
        
        return FALSE;
    }
    node->data = data;
    if (queue->first == NULL) {
        queue->first = queue->last = node;
    } else {
        queue->last->next = node;
        queue->last = node;
    }
    
    return TRUE;
}

/**
 * @brief Pop an element from the front of the queue.
 *
 * @param[in, out] queue The queue data structure.
 *
 * @return Pointer to opaque data stored in the queue if the queue is not empty,
 *         NULL if the queue is empty.
 */
void *pop_from_queue (queue_t *queue)
{
    node_t *node;
    void *data;
    
    if (queue->first == NULL) {
        
        return NULL;
    } else if (queue->first == queue->last){
        node = queue->first;
        assert(queue->first->next == NULL);
        queue->first = queue->last = NULL;
    } else {
        node = queue->first;
        queue->first = queue->first->next;
    }
    data = node->data;
    free(node);
    
    return data;
}

/**
 * @brief Destroy the queue data structure and freeing the elements.
 *
 * @param[in, out] queue Pointer to the queue data structure.
 */
void destroy_queue (queue_t *queue) {
    node_t *node, *temp;
    
    node = queue->first;
    while (node) {
        temp = node;
        node = node->next;
        free(temp);
    }
    queue->first = queue->last = NULL;
    free(queue);
}
