#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <mpi.h>
#include <fstream>
#include <random>
#include <sstream>
#include <iomanip>







// Fill matrix (size x size) and vector (size) with random whole numbers in [minVal,maxVal].
void generateMatrixAndVector(int size, std::vector<std::vector<double>>& matrix, std::vector<double>& vec) {
    std::random_device rd;
    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> dist(0, 1000);
    matrix.assign(size, std::vector<double>(size));
    vec.assign(size, 0.0);
    for (int i = 0; i < size; ++i) {
        vec[i] = static_cast<double>(dist(rng));
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = static_cast<double>(dist(rng));
        }
    }
}


//compare the parallel runs to the serial run to make sure the results are always the same and correct.

void multiplyMatrixVector(int matrixSize) {
  
    //create matrix and vector here for testing
    std::vector<std::vector<double>> matrix;
    std::vector<double> x;
    double start_time, end_time;

    generateMatrixAndVector(matrixSize, matrix, x); 

 
 start_time = MPI_Wtime();
 
 //computaion section
     std::vector<double> resultVector(matrixSize, 0.0);
     for (int i =0; i < matrixSize; ++i){
        for (int j=0; j < matrixSize; ++j){
            resultVector[i] += matrix[i][j] * x[j];
        }
     }

//stop timer
    end_time = MPI_Wtime();
    double elapsed_time = end_time - start_time;

//write to file
    std::ofstream out("../data/mpi_result.csv", std::ios::app);
    out << "MPI Serial Results:\n";
    out << "Walltime: " << elapsed_time << "s" << std::endl;
    
    out.close();
}

//making each process calculate each row. With the possible remainding rows going to the
//the next processes. Using the displacement idea will make sure the 
void mpiParallized(int matrixSize){
    double start_time, end_time, local_elapsed, max_elasped;
    int numProcs =0, myRank = 0;
    int numRows = matrixSize; //problem size is number of rows
    int numCols = matrixSize; // 
    int vectorSize = matrixSize; 
    std::vector<std::vector<double>> matrix;
    std::vector<double> x;
    generateMatrixAndVector(matrixSize, matrix, x); 

    
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs); 
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank); 

    //use master to divide the work
    std::vector<int> counts(numProcs), displs(numProcs);//holds number for each process

    // compute counts/displacements (rows per proc)
    int base = (numProcs>0) ? (numRows / numProcs) : 0;
    int rem = (numProcs>0) ? (numRows % numProcs) : 0;
    for (int r = 0; r < numProcs; ++r) {
        counts[r] = base + (r < rem ? 1 : 0);
        displs[r] = (r==0) ? 0 : (displs[r-1] + counts[r-1]);
    }

    // flatten matrix on root for Scatterv
    std::vector<double> flatA;
    if (myRank == 0) {
        flatA.resize(static_cast<size_t>(numRows) * std::max(1, numCols));
        for (int i = 0; i < numRows; ++i)
            for (int j = 0; j < numCols; ++j)
                flatA[i * numCols + j] = matrix[i][j];
    }

    // sendCounts/displs in units of elements (doubles)
    std::vector<int> sendCounts(numProcs), sendDispls(numProcs);
    for (int r = 0; r < numProcs; ++r) {
        sendCounts[r] = counts[r] * numCols;
        sendDispls[r] = displs[r] * numCols;
    }

    int localRows = counts[myRank];
    int recvCountElems = localRows * numCols;
    std::vector<double> myMatrix(static_cast<size_t>(std::max(1, recvCountElems)));

    // Safe scatter: root supplies buffer, others pass nullptr
    MPI_Scatterv((myRank==0) ? flatA.data() : nullptr, sendCounts.data(), sendDispls.data(), MPI_DOUBLE,
                 (recvCountElems>0) ? myMatrix.data() : nullptr, recvCountElems, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Broadcast the vector from root to everyone
    MPI_Bcast(x.data(), vectorSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);



    //make all processes are ready to compute before starting timer to only evaluate compute time.
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();
    
    
    
    // each process computes its own row
    std::vector<double> localY(localRows, 0.0);
    for (int i = 0; i < localRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            localY[i] += myMatrix[i * numCols + j] * x[j];
        }
    }

    // gather results at master
    std::vector<double> globaly;
    if (myRank == 0) globaly.resize(numRows);
    MPI_Gatherv((localRows>0) ? localY.data() : nullptr, localRows, MPI_DOUBLE,
                (myRank==0) ? globaly.data() : nullptr, counts.data(), displs.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    
   //end computation timer
    end_time = MPI_Wtime();
    local_elapsed = end_time - start_time;
    //use reduce to get the longest time among all processes.
    MPI_Reduce(&local_elapsed, &max_elasped, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    
    //write times, numProcs, and input size to mpi_results.csv
    if (myRank == 0) {
        std::ofstream out("../data/mpi_result.csv", std::ios::app);
        out << std::endl << "MPI Parallel Results:\n";
        out << "Processes - " << numProcs << " Processes\n";
        out << "Walltime - " << max_elasped << "s" << std::endl;
        //this could be for displaying the first 2 ele. x for each process
        /*for (int i = 0; i <= numProcs; ++i){
            out << "Rank " << i << globaly[displs[i]] << ", " << globaly[displs[i]+1] <<  std::endl;
            }
        */
        
        out.close(); 
    }
}


int main(){
   
    int myRank;
    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    
    //size of generated matrix 
    int matrixSize = 25000;
        // compute serial result on master and time it
        

    mpiParallized(matrixSize);

    MPI_Finalize();

    return 0;

}
