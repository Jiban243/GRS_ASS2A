#!/bin/bash

# Define programs and thread counts
PROGRAMS=("cpu_bound" "memory_bound" "io_bound" "mixed_workload")
THREADS_LIST=(2 4 8 10 50 100)

# Create a results directory
RESULTS_DIR="perf_results"
mkdir -p $RESULTS_DIR

# Run perf analysis for each program with different thread counts
for program in "${PROGRAMS[@]}"; do
    if [[ ! -x ./$program ]]; then
        echo "Error: $program is not executable. Skipping..."
        continue
    fi
    
    echo "==== Profiling $program ===="
    for threads in "${THREADS_LIST[@]}"; do
        echo "Running $program with $threads threads..."
        
        # Run perf and save output
        perf stat -e cycles,instructions,cache-references,cache-misses,context-switches,cpu-migrations,page-faults -r 5 ./$program $threads 2>&1 | tee $RESULTS_DIR/perf_${program}_${threads}.txt
    done
done

echo "Performance analysis completed. Check results in $RESULTS_DIR."
