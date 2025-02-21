#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define ARRAY_SIZE 10000  // Large array size for memory stress
#define ITERATIONS 100    // Number of times to traverse the array

int **large_array;
int NUM_THREADS;  // Global variable to hold thread count

void *memory_traverse(void *arg) {
    int thread_id = *(int *)arg;
    int start = (ARRAY_SIZE / NUM_THREADS) * thread_id;
    int end = (ARRAY_SIZE / NUM_THREADS) * (thread_id + 1);
    
    for (int iter = 0; iter < ITERATIONS; iter++) {
        for (int i = start; i < end; i++) {
            for (int j = 0; j < ARRAY_SIZE; j++) {
                large_array[i][j] += 1;  // Simulate memory-intensive operation
            }
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num_threads>\n", argv[0]);
        return EXIT_FAILURE;
    }

    NUM_THREADS = atoi(argv[1]);
    if (NUM_THREADS <= 0) {
        fprintf(stderr, "Error: Number of threads must be a positive integer.\n");
        return EXIT_FAILURE;
    }

    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    clock_t start, end;
    double time_taken;
    
    // Allocate memory for large 2D array
    large_array = (int **)malloc(ARRAY_SIZE * sizeof(int *));
    for (int i = 0; i < ARRAY_SIZE; i++) {
        large_array[i] = (int *)malloc(ARRAY_SIZE * sizeof(int));
        for (int j = 0; j < ARRAY_SIZE; j++) {
            large_array[i][j] = rand() % 100; // Initialize with random values
        }
    }
    
    // Start time measurement
    start = clock();
    
    // Create threads to traverse memory
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, memory_traverse, &thread_ids[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // End time measurement
    end = clock();
    time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("Execution Time with %d threads: %f seconds\n", NUM_THREADS, time_taken);
    
    // Free allocated memory
    for (int i = 0; i < ARRAY_SIZE; i++) {
        free(large_array[i]);
    }
    free(large_array);
    
    return 0;
}
