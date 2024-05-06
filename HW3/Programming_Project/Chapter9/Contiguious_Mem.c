#include <stdio.h>
#include <stdlib.h>

#define MAX_MEMORY_SIZE 2

struct MemoryBlock {
    int address;
    int size;
    int is_allocated;
};

struct MemoryBlock memory[MAX_MEMORY_SIZE];

void initialize_memory(int size) {
    int block_size = size / MAX_MEMORY_SIZE;
    for (int i = 0; i < MAX_MEMORY_SIZE; i++) {
        memory[i].address = i * block_size;
        memory[i].size = block_size;
        memory[i].is_allocated = 0;
    }
}

int allocate_memory(int size) {
    for (int i = 0; i < MAX_MEMORY_SIZE; i++) {
        if (!memory[i].is_allocated && memory[i].size >= size) {
            memory[i].is_allocated = 1;
            return memory[i].address;
        }
    }
    return -1; // Allocation failed
}

void release_memory(int address) {
    for (int i = 0; i < MAX_MEMORY_SIZE; i++) {
        if (memory[i].address == address && memory[i].is_allocated) {
            memory[i].is_allocated = 0;
            break;
        }
    }
}

void compact_memory() {
    int free_start = -1;
    int free_end = -1;
    for (int i = 0; i < MAX_MEMORY_SIZE; i++) {
        if (!memory[i].is_allocated) {
            if (free_start == -1) {
                free_start = i;
                free_end = i;
            } else {
                free_end++;
            }
        } else if (free_start != -1) {
            // Compact the free memory block
            memory[free_start].size = free_end - free_start + 1;
            free_start = -1;
            free_end = -1;
        }
    }
}

void report_memory_status() {
    printf("Memory Status:\n");
    for (int i = 0; i < MAX_MEMORY_SIZE; i++) {
        if (memory[i].is_allocated) {
            printf("Allocated block at address %d, size %d\n", memory[i].address, memory[i].size);
        } else if (memory[i].size > 0) {
            printf("Free block at address %d, size %d\n", memory[i].address, memory[i].size);
        }
    }
}

int main() {
    int initial_memory_size;
    printf("Enter the initial amount of memory: ");
    scanf("%d", &initial_memory_size);

    initialize_memory(initial_memory_size);

    // Sample usage of functions
    int alloc1 = allocate_memory(100);
    int alloc2 = allocate_memory(200);
    report_memory_status();

    release_memory(alloc1);
    compact_memory();
    report_memory_status();

    return 0;
}
