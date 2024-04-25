#include <pthread.h>

#define MAX_RESOURCES 5
int available_resources = MAX_RESOURCES;
pthread_mutex_t resource_lock;

void init_lock() {
    pthread_mutex_init(&resource_lock, NULL);
}

void destroy_lock() {
    pthread_mutex_destroy(&resource_lock);
}

int decrease_count(int count) {
    pthread_mutex_lock(&resource_lock);  
    if (available_resources < count) {
        pthread_mutex_unlock(&resource_lock); 
        return -1;
    } else {
        available_resources -= count;
        pthread_mutex_unlock(&resource_lock); 
        return 0;
    }
}

int increase_count(int count) {
    pthread_mutex_lock(&resource_lock); 
    available_resources += count;
    pthread_mutex_unlock(&resource_lock);
    return 0;
}


