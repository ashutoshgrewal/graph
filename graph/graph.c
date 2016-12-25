/**
 * Copyright © 2016 Ashutosh Grewal. All rights reserved.
 *
 * @file graph.c
 *
 * @author Ashutosh Grewal
 * @date 12/23/16
 * 
 * @brief This file implements the graph data structure.
 *
 * @details
 * A graph is set of vertices and edges. Each vertex is connected to a set
 * of other vertices through edges. This implementation stores data at each
 * vertex (another implementation might also have data assosciate with each
 * edge). The data stored is opaque allowing the user to store anything. The
 * adjacent vertices of each vertex are stored as an adjacency list (using 
 * the list implementation). The bread first and depth first traversal functions
 * use the queue and stack implentations respectively.
 *
 * @bug No bugs are know at this point.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "public.h"
#include "graph.h"
#include "list.h"
#include "queue.h"

/**
 * @brief The data structure that represents the vertex in the graph.
 *
 * @details
 * The adjacent vertices to a vertex are stored as list.
 */
struct vertex_s {
    node_t *adjacent_vertex_list; /**< Head of the list of adjacent vertices.*/
    void *data; /**< The data stored at the vertex.*/
    boolean visited;
};

/**
 * @brief Create and initialize the graph data structure.
 *
 * @return Pointer to the memory containing the struct if successful,
 *         NULL otherwise.
 */
graph_t *create_graph (print_data_t print_data, data_is_equal_t data_is_equal)
{
    graph_t *new_graph;
    
    new_graph = (graph_t *) malloc (sizeof(graph_t));
    
    if (new_graph != NULL) {
        new_graph->vertex = NULL;
        new_graph->print_data = print_data;
        new_graph->data_is_equal = data_is_equal;
    }
    
    return new_graph;
}

/**
 * @brief Make both the vertices adjacent to each other by adding them to
 *        each other's adjacency lists.
 *
 * @param[in, out] vertex1 First vertex.
 * @param[in, out] vertex2 Second vertex.
 */
static void make_vertices_adjacent (vertex_t *vertex1, vertex_t *vertex2)
{
    add_to_list(&vertex1->adjacent_vertex_list, vertex2);
    add_to_list(&vertex2->adjacent_vertex_list, vertex1);
}

/**
 * @brief Add a vertex to the graph.
 *
 * @details.
 * Adding a new vertex involves finding all the adjacent vertices using the adjacent
 * vertices data provided and settign up the assosciation.
 *
 * @param[in, out] graph Pointer to the graph data structure.
 * @param[in] data Information the new vertex will store.
 * @param[in] adj_vertex_data An array of opaque data identifying the adjacent vertices
 *                            to this new vertex.
 * @param[in] num_of_adj_vertices The number of adjacent vertices this new
 *                                vertex has.
 * 
 * @return TRUE if vertex is successfully added, FALSE otherwise.
 */
boolean add_vertex_to_graph (graph_t *graph, void *data, void **adj_vertex_data,
                             unsigned num_of_adj_vertices)
{
    vertex_t *vertex = NULL, *lookup_vertex;
    vertex_t **adjacent_vertices = NULL;
    
    adjacent_vertices = (vertex_t **) malloc (sizeof(vertex_t *) * num_of_adj_vertices);
    
    /*
     * Let us make sure, this data doesn't exist in the graph already.
     */
    lookup_vertex = breadth_first_search(graph, data);
    if (lookup_vertex != NULL) {
        goto fail;
    }
    
    /*
     * Find all the adjacent vertices using the data provided.
     */
    for (int i = 0; i < num_of_adj_vertices; i++) {
        lookup_vertex = breadth_first_search(graph, adj_vertex_data[i]);
        if (lookup_vertex == NULL) {
            goto fail;
        }
        adjacent_vertices[i] = lookup_vertex;
    }
    
    vertex = (vertex_t *) malloc (sizeof(vertex_t *));
    if (vertex == NULL) {
        goto fail;
    }
    memset(vertex, 0, sizeof(vertex_t));
     
    for (int i = 0; i < num_of_adj_vertices; i++) {
        make_vertices_adjacent(adj_vertex_data[i], vertex);
    }
    
    return TRUE;
    
fail:
    if (vertex) {
        free(vertex);
    }
    if (adjacent_vertices) {
        free(adjacent_vertices);
    }
    
    return FALSE;
}

/**
 * @brief Get the next adjacent vertex.
 * 
 * @param[in] vertex The current vertex.
 * 
 * @return The next adjacent vertex or NULL if we reached the end of the list.
 */
