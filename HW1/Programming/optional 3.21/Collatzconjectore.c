#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    printf("\n\n OS Optional HW 3.21\n");

    int n = 0;
    int k = 0;

    pid_t pid;

    do {
        printf("Please enter a number greater than 0 to run the Collatz Conjecture.\n");
        if (scanf("%d", &k) != 1 || k <= 0) {
            printf("Invalid input. Please enter a positive integer.\n");
            while (getchar() != '\n'); // Clear input buffer
        }
    } while (k <= 0);

    pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1; // Exit with an error if fork fails
    }

    if (pid == 0) {
        printf("Child is working...\n");
        printf("%d\n", k);
        while (k != 1) {
            if (k % 2 == 0) {
                k = k / 2;
            } else if (k % 2 == 1) {
                k = 3 * k + 1;
            }

            printf("%d\n", k);
        }

        printf("Child process is done.\n");
    } else {
        int status;
        printf("Parent is waiting for the child process...\n");
        wait(&status);
        printf("Parent process is done. Child process exit status: %d\n", status);
    }

    return 0;
}
