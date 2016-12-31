/**
 * @copyright © 2016 Ashutosh Grewal. All rights reserved.
 * @file list.c
 * @author Ashutosh Grewal 
 * @date 12/23/16.
 * 
 * @brief This file implements a list data structure.
 *
 * @details.
 * Each node in the linked list stores opaque data. The opaque data allows
 * user to store any information they want to store. Each list has a head
 * that points to the first element of the list. The elments are linked with
 * each other using pointers.
 */
#include <stdlib.h>
#include "public.h"
#include <stddef.h>
#include "list.h"
#include "list_private.h"

/**
 * @brief Add an element to the linked list whose head is provided.
 *
 * @param[out] head Pointer to the head node.
 * @param[in] data The data the new node will store.
 *
 * @return TRUE if succesful in adding, FALSE otherwise.
 */
boolean add_to_list (node_t **head, void *data) {
    node_t *node;
    
    node = (node_t *) malloc (sizeof(node_t));
    if (node == NULL) {
        
        return FALSE;
    }
    node->data = data;
    node->next = *head;
    *head = node;
    
    return TRUE;
}

/**
 * @brief Return the next element stored in the list.
 *
 * @param[in] node A node in the list.
 *
 * @return The next element or NULL if the list has reached the end.
 */
node_t *get_next_node (node_t *node)
{
    if (node) {
        return node->next;
    } else {
        return NULL;
    }
}


/**
 * @brief Return the opaque data of this list element.
 *
 * @param[in] node The current node in the list.
 *
 * @return Element's data or NULL if the passed in element is NULL.
 */
void *get_data_from_node (node_t *node)
{
    if (node) {
        return node->data;
    } else {
        return NULL;
    }
}
