# Parallelism

This repository contains a small parallel computing project that demonstrates serial and MPI-based matrix-vector multiplication.

## Project Structure

- `Default/`
  - `makefile` – build script for the MPI program
  - `jobScript.slurm` – example Slurm batch script for running the MPI executable on a cluster
  - `mpi_vector` – built MPI executable (if present)
- `data/`
  - `mpi_result.csv` – benchmark results produced by MPI runs
  - `data-table.png` – result visualization image
- `src/`
  - `mpi_vector.cpp` – MPI implementation of matrix-vector multiplication
  - `serial_vector_multi.cpp` – serial implementation of matrix-vector multiplication
  - `serial_vector_multi` – built serial executable (if present)
- `README.md` – project overview, build instructions, and usage notes

## Overview

The serial program performs matrix-vector multiplication in a single process. The MPI program splits the matrix by rows, broadcasts the vector to each process, and gathers the partial results to produce the final vector.

Timing for the MPI implementation is measured around the parallel computation phase, with barriers to synchronize processes before and after the timed section.

## Build Instructions

### Build the MPI program

Open a terminal in the project root and run:

```powershell
cd Default
make
```

This compiles `src/mpi_vector.cpp` with `mpic++` and produces the `mpi_vector` executable in the `Default/` directory.

### Build the serial program

From the project root, run a standard compiler command such as:

```powershell
g++ -O2 -std=c++11 -o src/serial_vector_multi src/serial_vector_multi.cpp
```

## Run Instructions

### Run the serial version

```powershell
cd src
./serial_vector_multi
```

### Run the MPI version

Use `mpirun` or `mpiexec`, for example:

```powershell
cd Default
mpirun -np 4 ./mpi_vector
```

Or use the provided Slurm script:

```powershell
sbatch Default/jobScript.slurm
```

## Notes

- The MPI implementation uses row distribution with `Scatterv`/`Gatherv` and a broadcast for the input vector.
- For larger matrix sizes, watch memory usage carefully.
- The current serial version uses a fixed example matrix and vector for demonstration.

## Status

The repository is now structured at the root with source code, build scripts, and benchmark outputs available directly from the project root.
