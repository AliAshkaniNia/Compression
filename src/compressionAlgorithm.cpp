#include "compressionAlgorithm.h"
#include <fstream>
#include <iostream>
    
int CompressionAlgorithm::saveToFile(const std::string& filename, const std::string& data){
    std::ostream* output;
    std::ofstream outputFile;
    if (!filename.empty()) {
        outputFile.open(filename);
        if (!outputFile) {
            std::cerr << "Error opening output file: " << filename << std::endl;
            return 1;
        }
        output = &outputFile;
    } else {
        output = &std::cout;
    }

    *output << data;
    return 0;
}
int CompressionAlgorithm::loadFromFile(const std::string& filename, std::string& data){
    std::istream* input;
    std::ifstream inputFile;
    if (!filename.empty()) {
        inputFile.open(filename);
        if (!inputFile) {
            std::cerr << "Error opening input file: " << filename << std::endl;
            return 1;
        }
        input = &inputFile;
    } else {
        input = &std::cin;
    }

    // Read the entire input file or stdin into a string
    std::string inputText((std::istreambuf_iterator<char>(*input)), std::istreambuf_iterator<char>());
    data = std::move(inputText);

    return 0;
}
