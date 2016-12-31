/**
 * @copyright © 2016 Ashutosh Grewal. All rights reserved.
 * @file stack.h
 * @author Ashutosh Grewal
 * @date 12/30/16.
 *
 * @brief This header file contains APIs to use the stack data structure and some
 *        public structure declarations (the definitions of these structures is
 *        not visisble to the rest of the system to prevent them from manipulating
 *        these structs without using APIs.
 */
#ifndef STACK_H
#define STACK_H

#include "public.h"

typedef struct stack_s stack_type;

stack_type *create_stack (void);
boolean push_to_stack (stack_type *, void *);
void *pop_from_stack (stack_type *);
void destroy_stack (stack_type *);

#endif /* STACK_H */
