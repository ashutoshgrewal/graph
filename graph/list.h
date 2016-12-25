/**
 * Copyright © 2016 Ashutosh Grewal. All rights reserved.
 *
 * @file file.h
 * @date 12/23/16
 * @author Ashutosh Grewal
 *
 * @brief Header file containing APIs to the list data structure.
 */
#ifndef LIST_H
#define LIST_H

boolean add_to_list (node_t **, void *);
void *get_next_elements_data (void *);
void *get_data_from_node (node_t *);

#endif /* LIST_H */
