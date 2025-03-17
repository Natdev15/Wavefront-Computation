#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include "./matrix_utils.hpp" 

// Compute checksum: sum of all matrix elements.
double compute_checksum(const std::vector<double>& M, int n) {
    double sum = 0.0;
    for (int i = 0; i < n * n; i++) {
        sum += M[i];
    }
    return sum;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);  // Initialize MPI

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 2) {
        if (rank == 0) {
            std::cerr << "Error: matrix_size argument missing.\nUsage: "
                  << argv[0] << " <matrix_size>" << std::endl;
        }
        MPI_Finalize();
        return -1;
    } 
  
    int n = std::atoi(argv[1]); // matrix dimension
    std::vector<double> matrix(n * n, 0.0);


    // initialize_diagonalMPI(matrix, n);
    initialize_diagonal(matrix.data(), n);


    double start_time = MPI_Wtime(); // Start timing

    // Loop over each diagonal offset k = 1,2,..., n-1.
    for (int k = 1; k < n; ++k) {
        int total_elements = n - k;   // number of elements in the current diagonal

        // Compute a simple "chunk size": if there's a remainder, increase chunk size.
        int chunk_size = total_elements / size;
        if (total_elements % size > 0) chunk_size++;

        // Local results array for the diagonal elements computed by this process
        std::vector<double> local_results;
        int m_start = rank * chunk_size;
        int m_end = std::min(m_start + chunk_size, n - k);
        int local_count = std::max(0, m_end - m_start); // Ensure non-negative

        // Each process computes its part (if any)
        if (local_count > 0) {
            local_results.resize(local_count, 0.0);
            for (int m = m_start; m < m_end; m++) {
                double res = 0.0;
                for (int i = 0; i < k; ++i) {
                    // Compute the dot-product for the first k elements along the row and column.
                    res += matrix[m * n + (m + i)] * matrix[(m + 1 + i) * n + (m + k)];
                }
                local_results[m - m_start] = std::cbrt(res);
            }
        }

        // Prepare arrays for MPI_Allgatherv:
        std::vector<int> recvcounts(size, 0);
        std::vector<int> displs(size, 0);
        for (int i = 0; i < size; ++i) {
            int m_s = i * chunk_size;
            int m_e = std::min(m_s + chunk_size, n - k);
            recvcounts[i] = std::max(0, m_e - m_s);
            displs[i] = m_s;
        }

        // Gather all local results to all processes.
        std::vector<double> gathered_results(total_elements);
        MPI_Allgatherv(
            local_results.data(),  // Data to send from this process
            local_count,           // Number of elements to send
            MPI_DOUBLE,            // Data type
            gathered_results.data(),  // Receive buffer
            recvcounts.data(),        // Number of elements received from each process
            displs.data(),            // Displacement for each process's data
            MPI_DOUBLE,
            MPI_COMM_WORLD
        );

        // Update the matrix with the gathered results (all processes do the same update)
        for (int m = 0; m < total_elements; ++m) {
            matrix[m * n + (m + k)] = gathered_results[m];
        }
    }
    // End timing.
    double end_time = MPI_Wtime();

    if (rank == 0) {
        double elapsed_ms = (end_time - start_time) * 1000;
        std::cout << "Elapsed time: " << elapsed_ms << " ms" << std::endl;
        std::cout << "Checksum: " << compute_checksum(matrix, n) << std::endl;
        std::cout << "Last element: M[0][" << n - 1 << "] = "
                  << matrix[0 * n + (n - 1)] << std::endl;

        // Print matrix when n == 4.
        if (n == 4) {
            std::cout << "Matrix contents:" << std::endl;
            print_matrix(matrix.data(), n);
        }
    }

    MPI_Finalize();
    return 0;
}

