#include <iostream>
#include <vector>
#include <chrono>



std::vector<double> multiplyMatrixVector(const std::vector<std::vector<double>>& matrix, const std::vector<double>& vector) {
    int numRows = matrix.size();
    int numCols = matrix[0].size(); // Assuming a non-empty matrix
    int vectorSize = vector.size();

    if (numCols != vectorSize) {
        std::cerr << "Error: Matrix columns must equal vector rows for multiplication." << std::endl;
        return {}; // Return empty vector on error
    }

    std::vector<double> resultVector(numRows, 0.0);
    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            resultVector[i] += matrix[i][j] * vector[j];
        }
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = end_time - start_time;
    long long duration_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return resultVector;
}

int main(){
    

    // Example Matrix (10x10), no specific pattern, just arbitrary numbers
     std::vector<std::vector<double>> matrix = {
        {44, 7, 85, 63, 21, 34, 55, 89, 13, 2},
        {14, 28, 35, 49, 56, 70, 77, 91, 105, 112},
        {3, 6, 9, 12, 15, 18, 21, 24, 27, 30},
        {11, 22, 33, 44, 55, 66, 77, 88, 99, 110},
        {5, 10, 15, 20, 25, 30, 35, 40, 45, 50},
        {8, 16, 24, 32, 40, 48, 56, 64, 72, 80},
        {13, 26, 39, 52, 65, 78, 91, 104, 117, 130},
        {17, 34, 51, 68, 85, 102, 119, 136, 153, 170},
        {19, 38, 57, 76, 95, 114, 133, 152, 171, 190},
        {23, 46, 69, 92, 115, 138, 161, 184, 207, 230}
    };
    // Example Vector (10x1), the numbers are arbitrary
    std::vector<double> vector = {7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0};


    //start timer
     auto start_time = std::chrono::high_resolution_clock::now();
    //serial process of matrix-vector multiplication



    std::vector<double> result = multiplyMatrixVector(matrix, vector);

    //stop timer
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = end_time - start_time;
        long long duration_in_nano = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();



    // Print the result vector
    if (!result.empty()) {
        std::cout << "Resultant Vector:" << std::endl;
        for (double val : result) {
            std::cout << val << std::endl;
        }
    }
    std::cout << "Time taken for multiplication: " << duration_in_nano << " nanoseconds" << std::endl;

    return 0;

}