#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 100

int fib[MAX]; 
int count = 0; // Add a count variable to keep track of the numbers computed
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *child(void *arg) {
    int *n = (int *)arg;
    fib[0] = 0;
    fib[1] = 1;
    for (int i = 2; i < *n + 2; i++) { // Change this line
        fib[i] = fib[i-1] + fib[i-2];
        pthread_mutex_lock(&mutex);
        count++; // Increment count after a number has been computed
        pthread_cond_signal(&cond); // Signal parent thread that a number has been computed
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    int n = 10; // Number of Fibonacci numbers to compute
    pthread_t tid;
    pthread_create(&tid, NULL, child, &n);
    for (int i = 0; i < n; i++) {
        pthread_mutex_lock(&mutex);
        while (i >= count) { // Wait for child thread to signal that a number has been computed
            pthread_cond_wait(&cond, &mutex);
        }
        printf("%d\n", fib[i]);
        pthread_mutex_unlock(&mutex);
    }
    pthread_join(tid, NULL);
    return 0;
}