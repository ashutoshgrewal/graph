/**
 * Copyright © 2016 Ashutosh Grewal. All rights reserved.
 *
 * @file graph.h
 *
 * @date 12/23/16
 * @author Ashutosh Grewal
 *
 * @brief Header file containing APIs to the graph data structure
 * and some public data structures definition and declarations.
 */

#ifndef GRAPH_H
#define GRAPH_H

typedef struct vertex_s vertex_t;

typedef void (*print_data_t) (void *);
typedef boolean (*data_is_equal_t) (void *, void *);

/**
 * @brief The graph data structure.
 */
typedef struct graph_s {
    vertex_t *vertex; /**< Vertex in the graph. */
    print_data_t print_data; /**< Function pointer to print the user stored
                                  opaque data in the vertices. */
    data_is_equal_t data_is_equal; /**< Function pointer to compare the user
                                        stored opaque data in the vertices. */
} graph_t;

graph_t *create_graph (print_data_t, data_is_equal_t);
boolean add_to_graph (graph_t *, void *, void *[], unsigned int);
boolean delete_from_graph (graph_t *, void *);
vertex_t *breadth_first_search (graph_t *, void *);
vertex_t *depth_first_search (graph_t *, void *);
void bread_first_traversal (graph_t *);
void depth_first_traversal (graph_t *);
void destroy_graph (graph_t *);

#endif /* GRAPH_H */
