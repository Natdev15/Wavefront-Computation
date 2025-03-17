#include <iostream>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include <vector>
#include "./matrix_utils.hpp"

// Compute a checksum of the matrix (sum of all elements)
double compute_checksum(const std::vector<double>& matrix, int n) {
    double sum = 0.0;
    for (int i = 0; i < n * n; i++) {
        sum += matrix[i];
    }
    return sum;
}

// Wavefront computation cube root: store the raw dot product in the matrix.
void seq_wavefront_computation(double* matrix, int n) {
    for (int k = 1; k < n; k++) {            // Loop over diagonals
        for (int m = 0; m < n - k; m++) {    // For each element in the diagonal
            double res = 0.0;
            for (int i = 0; i < k; i++) {
                res += matrix[m * n + (m + i)]
                     * matrix[(m + 1 + i) * n + (m + k)];
            }
            // store cube root of dot product
            matrix[m * n + (m + k)] = std::cbrt(res);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: matrix_size argument missing.\nUsage: " 
                  << argv[0] << " <matrix_size>" << std::endl;
        return -1;
    }
    int n = std::atoi(argv[1]);
    std::vector<double> matrix(n * n, 0.0);

    // Initialize matrix (main diagonal) via utility function
    initialize_diagonal(matrix.data(), n);

    auto start = std::chrono::high_resolution_clock::now();
    seq_wavefront_computation(matrix.data(), n);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Elapsed Time: " << duration.count() << " ms" << std::endl;

    double checksum = compute_checksum(matrix, n);
    std::cout << "Checksum: " << checksum << std::endl;

    // Print the top-right element
    std::cout << "Top-right element: M[0][" << (n - 1) << "] = "
              << matrix[0 * n + (n - 1)] << std::endl;

    // If the matrix is small (e.g., 4×4), print the entire matrix
    if (n == 4) {
        std::cout << "Matrix contents:" << std::endl;
        print_matrix(matrix.data(), n);
    }

    return 0;
}

