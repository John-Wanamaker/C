/* 
   Multithreaded implementation of the Floyd-Warshall algorithm 
   for parallel computation of all-pairs shortest paths in a weighted undirected graph.

   Author: John Wanamaker (A00438310)
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_NODES 100 // Maximum number of nodes in the graph
#define INF 99999     // Infinity value for unreachable nodes

int **graph; // Adjacency matrix for the graph
int **dist;  // Distance matrix
int num_nodes, num_edges; // Number of nodes and edges in the graph
int num_threads; // Number of threads for parallel execution

pthread_mutex_t mutex; // Mutex for synchronization
pthread_rwlock_t rwlock; // Reader-writer lock

// Structure to hold values n, k, and i
struct arg_s {
    int n;
    int i;
    int k;
};

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

// Worker function
void *worker(void *args) {
    struct arg_s *params = (struct arg_s *)args;
    int n = params->n;
    int k = params->k;
    int i = params->i;
    for (int j = 0; j < n; j++) {
        pthread_rwlock_rdlock(&rwlock);
        if ((dist[i][k] + dist[k][j]) < dist[i][j]) {
            pthread_rwlock_unlock(&rwlock);
            pthread_rwlock_wrlock(&rwlock);
            dist[i][j] = dist[i][k] + dist[k][j];
            pthread_rwlock_unlock(&rwlock);
        } else {
            pthread_rwlock_unlock(&rwlock);
        }
    }
    pthread_exit(NULL);
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

    // Initialize mutex for synchronization
    pthread_rwlock_init(&rwlock, NULL);

    // Set the number of threads to the number of nodes for this algorithm
    num_threads = num_nodes;

    // Create threads for each iteration of k
    pthread_t threads[num_threads];
    struct arg_s arguments[num_threads];

    for (int k = 0; k < num_nodes; k++) {
        for (int i = 0; i < num_threads; i++) {
            arguments[i].n = num_nodes;
            arguments[i].i = i;
            arguments[i].k = k;
            pthread_create(&threads[i], NULL, worker, (void *)&arguments[i]);
        }
        for (int i = 0; i < num_threads; i++) {
            pthread_join(threads[i], NULL);
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

    // Destroy mutex
    pthread_rwlock_destroy(&rwlock);

    // Calculate and print the execution time
    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %.4f seconds\n", execution_time);

    return 0;
}
