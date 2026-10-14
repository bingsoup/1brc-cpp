#include <iostream>
#include <chrono>

#include "1brc/finn.hpp"

int main(int argc, char *argv[]) {
    std::string path = "measurements.txt";
    int totalRuns = 1;
    int runNum = 1;
    int64_t min = 0;
    int64_t max = 0;
    int avg = 0;

    if (argc == 3){
        path = argv[1];
        totalRuns = std::stoi(argv[2]);
    }
    
    while (runNum <= totalRuns)
    {
        std::cout << "Run: " << runNum << '\n';

        const auto startTime = std::chrono::high_resolution_clock::now();
        
        solution(path);
        
        const auto endTime = std::chrono::high_resolution_clock::now();
        const auto ms = 
            std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        const auto s =
            std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);

        std::cout << "Time taken:\n" 
            << ms.count() << " milliseconds\n"
            << s.count() << " seconds\n";
        
        if (runNum == 1){
            min = ms.count();
            max = ms.count();
        }
        min = std::min(min,ms.count());
        max = std::max(max,ms.count());
        avg += ms.count();

        runNum++;
    }

    avg /= totalRuns;
    std::cout << "Results:\n" 
            << "Min: " << min << "/"
            << "Avg: " << avg << "/"
            << "Max: " << max << "\n";

    return 0;
}