/**
 * Copyright © 2016 Ashutosh Grewal. All rights reserved.
 * @file  main.c
 *
 * @date 12/23/2016
 * @author Ashutosh Grewal
 *
 * @brief This file contains some test code that also demonstrates how to
 * use the graph data structure APIs.
 */

#include <stdio.h>
#include <string.h>
#include "public.h"
#include "graph.h"

/**
 * @brief Print the opaque data knowing that it stores strings.
 *
 * @param[in] data The opaque data we need to print.
 */
void print_string (void *data)
{
    printf(" %s ", (char *)data);
}

/**
 * @brief Determine if the opaque data (which is storing strings) is same.
 *
 * @param[in] data1 First opaque data.
 * @param[in] data2 Second opaque data.
 *
 * @return TRUE if both strings are same, FALSE otherwise.
 */
boolean string_is_same (void *data1, void *data2)
{
    char *string1, *string2;
    unsigned int str1_len, str2_len;
    
    string1 = (char *)data1;
    string2 = (char *)data2;
    str1_len = strlen(string1);
    str2_len = strlen(string2);
    
    if (str1_len != str2_len) {
        return FALSE;
    }
    
    for (int i = 0; i < str1_len; i++) {
        if (string1[i] != string2[i]) {
            return FALSE;
        }
    }
    
    return TRUE;
}

int main(int argc, const char * argv[]) {
    graph_t *graph;
    
    graph = create_graph (print_string, string_is_same);
    return 0;
}
