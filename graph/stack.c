/**
 * @copyright © 2016 Ashutosh Grewal. All rights reserved.
 * @file stack.c
 * @author Ashutosh Grewal 
 * @date 12/30/16.
 *
 * @brief This file impleents the stack data structure.
 * 
 * @details
 * A stack data structure is a type of linked list where all new elements are
 * pushed to the front and all deletions happen at the front. This can be 
 * visualized as a stack of dishes being kept on top of each other and then being
 * picked back for washing. The last one put is the one picked up first.
 * This is implemented as a structure of nodes where we track the top of the 
 * list and each node is linked to the next via a pointer.
 * 
 * @bug No bugs are know at this point.
 */
#include "list_private.h"
#include "list.h"
#include "stack.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief The stack data structure.
 */
struct stack_s {
    node_t *top;      /**< Pointer to the top element of the stack. */
};

/**
 * @brief Create and intialize the stack data structure.
 *
 * @return Pointer to the stack data structure if allocation was successful, NULL
 *         if allocation failed.
 */
stack_type *create_stack (void)
{
    stack_type *stack;
    
    stack = (stack_type *) malloc (sizeof(stack_type));
    if (stack != NULL) {
        stack->top = NULL;
    }
    
    return stack;
}

/**
 * @brief Push an element to the top of the stack.
 *
 * @param[in, out] stack Pointer to the stack data structure.
 * @param[in] data The opaque data stack's element is asked to store.
 *
 * @return TRUE if successful, FALSE otherwise.
 */
boolean push_to_stack (stack_type *stack, void *data)
{
    node_t *node;
    
    if (stack == NULL) {
        return FALSE;
    }
    
    node = (node_t *) malloc (sizeof(node_t));
    if (node != NULL) {
        node->data = data;
        node->next = stack->top;
        stack->top = node;
        
        return TRUE;
    }
    
    return FALSE;
}

/**
 * @brief Pop an element from the top of stack.
 *
 * @param[in, out] stack Pointer to the stack data structure.
 * 
 * @return Pointer to the opaque data stored in the stack's top element if
 *         the stack is not empty, NULL if the stack is empty.
 */
void *pop_from_stack (stack_type *stack)
{
    node_t *node;
    void *data;
    
    if (stack == NULL) {
        return NULL;
    }
    
    if (stack->top != NULL) {
        node = stack->top;
        data = node->data;
        stack->top = node->next;
        free(node);
        
        return data;
    }
    
    return NULL;
}

/**
 * @brief Destory the stack data structure, freeing all it's elements in the
 *        process.
 * 
 * @param[in, out] stack Pointer to the stack data structure.
 */
void destroy_stack (stack_type *stack)
{
    node_t *node, *temp;
    
    if (stack == NULL) {
        
        return;
    }
    node = stack->top;
    while (node) {
        temp = node;
        node = node->next;
        free(temp);
    }
    stack->top = NULL;
    free(stack);
}
