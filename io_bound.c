#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define LINES_PER_FILE 10000  // Number of lines per file

int NUM_THREADS;  // Global variable to hold thread count

void *file_operations(void *arg) {
    int thread_id = *(int *)arg;
    char filename[20];
    sprintf(filename, "file_%d.txt", thread_id);
    
    // Write to file
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("File open error");
        pthread_exit(NULL);
    }
    for (int i = 0; i < LINES_PER_FILE; i++) {
        fprintf(fp, "Thread %d: Line %d\n", thread_id, i);
    }
    fclose(fp);
    
    // Read from file
    fp = fopen(filename, "r");
    if (!fp) {
        perror("File read error");
        pthread_exit(NULL);
    }
    char buffer[100];
    while (fgets(buffer, sizeof(buffer), fp)) {
        // Simulate processing the read data
    }
    fclose(fp);
    
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
    
    // Start time measurement
    start = clock();
    
    // Create threads for file operations
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, file_operations, &thread_ids[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // End time measurement
    end = clock();
    time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("Execution Time with %d threads: %f seconds\n", NUM_THREADS, time_taken);
    
    return 0;
}
