#include <cxxopts.hpp>
#include <fstream>
#include <iostream>
#include <istream>
#include <string>

int main(int argc, char* argv[]) {
    // Command line options
    cxxopts::Options options("compression", "Simple Compression Utility");

    options.add_options()
        ("h,help", "Show help")
        ("a,algorithm", "Compression algorithm", cxxopts::value<std::string>()->default_value("huffman"))
        ("e,encode", "Encode input to output")
        ("d,decode", "Decode")
        ("i,input", "Input file (Can be stdin if left empty)", cxxopts::value<std::string>())
        ("o,output", "Output file (Can be stdout if left empty)", cxxopts::value<std::string>());

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    if (!(result.count("encode") ^ result.count("decode"))) {
        std::cerr << "You must choose one between encode and decode." << std::endl;
        std::cout << options.help() << std::endl;
        return 1;
    }

    std::string algorithmName = result["algorithm"].as<std::string>();

    if (algorithmName != "huffman" && algorithmName != "LZW") {
        std::cerr << "Invalid algorithm. Use -h or --help for help." << std::endl;
        return 1;
    }

    bool decode = result.count("decode") > 0;
    std::string inputFileName;
    std::string outputFileName;

    if (result.count("input")) {
        inputFileName = result["input"].as<std::string>();
    }

    if (result.count("output")) {
        outputFileName = result["output"].as<std::string>();
    }

    // Read input text from file or stdin
    std::istream* input;
    std::ifstream inputFile;
    if (!inputFileName.empty()) {
        inputFile.open(inputFileName);
        if (!inputFile) {
            std::cerr << "Error opening input file: " << inputFileName << std::endl;
            return 1;
        }
        input = &inputFile;
    } else {
        input = &std::cin;
    }

    // Read the entire input file or stdin into a string
    std::string inputText((std::istreambuf_iterator<char>(*input)), std::istreambuf_iterator<char>());


    std::ostream* output;
    std::ofstream outputFile;
    if (!outputFileName.empty()) {
        outputFile.open(outputFileName);
        if (!outputFile) {
            std::cerr << "Error opening output file: " << outputFileName << std::endl;
            // deleteAlgorithm(compressionAlgorithm);
            return 1;
        }
        output = &outputFile;
    } else {
        output = &std::cout;
    }

    *output << " hello ( " << inputText << ")\n";

    // Rest of your code...
}