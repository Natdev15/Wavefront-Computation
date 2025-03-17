#!/bin/bash
#SBATCH --job-name=slurm_computation
#SBATCH --output=slurm_output.txt
#SBATCH --error=slurm_error.txt
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=00:30:00


MATRIX_SIZE=400
WORKERS=$SLURM_NTASKS  


echo "Running Sequential version:"
./sequential $MATRIX_SIZE

echo "Running FastFlow version:"
./ff_parallelfor $MATRIX_SIZE $WORKERS

echo "Running MPI version:"
# srun will use the SLURM-assigned number of processes.
srun --mpi=pmix -n $SLURM_NTASKS ./mpi $MATRIX_SIZE



