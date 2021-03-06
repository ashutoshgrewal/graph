/**
 * @copyright © 2016 Ashutosh Grewal. All rights reserved.
 * @file graph.c
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
 * @bug
 * If a vertex's only adjacent vertex in the graph is deleted, this vertex will
 * get leaked.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "public.h"
#include "graph.h"
#include "list.h"
#include "queue.h"
#include "stack.h"

/**
 * @brief The data structure that represents the vertex in the graph.
 *
 * @details
 * The adjacent vertices to a vertex are stored as list.
 */
struct vertex_s {
    node_t *adjacent_vertex_list; /**< Head of the list of adjacent vertices.*/
    void *data; /**< The data stored at the vertex.*/
    boolean visited; /**< Boolean to help identify nodes already visited during
                          a traversal. */
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
                             unsigned int num_of_adj_vertices)
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
    
    vertex = (vertex_t *) malloc (sizeof(vertex_t));
    if (vertex == NULL) {
        goto fail;
    }
    memset(vertex, 0, sizeof(vertex_t));
    vertex->data = data;
     
    for (int i = 0; i < num_of_adj_vertices; i++) {
        make_vertices_adjacent(adjacent_vertices[i], vertex);
    }
    if (graph->vertex == NULL) {
        graph->vertex = vertex;
    }
    if (adjacent_vertices) {
        free(adjacent_vertices);
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
 * that we visit all adjacent vertices of a node before visiting any adjacent
 * vertices of the adjacent vertices. We carefully avoid re-visting already
 * visited nodes. We do so by pushing not yet visited adjacent vertices of the
 * node to a queue. We pop an element from the queue and repeat this process.
 *
 * @note
 * We can also remove the visited mark from vertices after a search or traversal
 * by keeping pointers to all the visited nodes during the first pass. This
 * would have been a linear time complexity solution but would need us to
 * allocate pointers worth the # of vertices in the graph (linear space
 * complexity). The current solution requires no extra space.
 *
 * @param[in] graph Pointer to the graph data structure.
 */
