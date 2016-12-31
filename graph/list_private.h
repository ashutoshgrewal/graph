/**
 * Copyright © 2016 Ashutosh Grewal. All rights reserved.
 *
 * @file list_private.h
 *
 * @date 12/23/16
 * @author Ashutosh Grewal
 *
 * @brief Private definition of structures shared by the list, stack and queue
 *        data structures. This separate header file is made as we do not want
 *        these definitions made visible to the rest of the system.
 */
#ifndef LIST_PRIVATE_H
#define LIST_PRIVATE_H

/**
 * @brief A node that makes up a linked list (or a queue/stack).
 */
struct node_s {
    struct node_s *next; /**< Pointer to the next node. */
    void *data; /**< The user created opaque data stored. */
};

#endif /* LIST_PRIVATE_H */
