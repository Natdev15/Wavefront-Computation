#ifndef MATRIX_UTILS_HPP
#define MATRIX_UTILS_HPP

#include <cstdio>
#include <cmath>


// Initializes the matrix diagonal: M[i][i] = (i+1)/n.
inline void initialize_diagonal(double* matrix, int n) {
    for (int i = 0; i < n; ++i) {
        matrix[i * n + i] = static_cast<double>(i + 1) / n;
    }
}

// Prints the matrix in a formatted way.
inline void print_matrix(double* matrix, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; j++) {
            printf("%6.2f ", matrix[i * n + j]);
        }
        printf("\n");
    }
}

#endif // MATRIX_UTILS_HPP

