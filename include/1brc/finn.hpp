#ifndef OBRC_USERNAME_HPP
#define OBRC_USERNAME_HPP

#include <fstream>
#include <unordered_map>
#include <thread>
#include <filesystem>
#include <charconv>

struct station{
    int min = 1000;
    int max = -1000;
    int sum = 0;
    int elements = 0;
};

using uMap = std::unordered_map<std::string, station>;

inline void processLine(const std::string& line, uMap& results){
    size_t delimiter_pos = line.find(';');

    if (delimiter_pos == std::string::npos) {
            return;
    }

    std::string station_name = line.substr(0, delimiter_pos);
    std::string number = line.substr(delimiter_pos +1);
    number.erase(number.find("."),1);
    int temperature;
    auto [ptr, ec] = std::from_chars(
        number.data(),
        number.data() + number.size(),
        temperature
    );
    
    station& curStation = results[station_name];

    curStation.elements++;

    if (temperature < curStation.min) {
        curStation.min = temperature;
    }
    if (temperature > curStation.max) {
        curStation.max = temperature;
    }

    curStation.sum += temperature;
}

inline void processChunk(
    const std::string& filename, 
    std::streamoff start, 
    std::streamoff end, 
    uMap& results) 
    {
    std::ifstream inputFile(filename, std::ios::binary);
    inputFile.seekg(start);

    // Ensure line isn't partial
    if (start > 0) {
        inputFile.seekg(start-1);

        char previous;
        inputFile.get(previous);

        if (previous != '\n'){
            std::string ignored;
            std::getline(inputFile,ignored);
        }
    }

    auto position = inputFile.tellg();
    std::string line;

    while (position < end && std::getline(inputFile,line)) {
        processLine(line, results);
        position += static_cast<std::streamoff>(line.size()) + 1;
    }
}

// Don't change signature of this function, feel free to change everything else
inline void solution(const std::string& filename) {
    std::ifstream inputFile(filename); 
    size_t fileSize = std::filesystem::file_size(filename);

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << "\n";
        return;
    }

    uMap stations1{};
    uMap stations2{};
    stations1.reserve(413);
    std::vector<std::string> keys;

    std::string line;
    
    std::thread worker(processChunk, 
        std::cref(filename),
        static_cast<std::streamoff>(fileSize / 2),
        static_cast<std::streamoff>(fileSize),
        std::ref(stations2)
    );

    processChunk(filename, 0, fileSize / 2, stations1);

    worker.join();

    inputFile.close();

    std::ofstream outputFile("output.txt");

    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file \n";
        return;
    }

    keys.reserve(stations1.size());
    // Merge station maps
    for (auto& it : stations2) {
        station& cStation2 = stations2[it.first];
        station& cStation = stations1[it.first];
        keys.push_back(it.first);
        cStation.elements += cStation2.elements;
        cStation.min = std::min(cStation.min, cStation2.min);
        cStation.max = std::max(cStation.max, cStation2.max);
        cStation.sum += cStation2.sum;
    }

    std::sort (keys.begin(), keys.end());

    for (const auto& name : keys) {
        station& currentStation = stations1[name];
        float average = (currentStation.sum / 10.0f) / currentStation.elements;
        float min = currentStation.min / 10.0f;
        float max = currentStation.max / 10.0f;
        outputFile << std::fixed << std::setprecision(1) << name << ";" << min << ";" << average << ";" << max << "\n";
    }
}
#endif