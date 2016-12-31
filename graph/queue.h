/**
 * @copyright © 2016 Ashutosh Grewal. All rights reserved.
 * @file queue.h
 * @author Ashutosh Grewal 
 * @date 12/24/16.
 *
 * @brief This header file contains APIs to use the queue data structure and
 *        some public structure declarations (the definitions of these
 *        structures is not visible to the rest of the system to prevent 
 *        them from manipulating without using APIs).
 */
#ifndef QUEUE_H
#define QUEUE_H

#include "public.h"

typedef struct queue_s queue_t;

queue_t *create_queue (void);
boolean push_to_queue (queue_t *, void *);
void *pop_from_queue (queue_t *);
void destroy_queue (queue_t *);

#endif /* QUEUE_H */

