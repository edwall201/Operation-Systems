#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_FIB_NUMBERS 100

int fib_sequence[MAX_FIB_NUMBERS];
int fib_count;

void *fibonacci(void *arg) {
    int n = *((int*) arg);
    int i;
    if (n > 0) {
        fib_sequence[0] = 0;
    }
    if (n > 1) {
        fib_sequence[1] = 1;
    }
    for (i = 2; i < n; i++) {
        fib_sequence[i] = fib_sequence[i-1] + fib_sequence[i-2];
    }
    fib_count = n;
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./fibonacci <number of fibonacci numbers>\n");
        return -1;
    }
    int n = atoi(argv[1]);
    if (n > MAX_FIB_NUMBERS) {
        printf("Error: Maximum number of fibonacci numbers is %d\n", MAX_FIB_NUMBERS);
        return -1;
    }
    pthread_t thread;
    int thread_arg = n;
    int rc = pthread_create(&thread, NULL, fibonacci, &thread_arg);
    if (rc) {
        printf("Error: pthread_create failed with code %d\n", rc);
        return -1;
    }
    pthread_join(thread, NULL);
    printf("Fibonacci sequence:");
    int i;
    for (i = 0; i < fib_count; i++) {
        printf(" %d", fib_sequence[i]);
    }
    printf("\n");
    return 0;
}

