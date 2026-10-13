#ifndef OBRC_USERNAME_HPP
#define OBRC_USERNAME_HPP

#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>

struct station{
    std::vector<float> data;
    float min = 100.0f;
    float max = -100.0f;
};

// Don't change signature of this function, feel free to change everything else
inline void solution(const std::string& filename) {
    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << "\n";
        return;
    }

    std::unordered_map<std::string, station> stations{};

    std::string line;
    
    while (std::getline(inputFile, line)) {
        size_t delimiter_pos = line.find(';');

        if (delimiter_pos == std::string::npos) {
            break;
        }

        std::string station_name = line.substr(0, delimiter_pos);
        float temperature = std::stof(line.substr(delimiter_pos + 1));
        
        station& curStation = stations[station_name];

        curStation.data.push_back(temperature);
        if (temperature < curStation.min) {
            curStation.min = temperature;
        }
        if (temperature > curStation.max) {
            curStation.max = temperature;
        }
    }

    inputFile.close();

    std::ofstream outputFile("output.txt");

    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file \n";
        return;
    }

    for (const auto& [station_name, station_data] : stations) {
        float sum = 0.0f;
        for (float temp : station_data.data) {
            sum += temp;
        }
        float average = sum / station_data.data.size();
        outputFile << std::fixed << std::setprecision(1) << station_name << ";" << station_data.min << ";" << average << ";" << station_data.max << "\n";
    }

}
#endif