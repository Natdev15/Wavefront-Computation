#include "./fastflow/ff/parallel_for.hpp"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include "./matrix_utils.hpp"


// Compute a checksum of the matrix (sum of all elements).
double compute_checksum(double* matrix, int n) {
    double sum = 0.0;
    for (int i = 0; i < n * n; i++) {
        sum += matrix[i];
    }
    return sum;
}

// wavefront computation using the fastFlow parallel_for abstraction.
void parallel_for_wavefront(double* matrix, int n, int num_workers) {
    
    if (num_workers == 1) {
        for (int k = 1; k < n; k++) {
            for (int m = 0; m < n - k; m++) {
                double sum = 0.0;
                for (int j = 0; j < k; j++) {
                    sum += matrix[m * n + (m + j)] *
                           matrix[(m + 1 + j) * n + (m + k)];
                }
                matrix[m * n + (m + k)] = std::cbrt(sum);
            }
        }
        return;
    }

    // use fastFlow's parallel_for 
    ff::ParallelFor pf(num_workers);

    for (int k = 1; k < n; k++) {
        long total_tasks = n - k; // Number of elements (tasks) in the current diagonal.
        pf.parallel_for(0L, total_tasks, 1, [=](long m) {
            double sum = 0.0;
            for (int j = 0; j < k; j++) {
                sum += matrix[m * n + (m + j)] *
                       matrix[(m + 1 + j) * n + (m + k)];
            }
            matrix[m * n + (m + k)] = std::cbrt(sum);
        });
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Error: matrix_size and num_workers argument missing.\nUsage: " 
		  << argv[0] << " <matrix_size> <num_workers>" << std::endl;
        return -1;
    }
    int n = std::atoi(argv[1]);
    int user_workers = std::atoi(argv[2]);

    const int MAX_THREADS = 32;
    int num_workers = std::min(user_workers, MAX_THREADS);

    if(user_workers > MAX_THREADS) {
        std::cout << "Warning: Requested " << user_workers 
                  << " workers, but only " << MAX_THREADS 
                  << " threads are allowed. Using " << MAX_THREADS 
                  << " workers instead." << std::endl;
    }

    // Allocate and initialize the matrix.
    double* matrix = (double*)std::calloc(n * n, sizeof(double));
    if (!matrix) {
        std::cerr << "Error: Unable to allocate matrix." << std::endl;
        return -1;
    }
    initialize_diagonal(matrix, n);

    // Time the wavefront computation.
    auto start = std::chrono::high_resolution_clock::now();
    parallel_for_wavefront(matrix, n, num_workers);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    std::cout << "Elapsed time: " << duration.count() << " ms" << std::endl;

    // Compute and output checksum and top-right element.
    double checksum = compute_checksum(matrix, n);
    std::cout << "Checksum: " << checksum << std::endl;
    std::cout << "Top-right element: " << matrix[0 * n + (n - 1)] << std::endl;
    if (n == 4) {
        std::cout << "Matrix contents:" << std::endl;
        print_matrix(matrix, n);
    }

    std::free(matrix);
    return 0;
}

