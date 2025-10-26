#!/bin/bash

#SBATCH --nodes=7
#SBATCH --ntasks-per-node=10
#SBATCH --time=6:00
#SBATCH --mail-type=END,FAIL
#SBATCH --account=PCS0294
#SBATCH --job-name=MPIish2

module load mvapich/3.0
module load boost/1.83.0

#module load intel
cd $SLURM_SUBMIT_DIR/
make clean && make OSC



    mpiexec -n 8 mpi_vector






