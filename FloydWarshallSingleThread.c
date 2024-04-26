/* 
   Single-threaded implementation of the Floyd-Warshall algorithm 
   for computing all-pairs shortest paths in a weighted undirected graph.

   Author: John Wanamaker(A00438310)
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NODES 100 // Maximum number of nodes in the graph
#define INF 99999     // Infinity value for unreachable nodes

int **graph; // Adjacency matrix for the graph
int **dist;  // Distance matrix
int num_nodes, num_edges; // Number of nodes and edges in the graph

// Function to initialize distance matrix with adjacency matrix
void initialize_distance_matrix() {
    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            if (i == j)
                dist[i][j] = 0; // Distance from a node to itself is 0
            else if (graph[i][j])
                dist[i][j] = graph[i][j]; // Distance from i to j is the weight of the edge
            else
                dist[i][j] = INF; // Nodes are not connected
        }
    }
}

int main() {
    // Record the start time
    clock_t start_time = clock();

    // Read input graph
    printf("Enter the number of nodes and edges: ");
    scanf("%d %d", &num_nodes, &num_edges);

    // Allocate memory for graph and distance matrices
    graph = (int **)malloc(num_nodes * sizeof(int *));
    dist = (int **)malloc(num_nodes * sizeof(int *));
    for (int i = 0; i < num_nodes; ++i) {
        graph[i] = (int *)malloc(num_nodes * sizeof(int));
        dist[i] = (int *)malloc(num_nodes * sizeof(int));
    }

    // Initialize graph with zeros
    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            graph[i][j] = 0;
        }
    }

    printf("Enter the edge information (source, destination, weight):\n");
    for (int i = 0; i < num_edges; i++) {
        int source, destination, weight;
        scanf("%d %d %d", &source, &destination, &weight);
        if (weight < 0) {
            printf("Error: Negative edge weight detected.\n");
            return 1;
        }
        graph[source - 1][destination - 1] = weight;
        graph[destination - 1][source - 1] = weight; // Setting the reverse edge weight
    }

    // Initialize distance matrix
    initialize_distance_matrix();

    // Floyd-Warshall algorithm
    for (int k = 0; k < num_nodes; ++k) {
        for (int i = 0; i < num_nodes; ++i) {
            for (int j = 0; j < num_nodes; ++j) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    // Record the end time
    clock_t end_time = clock();

    // Print distance matrix
    printf("Final distance matrix:\n");
    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            if (dist[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%d ", dist[i][j]);
            }
        }
        printf("\n");
    }

    // Free memory
    for (int i = 0; i < num_nodes; ++i) {
        free(graph[i]);
        free(dist[i]);
    }
    free(graph);
    free(dist);

    // Calculate and print the execution time
    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %.4f seconds\n", execution_time);

    return 0;
}
