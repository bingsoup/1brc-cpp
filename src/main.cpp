#include <iostream>
#include <string>
#include <chrono>

#include "1brc/finn.hpp"

int main(int argc, char *argv[]) {
    std::string path = "measurements.txt";
    int totalRuns = 1;
    int runNum = 1;

    if (argc == 2){
        path = argv[1];
        totalRuns = std::stoi(argv[2]);
    }
    
    while (runNum <= totalRuns)
    {
        std::cout << "Run: " << runNum << '\n';
        const auto startTime = std::chrono::high_resolution_clock::now();
        solution(path);
        const auto endTime = std::chrono::high_resolution_clock::now();
        const auto seconds =
            std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
        const auto ms = 
            std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        std::cout << "Time taken:\n" 
            << seconds.count() << " seconds\n" 
            << ms.count() << " milliseconds\n";
        runNum++;
    }

    return 0;
}