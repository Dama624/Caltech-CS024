#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cmdline.h"
#include "memory.h"
#include "cache.h"


/* This is the number of nodes to have in the graph. */
#define NUM_NODES 400

/* This is a value between 0 and 100 indicating the percent of edges
 * that are connections.
 */
#define CONNECTED_PCT 10

/* This is the weight used to represent no edge between nodes. */
#define INFINITY 1000000


/* Set to time(NULL) to generate new random data each time, or a constant to
 * generate the same random data each time.
 */
#define SEED 54321098


typedef struct {
    int num_nodes;

    membase_t *p_mem;
} shortest_path_info;


int get_weight(shortest_path_info *info, int row, int col) {
    return read_int(info->p_mem, row * info->num_nodes + col);
}


void set_weight(shortest_path_info *info, int row, int col, int weight) {
    write_int(info->p_mem, row * info->num_nodes + col, weight);
}


int get_path(shortest_path_info *info, int row, int col) {
    int nodes = info->num_nodes;
    return read_int(info->p_mem, nodes * nodes + row * nodes + col);
}


void set_path(shortest_path_info *info, int row, int col, int node) {
    int nodes = info->num_nodes;
    write_int(info->p_mem, nodes * nodes + row * nodes + col, node);
}


void compute_shortest_paths(shortest_path_info *info) {
    int nodes = info->num_nodes;
    int i, j, k;

    printf(" * Clearing the path-reconstruction state.\n");
    for (i = 0; i < nodes; i++)
        for (j = 0; j < nodes; j++)
            set_path(info, i, j, -1);

    printf(" * Computing the all-points shortest path results.\n");
    for (k = 0; k < nodes; k++) {
        for (i = 0; i < nodes; i++) {
            for (j = 0; j < nodes; j++) {
                int weight_ikj = get_weight(info, i, k) + get_weight(info, k, j);
                int weight_ij = get_weight(info, i, j);

                if (weight_ikj < weight_ij) {
                    set_weight(info, i, j, weight_ikj);
                    set_path(info, i, j, k);
                }
            }
        }
        printf(".");
        fflush(stdout);
    }
    printf("\n");
}



int main(int argc, const char **argv) {
    membase_t *p_mem;

    shortest_path_info info;
    int i, j;

    /* Set up the simulated memory. */
    p_mem = make_cached_memory(argc, argv,
                               2 * NUM_NODES * NUM_NODES * sizeof(int));

    /* Generate a random graph. */

    printf("Generating a random graph containing %d nodes.\n", NUM_NODES);

    srand(SEED);

    info.num_nodes = NUM_NODES;
    info.p_mem = p_mem;

    for (i = 0; i < info.num_nodes; i++) {
        for (j = 0; j < info.num_nodes; j++) {
            if (i != j) {
                if (rand() % 100 < CONNECTED_PCT)
                    set_weight(&info, i, j, 1 + rand() % 10);
                else
                    set_weight(&info, i, j, INFINITY);
            }
            else {
                set_weight(&info, i, j, 0);
            }
        }
    }

    /* Compute the all-points shortest path of the graph. */

    printf("Computing the all-points-shortest-paths of the graph.\n");
    compute_shortest_paths(&info);

    /* Print out the results of the all-points-shortest-paths computation. */

    printf("\nMemory-Access Statistics:\n\n");
    p_mem->print_stats(p_mem);
    printf("\n");

    return 0;
}

