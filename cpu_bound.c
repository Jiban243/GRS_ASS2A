#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#define MATRIX_SIZE 500  // Adjust size for workload
#define MAX_NUMBER 100000 // Limit for prime computation

int **matrix1, **matrix2, **result;
int NUM_THREADS;
pthread_mutex_t mutex;

void *matrix_multiply(void *arg) {
    int thread_id = *(int *)arg;
    int start = (MATRIX_SIZE / NUM_THREADS) * thread_id;
    int end = (MATRIX_SIZE / NUM_THREADS) * (thread_id + 1);
    for (int i = start; i < end; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            result[i][j] = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    pthread_exit(NULL);
}

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

    srand(time(NULL));
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    clock_t start, end;
    double time_taken;

    // Allocate matrices dynamically
    matrix1 = (int **)malloc(MATRIX_SIZE * sizeof(int *));
    matrix2 = (int **)malloc(MATRIX_SIZE * sizeof(int *));
    result = (int **)malloc(MATRIX_SIZE * sizeof(int *));
    for (int i = 0; i < MATRIX_SIZE; i++) {
        matrix1[i] = (int *)malloc(MATRIX_SIZE * sizeof(int));
        matrix2[i] = (int *)malloc(MATRIX_SIZE * sizeof(int));
        result[i] = (int *)malloc(MATRIX_SIZE * sizeof(int));
    }

    // Initialize matrices with random values
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrix1[i][j] = rand() % 10;
            matrix2[i][j] = rand() % 10;
        }
    }

    pthread_mutex_init(&mutex, NULL);

    // Start time measurement
    start = clock();

    // Create threads for matrix multiplication
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, matrix_multiply, &thread_ids[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Create threads for prime number computation
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, compute_primes, &thread_ids[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // End time measurement
    end = clock();
    time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("Execution Time: %f seconds with %d threads\n", time_taken, NUM_THREADS);

    pthread_mutex_destroy(&mutex);

    // Free dynamically allocated memory
    for (int i = 0; i < MATRIX_SIZE; i++) {
        free(matrix1[i]);
        free(matrix2[i]);
        free(result[i]);
    }
    free(matrix1);
    free(matrix2);
    free(result);

    return 0;
}
