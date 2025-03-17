Wavefront Computation:
Wavefront Computation is a parallel application implemented in C++ that performs dependency-based computations following a wavefront pattern. This project leverages a hybrid parallel programming model:

Shared Memory Parallelism using FastFlow
Distributed Memory Parallelism using MPI
It was developed as part of my exam project in the SPM (Parallel and Distributed Paradigm) course.

Features:
Hybrid Parallelism: Combines shared memory (via FastFlow) and distributed memory (via MPI) techniques.
Wavefront Pattern: Computes data along diagonals (wavefronts) where each computation depends on the previous one—ideal for dynamic programming, simulations, or matrix-based computations.
Scalability: Designed to scale across multi-core machines and distributed clusters.
Performance Optimization: Efficient scheduling and communication strategies to optimize both intra-node and inter-node performance.

Requirements:
C++ Compiler: Supporting C++20
FastFlow Library: For shared memory parallelism (FastFlow GitHub)
MPI Library: Such as OpenMPI or MPICH
CMake: For building the project (optional but recommended)


Install Dependencies:
Download and install FastFlow, or include its headers in the project
Fastflow Library Github: https://github.com/fastflow/fastflow/tree/master/ff
Ensure an MPI library (e.g., OpenMPI) is installed and available (mpicc/mpic++).


Implementation Details
Wavefront Algorithm: The computation follows a wavefront pattern where each element is computed after its dependencies (typically the elements from the previous diagonal) are ready.
Hybrid Approach: The program dynamically selects between MPI and FastFlow depending on whether it is running on a multi-node cluster or a single multi-core system.
Synchronization: MPI is responsible for synchronizing boundaries between nodes, while FastFlow handles parallelism within a node.


Acknowledgments
FastFlow Library: For providing a robust shared memory parallel framework.
MPI Community: For the scalable distributed computing tools.
Developed as part of the SPM Parallel and Distributed Paradigm exam.

