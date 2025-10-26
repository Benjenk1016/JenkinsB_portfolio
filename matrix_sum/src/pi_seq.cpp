#include <random>
#include <sstream>
#include <iostream>
#include <mpi.h>

//modifed for MPI
long long countInside(int nSamples, std::mt19937_64 &rng) { //pass rng by reference to avoid copying
    std::uniform_real_distribution<double> dist(-1.0, 1.0); 
    long long numInCircle = 0;
    for (int i = 0; i < nSamples; i++) {
        double x = dist(rng);
        double y = dist(rng);
        if (x*x + y*y <= 1.0) numInCircle++;
    }
    return numInCircle;
}


int main() {
    
MPI_Init(NULL, NULL);
int myRank, P;
MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
MPI_Comm_size(MPI_COMM_WORLD, &P);

//defining variables
long long N = 100000; //number of samples
unsigned long long base_seed = 12345; //base seed

local_N = N / P;
if (myRank == 0) local_N += N % P;//distribute remainder if needed

std::mt19937_64 rng(base_seed + myRank); //makes differnert rng for each process based on rank
double start = MPI_Wtime(); // start timer
long long local_inside = countInside(local_N, rng); //local count
double local_time = MPI_Wtime() - start; //time for local computation

long long total_inside;
double walltime;
MPI_Reduce(&local_inside, &total_inside, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
MPI_Reduce(&local_time, &walltime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

if (myRank == 0) {
    double pi = 4.0 * (double) total_inside / N;
    std::cout << "Pi ≈ " << pi << ", Walltime = " << walltime << "s\n";
}

MPI_Finalize();

    
}


