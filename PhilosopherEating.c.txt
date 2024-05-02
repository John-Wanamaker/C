/*Implement a deadlock-free and starvation-free solution for 
Dining Philosophers Problem using any synchronization primitives. 
Attach a source code here along with few lines of output (40 points) 
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

pthread_mutex_t chopsticks[NUM_PHILOSOPHERS];
pthread_t philosophers[NUM_PHILOSOPHERS];

// Function to simulate philosopher's behavior
void *philosopher_behavior(void *arg) {
    int philosopher_id = *((int *) arg);
    int left_chopstick = philosopher_id;
    int right_chopstick = (philosopher_id + 1) % NUM_PHILOSOPHERS;

    while (1) {
        // Philosopher is thinking
        printf("P%d is thinking\n", philosopher_id + 1);
        usleep(rand() % 1000000 + 1); // Simulate thinking

        // Philosopher wants to eat
        printf("P%d wants to eat\n", philosopher_id + 1);

        // Acquire chopsticks in a resource hierarchy order
        if (philosopher_id % 2 == 0) {
            pthread_mutex_lock(&chopsticks[left_chopstick]);
            pthread_mutex_lock(&chopsticks[right_chopstick]);
        } else {
            pthread_mutex_lock(&chopsticks[right_chopstick]);
            pthread_mutex_lock(&chopsticks[left_chopstick]);
        }

        // Philosopher is eating
        printf("P%d is eating\n", philosopher_id + 1);
        usleep(rand() % 1000000 + 1); // Simulate eating

        // Release chopsticks
        pthread_mutex_unlock(&chopsticks[left_chopstick]);
        pthread_mutex_unlock(&chopsticks[right_chopstick]);
    }

    return NULL;
}

int main() {
    srand(time(NULL));

    // Initialize mutex locks for each chopstick
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&chopsticks[i], NULL);
    }

    // Create threads for each philosopher
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&philosophers[i], NULL, philosopher_behavior, (void *) id);
    }

    // Wait for all philosopher threads to finish
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy mutex locks
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&chopsticks[i]);
    }

    return 0;
}
