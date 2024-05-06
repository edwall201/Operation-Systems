#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

#define NUM_THREADS 4
#define NUM_POINTS 10000000

// Global variables
int count = 0;
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

// Thread function to generate random points and count those that fall within the circle
void *monte_carlo(void *arg) {
    int i;
    double x, y, distance;
    int local_count = 0;

    // Initialize random number generator with thread-specific seed
    unsigned int seed = (unsigned int) time(NULL) * (unsigned int) pthread_self();
    srand(seed);

    // Generate NUM_POINTS/NUM_THREADS random points and count those within the circle
    for (i = 0; i < NUM_POINTS/NUM_THREADS; i++) {
        x = (double) rand() / RAND_MAX * 2 - 1; // Random x coordinate between -1 and 1
        y = (double) rand() / RAND_MAX * 2 - 1; // Random y coordinate between -1 and 1
        distance = sqrt(x*x + y*y); // Distance from origin
        if (distance <= 1.0) { // Point falls within circle
            local_count++;
        }
    }

    // Acquire lock on global count variable and update it
    pthread_mutex_lock(&count_mutex);
    count += local_count;
    pthread_mutex_unlock(&count_mutex);

    pthread_exit(NULL);
}

int main() {
    int i;
    double pi_estimate;
    pthread_t threads[NUM_THREADS];

    // Create threads to perform Monte Carlo simulation
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, monte_carlo, NULL);
    }

    // Wait for all threads to finish
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Estimate pi and output result
    pi_estimate = 4.0 * count / NUM_POINTS;
    printf("Estimated value of pi: %f\n", pi_estimate);

    return 0;
}