void breadth_first_traversal (graph_t *graph)
{
    vertex_t *vertex, *adj_vertex;
    queue_t *queue;
    node_t *node;
    
    vertex = graph->vertex;
    queue = create_queue();
    
    while (vertex) {
        mark_visited(vertex);
        graph->print_data(vertex->data);
        
        /*
         * Add non visited adjacent vertices of this vertex to the queue.
         */
        for (node = vertex->adjacent_vertex_list; node; node = get_next_node(node)) {
            adj_vertex = get_data_from_node(node);
            if (!is_visited(adj_vertex)) {
                mark_visited(adj_vertex);
                push_to_queue(queue, adj_vertex);
            }
        }
        vertex = pop_from_queue(queue);
    }
    
    /*
     * Unmark all the vertices, to enable next search/traversal.
     */
    destroy_queue(queue);
    queue = create_queue();
    vertex = graph->vertex;
    while (vertex) {
        mark_not_visited(vertex);
        for (node = vertex->adjacent_vertex_list; node; node = get_next_node(node)) {
            adj_vertex = get_data_from_node(node);
            if (is_visited(adj_vertex)) {
                mark_not_visited(adj_vertex);
                push_to_queue(queue, adj_vertex);
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
    vertex_t *vertex, *lookedup_vertex, *adj_vertex;
    queue_t *queue;
    node_t *node;
    
    vertex = graph->vertex;
    queue = create_queue();
    
    while (vertex) {
        mark_visited(vertex);
        if (graph->data_is_equal(data, vertex->data)) {
            break;
        }
            
        for (node = vertex->adjacent_vertex_list; node; node = get_next_node(node)) {
            adj_vertex = get_data_from_node(node);
            if (!is_visited(adj_vertex)) {
                mark_visited(adj_vertex);
                push_to_queue(queue, adj_vertex);
            }
        }
        vertex = pop_from_queue(queue);
    }
    lookedup_vertex = vertex;
    
    /*
     * Unmark all the vertices that we marked to enable next search/traversal.
     */
    destroy_queue(queue);
    queue = create_queue();
    vertex = graph->vertex;
    while (vertex) {
        mark_not_visited(vertex);
        if (graph->data_is_equal(data, vertex->data)) {
            break;
        }
        
        for (node = vertex->adjacent_vertex_list; node; node = get_next_node(node)) {
            adj_vertex = get_data_from_node(node);
            if (is_visited(adj_vertex)) {
                mark_not_visited(adj_vertex);
                push_to_queue(queue, adj_vertex);
            }
        }
        vertex = pop_from_queue(queue);
    }
    destroy_queue(queue);
    
    return lookedup_vertex;
}

/**
 * @brief Traverse the graph data structure in a depth first fashion.
 *
 * @details
 * We traverse the graph starting from a node. This kind of traversal mandates
 * that we visit adjacent vertices of a node's immediate adjacent vertices before
 * visiting the adjacent vertices of a node. We carefully avoid re-visiting 
 * already visited vertices. We do so by pushing not yet visited adjacent
 * vertices of the node to a stack. We pop an element from the stack and repeat
 * this process.
 *
 * @param[in] graph Pointer to the graph data structure.
 */
void depth_first_traversal (graph_t *graph)
{
    vertex_t *vertex, *adj_vertex;
    stack_type *stack;
    node_t *node;
    
    vertex = graph->vertex;
    stack = create_stack();
    
    while (vertex) {
        mark_visited(vertex);
        graph->print_data(vertex->data);
            
        /*
         * Add non visited adjacent vertices of this vertex to the queue.
         */
        for (node = vertex->adjacent_vertex_list; node; node = get_next_node(node)) {
            adj_vertex = get_data_from_node(node);
            if (!is_visited(adj_vertex)) {
                mark_visited(adj_vertex);
                push_to_stack(stack, adj_vertex);
            }
        }
        vertex = pop_from_stack(stack);
    }
    
    /*
     * Unmark all the vertices, to enable next search/traversal.
     */
    destroy_stack(stack);
    stack = create_stack();
    vertex = graph->vertex;
    while (vertex) {
        mark_not_visited(vertex);
        for (node = vertex->adjacent_vertex_list; node; node = get_next_node(node)) {
            adj_vertex = get_data_from_node(node);
            if (is_visited(adj_vertex)) {
                mark_not_visited(adj_vertex);
                push_to_stack(stack, adj_vertex);
            }
        }
        vertex = pop_from_stack(stack);
    }
    destroy_stack(stack);
}

/**
 * @brief Find a node with the given data in the graph traversing in a depth
 *        first fashion.
 *
 * @see depth_first_traversal
 *
 * @param[in] graph Pointer to the graph data structure.
 * @param[in] data Opaque data for which we need to search.
 *
 * @return Pointer to vertex containing the opaque data in the graph if one exists,
 *         NULL otherwise.
 */
vertex_t *depth_first_search (graph_t *graph, void *data)
{
    vertex_t *vertex, *lookedup_vertex, *adj_vertex;
    stack_type *stack;
    node_t *node;
    
    vertex = graph->vertex;
    stack = create_stack();
    
    while (vertex) {
        mark_visited(vertex);
        if (graph->data_is_equal(data, vertex->data)) {
            break;
        }
        
        for (node = vertex->adjacent_vertex_list; node; node = get_next_node(node)) {
            adj_vertex = get_data_from_node(node);
            if (!is_visited(adj_vertex)) {
                mark_visited(adj_vertex);
                push_to_stack(stack, adj_vertex);
            }
        }
        vertex = pop_from_stack(stack);
    }
    lookedup_vertex = vertex;
    
    /*
     * Unmark all the vertices that we marked to enable next search/traversal.
     */
    destroy_stack(stack);
    stack = create_stack();
    vertex = graph->vertex;
    while (vertex) {
        mark_not_visited(vertex);
        if (graph->data_is_equal(data, vertex->data)) {
            break;
        }
        
        for (node = vertex->adjacent_vertex_list; node; node = get_next_node(node)) {
            adj_vertex = get_data_from_node(node);
            if (is_visited(adj_vertex)) {
                mark_not_visited(adj_vertex);
                push_to_stack(stack, adj_vertex);
            }
        }
        vertex = pop_from_stack(stack);
    }
    destroy_stack(stack);
    
    return lookedup_vertex;
}

/**
 * @brief Delete a passed in vertex from the graph.
 *
 * @param[in,out] graph Pointer to the graph data structure.
 * @param[in] vertex Vertex we need to delete.
 *
 * @return TRUE if successfully deleted the specified vertex, FALSE otherwise.
 */
static boolean delete_vertex_from_graph (graph_t *graph, vertex_t *vertex)
{
    vertex_t *adj_vertex;
    node_t *node;

    if (vertex == NULL) {
        
        return FALSE;
    }
    
    for (node=vertex->adjacent_vertex_list; node; node = get_next_node(node)) {
        adj_vertex = get_data_from_node(node);
        assert(delete_from_list(&adj_vertex->adjacent_vertex_list, vertex));
        assert(delete_from_list(&vertex->adjacent_vertex_list, adj_vertex));
    }
    
    assert(vertex->adjacent_vertex_list == NULL);
    free(vertex);
    
    return TRUE;
}

/**
 * @brief Delete a vertex - containing the specified data- from the graph.
 *
 * @details
 * Deleting a vertex involves deleting this node from the adjacent list of
 * all the vertices that are adjacent.
 * 
 * @param[in,out] graph Pointer to the graph data structure.
 * @param[in] data Information the vertex we need to delete contains.
 *
 * @return TRUE if found and deleted a vertex containing the specifed data,
 * False otherwise.
 */
boolean delete_from_graph (graph_t *graph, void *data)
{
    vertex_t *vertex;
    
    vertex = breadth_first_search(graph, data);
    return delete_vertex_from_graph(graph, vertex);
}

/**
 * @brief Destory the graph, deleting all the vertexes and related assosciations
 * in the process.
 *
 * @details
 * This is simply implemented as a DFS traversal in which we store each visited
 * vertex to a stack. ONce the traversal is complete, we delete all the vertices
 * stored in the stack.
 * 
 * @param[in,out] graph Pointer to the graph.
 */
void destroy_graph (graph_t *graph)
{
    vertex_t *vertex, *adj_vertex;
    stack_type *traversal_stack, *stack_with_all_vertices;
    node_t *node;
    
    vertex = graph->vertex;
    traversal_stack = create_stack();
    stack_with_all_vertices = create_stack();
    if (vertex) {
        push_to_stack(stack_with_all_vertices, vertex);
    }
    
    while (vertex) {
        mark_visited(vertex);
        
        /*
         * Add non visited adjacent vertices of this vertex to the queue.
         */
        for (node = vertex->adjacent_vertex_list; node; node = get_next_node(node)) {
            adj_vertex = get_data_from_node(node);
            if (!is_visited(adj_vertex)) {
                mark_visited(adj_vertex);
                push_to_stack(traversal_stack, adj_vertex);
                push_to_stack(stack_with_all_vertices, adj_vertex);
            }
        }
        vertex = pop_from_stack(traversal_stack);
    }
    destroy_stack(traversal_stack);
    
    /*
     * Delete all the vertices in the graph.
     */
    vertex = pop_from_stack(stack_with_all_vertices);
    for (; vertex; vertex = pop_from_stack(stack_with_all_vertices)) {
        delete_vertex_from_graph(graph, vertex);
    }
    free(graph);
    destroy_stack(stack_with_all_vertices);
}