vertex_t *get_next_adjacent_vertex (vertex_t *vertex)
{
    vertex_t *next_vertex;
    
    next_vertex = get_next_elements_data(vertex);
    
    return next_vertex;
}

/**
 * @brief Has this vertex been visited before?
 *
 * @param[in] vertex The vertex under consideration.
 *
 * @return TRUE if we've visited this vertex before, FALSE otherwise.
 */
static boolean is_visited (vertex_t *vertex)
{
    if (vertex) {
        return vertex->visited;
    } else {
        return FALSE;
    }
}

/**
 * @brief Mark this vertex as visited.
 *
 * @param[in] vertex The vertex we're asked to mark.
 */
static void mark_visited (vertex_t *vertex)
{
    if (vertex) {
        vertex->visited = TRUE;
    }
}

/**
 * @brief Mark this vertex as not visited.
 *
 * @param[in] vertex The vertex we're asked to mark.
 */
static void mark_not_visited (vertex_t *vertex)
{
    if (vertex) {
        vertex->visited = FALSE;
    }
}

/**
 * @brief Traverse the graph data structure in a breadth first fashion.
 *
 * @details
 * We traverse the graph starting from a node. This kind of traversal mandates
 * that we visit all non visisted adjacent vertices of a node before visiting
 * any adjacent vertices of the adjacent vertices. We do so by pushing not yet
 * visisted adjacent vertices of the node to a queue. We pop an element from the
 * queue and repeat this process.
 *
 * @param[in] graph Pointer to the graph data structure.
 */
void breadth_first_traversal (graph_t *graph)
{
    vertex_t *vertex, *adj_vertex;
    queue_t *queue;
    
    vertex = graph->vertex;
    queue = create_queue();
    
    while (vertex) {
        if (!is_visited(vertex)) {
            mark_visited(vertex);
            graph->print_data(vertex->data);
            
            /*
             * Add non visited adjacent vertices of this vertex to the queue.
             */
            adj_vertex = get_data_from_node(vertex->adjacent_vertex_list);
            while (adj_vertex) {
                if (!is_visited(adj_vertex)) {
                    push_to_queue(queue, adj_vertex);
                }
                adj_vertex = get_next_adjacent_vertex(adj_vertex);
            }
        }
        vertex = pop_from_queue(queue);
    }
    
    /*
     * Unmark all the vertices, to enable next search/traversal.
     */
    while (vertex) {
        if (is_visited(vertex)) {
            mark_not_visited(vertex);
            adj_vertex = get_data_from_node(vertex->adjacent_vertex_list);
            while (adj_vertex) {
                if (is_visited(adj_vertex)) {
                    push_to_queue(queue, adj_vertex);
                }
                adj_vertex = get_next_adjacent_vertex(adj_vertex);
            }
        }
        vertex = pop_from_queue(queue);
    }
    destroy_queue(queue);
}

/**
 * @brief Find a node with the given data in the graph traversing in a breadth
 *        first fashion.
 *
 * @see breadth_first_traversal
 *
 * @param[in] graph Pointer to the graph data structure.
 * @param[in] data Opaque data for which we need to search.
 *
 * @return Pointer to vertex containing the opaque data in the graph if one exists,
 *         NULL otherwise.
 */
vertex_t *breadth_first_search (graph_t *graph, void *data)
{
    vertex_t *vertex, *adj_vertex;
    queue_t *queue;
    
    vertex = graph->vertex;
    queue = create_queue();
    
    while (vertex) {
        if (!is_visited(vertex)) {
            mark_visited(vertex);
            if (graph->data_is_equal(data, vertex->data)) {
                break;
            }
            
            adj_vertex = get_data_from_node(vertex->adjacent_vertex_list);
            while (adj_vertex) {
                if (!is_visited(adj_vertex)) {
                    push_to_queue(queue, adj_vertex);
                }
                adj_vertex = get_next_adjacent_vertex(adj_vertex);
            }
        }
        vertex = pop_from_queue(queue);
    }
    
    /*
     * Unmark all the vertices that we marked to enable next search/traversal.
     */
    while (vertex) {
        if (is_visited(vertex)) {
            mark_not_visited(vertex);
            if (graph->data_is_equal(data, vertex->data)) {
                break;
            }
            
            adj_vertex = get_data_from_node(vertex->adjacent_vertex_list);
            while (adj_vertex) {
                if (is_visited(adj_vertex)) {
                    push_to_queue(queue, adj_vertex);
                }
                adj_vertex = get_next_adjacent_vertex(adj_vertex);
            }
        }
        vertex = pop_from_queue(queue);
    }
    
    destroy_queue(queue);
    return vertex;
}
