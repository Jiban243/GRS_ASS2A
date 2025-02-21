Multithreaded Workload Programs

Overview

This repository contains four multithreaded programs implemented using the pthread library, each representing a different workload type:

CPU-bound: Performs intensive mathematical computations (matrix multiplication and prime number computation).

Memory-bound: Operates on a large 2D array, stressing memory bandwidth.

I/O-bound: Involves frequent file read/write operations to simulate disk stress.

Mixed Workload: Combines CPU, memory, and I/O operations in a single program.

Compilation

A Makefile is provided to compile all programs efficiently. To build the executables, run:
make

To compile a specific program, run:
make cpu_bound      # For CPU-bound workload
make memory_bound   # For Memory-bound workload
make io_bound       # For I/O-bound workload
make mixed_workload # For Mixed workload

Execution

After compilation, run the programs using:
./cpu_bound
./memory_bound
./io_bound
./mixed_workload

Each program will output execution time for performance analysis.

Profiling with perf

To analyze performance metrics, use the perf tool:
perf stat -e cycles,instructions,cache-misses,context-switches ./cpu_bound
perf stat -e cache-references,cache-misses,mem-loads,mem-stores ./memory_bound
perf stat -e context-switches,cpu-migrations,page-faults ./io_bound
perf stat -e cycles,instructions,cache-misses,mem-loads,context-switches ./mixed_workload

Cleanup

To remove compiled binaries, run:
make clean

Author

Developed as part of a multithreading performance analysis project.

