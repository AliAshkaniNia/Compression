#include "unixFileHandler.h"
#include <fstream>
#include <iostream>

void FileHandlers::UnixFileHandler::init(std::string_view input_file_path, std::string_view output_file_path){
    this->input_file_path = input_file_path;
    this->output_file_path = output_file_path;
}

int FileHandlers::UnixFileHandler::load(std::string& content) {
    std::istream* input;
    std::ifstream inputFile;
    if (!input_file_path.empty()) {
        inputFile.open(input_file_path);
        if (!inputFile) {
            std::cerr << "Error opening input file: " << input_file_path << std::endl;
            return 1;
        }
        input = &inputFile;
    } else {
        input = &std::cin;
    }

    // Read the entire input file or stdin into a string
    std::string inputText((std::istreambuf_iterator<char>(*input)), std::istreambuf_iterator<char>());
    content = std::move(inputText);
    
    if (inputFile.is_open()) {
        inputFile.close();
    }
    return 0;
}

int FileHandlers::UnixFileHandler::save(std::string_view content){
    std::ostream* output;
    std::ofstream outputFile;
    if (!output_file_path.empty()) {
        outputFile.open(output_file_path);
        if (!outputFile) {
            std::cerr << "Error opening output file: " << output_file_path << std::endl;
            return 1;
        }
        output = &outputFile;
    } else {
        output = &std::cout;
    }

    *output << content;
    if (outputFile.is_open()) {
        outputFile.close();
    }

    return 0;
}

