#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#define MATRIX_SIZE 1000   // Memory-bound task
#define MAX_NUMBER 100000  // CPU-bound task limit
#define LINES_PER_FILE 5000 // I/O-bound task

int **matrix;
pthread_mutex_t mutex;
int NUM_THREADS;  // Thread count from command line

// CPU-bound: Prime number computation
int is_prime(int num) {
    if (num < 2) return 0;
    for (int i = 2; i <= sqrt(num); i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}
void *compute_primes(void *arg) {
    int thread_id = *(int *)arg;
    int count = 0;
    for (int i = thread_id * (MAX_NUMBER / NUM_THREADS); i < (thread_id + 1) * (MAX_NUMBER / NUM_THREADS); i++) {
        if (is_prime(i)) {
            pthread_mutex_lock(&mutex);
            count++;
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_exit(NULL);
}

// Memory-bound: Large matrix traversal
void *memory_traverse(void *arg) {
    int thread_id = *(int *)arg;
    int start = (MATRIX_SIZE / NUM_THREADS) * thread_id;
    int end = (MATRIX_SIZE / NUM_THREADS) * (thread_id + 1);
    for (int i = start; i < end; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrix[i][j] += 1; // Simulate heavy memory access
        }
    }
    pthread_exit(NULL);
}

// I/O-bound: File read/write operations
void *file_operations(void *arg) {
    int thread_id = *(int *)arg;
    char filename[20];
    sprintf(filename, "file_%d.txt", thread_id);
    
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("File open error");
        pthread_exit(NULL);
    }
    for (int i = 0; i < LINES_PER_FILE; i++) {
        fprintf(fp, "Thread %d: Line %d\n", thread_id, i);
    }
    fclose(fp);

    fp = fopen(filename, "r");
    if (!fp) {
        perror("File read error");
        pthread_exit(NULL);
    }
    char buffer[100];
    while (fgets(buffer, sizeof(buffer), fp)) {}
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

    pthread_t *threads = malloc(NUM_THREADS * 3 * sizeof(pthread_t));
    int *thread_ids = malloc(NUM_THREADS * sizeof(int));
    clock_t start, end;
    double time_taken;

    srand(time(NULL));
    pthread_mutex_init(&mutex, NULL);

    // Allocate memory for matrix
    matrix = malloc(MATRIX_SIZE * sizeof(int *));
    for (int i = 0; i < MATRIX_SIZE; i++) {
        matrix[i] = malloc(MATRIX_SIZE * sizeof(int));
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrix[i][j] = rand() % 100;
        }
    }

    start = clock();
    
    // Create threads for CPU, memory, and I/O tasks
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, compute_primes, &thread_ids[i]);
        pthread_create(&threads[i + NUM_THREADS], NULL, memory_traverse, &thread_ids[i]);
        pthread_create(&threads[i + (2 * NUM_THREADS)], NULL, file_operations, &thread_ids[i]);
    }
    for (int i = 0; i < NUM_THREADS * 3; i++) {
        pthread_join(threads[i], NULL);
    }
    
    end = clock();
    time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Execution Time with %d threads: %f seconds\n", NUM_THREADS, time_taken);
    
    // Cleanup
    pthread_mutex_destroy(&mutex);
    free(threads);
    free(thread_ids);
    for (int i = 0; i < MATRIX_SIZE; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}
