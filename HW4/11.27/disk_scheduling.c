#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CYLINDERS 5000
#define REQUESTS 1000

// Function prototypes
void generate_requests(int requests[]);
void print_requests(int requests[]);
int fcfs(int requests[], int start);
int scan(int requests[], int start);
int c_scan(int requests[], int start);
int compare(const void *a, const void *b);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <initial head position>\n", argv[0]);
        return 1;
    }

    int start = atoi(argv[1]);
    if (start < 0 || start >= CYLINDERS) {
        fprintf(stderr, "Error: Initial head position must be between 0 and %d\n", CYLINDERS - 1);
        return 1;
    }

    int requests[REQUESTS];
    generate_requests(requests);

    printf("Disk requests:\n");
    print_requests(requests);

    int fcfs_movement = fcfs(requests, start);
    int scan_movement = scan(requests, start);
    int c_scan_movement = c_scan(requests, start);

    printf("\nTotal head movement:\n");
    printf("FCFS: %d\n", fcfs_movement);
    printf("SCAN: %d\n", scan_movement);
    printf("C-SCAN: %d\n", c_scan_movement);

    return 0;
}

void generate_requests(int requests[]) {
    srand(time(NULL));
    for (int i = 0; i < REQUESTS; i++) {
        requests[i] = rand() % CYLINDERS;
    }
}

void print_requests(int requests[]) {
    for (int i = 0; i < REQUESTS; i++) {
        printf("%d ", requests[i]);
    }
    printf("\n");
}

int fcfs(int requests[], int start) {
    int total_movement = 0;
    int current_position = start;
    for (int i = 0; i < REQUESTS; i++) {
        total_movement += abs(requests[i] - current_position);
        current_position = requests[i];
    }
    return total_movement;
}

int scan(int requests[], int start) {
    int total_movement = 0;
    int current_position = start;
    int direction = 1; // Start by moving towards higher cylinder numbers

    qsort(requests, REQUESTS, sizeof(int), compare);

    for (int i = 0; i < REQUESTS; i++) {
        if (requests[i] >= start) {
            // Move towards the end of the disk
            for (int j = i; j < REQUESTS; j++) {
                total_movement += abs(current_position - requests[j]);
                current_position = requests[j];
            }
            // Move to the end of the disk
            total_movement += abs(current_position - (CYLINDERS - 1));
            current_position = CYLINDERS - 1;
            direction = -1;
            break;
        }
    }

    if (direction == 1) {
        // If no requests were greater than start, we must reverse direction
        total_movement += abs(current_position - 0);
        current_position = 0;
        direction = -1;
    }

    for (int i = REQUESTS - 1; i >= 0; i--) {
        if (requests[i] < start) {
            total_movement += abs(current_position - requests[i]);
            current_position = requests[i];
        }
    }

    return total_movement;
}

int c_scan(int requests[], int start) {
    int total_movement = 0;
    int current_position = start;

    qsort(requests, REQUESTS, sizeof(int), compare);

    for (int i = 0; i < REQUESTS; i++) {
        if (requests[i] >= start) {
            // Move towards the end of the disk
            for (int j = i; j < REQUESTS; j++) {
                total_movement += abs(current_position - requests[j]);
                current_position = requests[j];
            }
            // Move to the end of the disk
            total_movement += abs(current_position - (CYLINDERS - 1));
            current_position = CYLINDERS - 1;
            // Move to the start of the disk
            total_movement += CYLINDERS - 1;
            current_position = 0;
            break;
        }
    }

    for (int i = 0; i < REQUESTS; i++) {
        if (requests[i] < start) {
            total_movement += abs(current_position - requests[i]);
            current_position = requests[i];
        }
    }

    return total_movement;
}

int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}
