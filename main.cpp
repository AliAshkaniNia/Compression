#include <cxxopts.hpp>
#include <iostream>
#include <string>
#include "compressionAlgorithm.h"
#include "LZWCompression.h"
#include "huffmanCompression.h"

int main(int argc, char* argv[]) {
    // Command line options
    cxxopts::Options options("compression", "Simple Compression Utility");

    options.add_options()
        ("h,help", "Show help")
        ("a,algorithm", "Compression algorithm (can be huffman or LZW)", cxxopts::value<std::string>()->default_value("huffman"))
        ("e,encode", "Encode input to output")
        ("d,decode", "Decode")
        ("i,input", "Input file (Will be stdin if left empty)", cxxopts::value<std::string>())
        ("o,output", "Output file (Will be stdout if left empty)", cxxopts::value<std::string>());

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

    std::unique_ptr<CompressionAlgorithm> compression;

    if (algorithmName == "huffman") {
        compression = std::make_unique<HuffmanCompression>();
    } else if (algorithmName == "LZW") {
        compression = std::make_unique<LZWCompression>();
    } else {
        std::cerr << "Invalid algorithm. Use -h or --help for help." << std::endl;
        return 1;
    }

    bool encode = result.count("encode") > 0;
    std::string inputFileName;
    std::string outputFileName;
    std::string inputText, outputText;

    if (result.count("input")) {
        inputFileName = result["input"].as<std::string>();
    }

    if (result.count("output")) {
        outputFileName = result["output"].as<std::string>();
    }

    if(compression->loadFromFile(inputFileName, inputText) != 0){
        return 1;
    }

    if(encode){
        compression->encode(inputText, outputText);
    }else{
        compression->decode(inputText, outputText);
    }

    if(compression->saveToFile(outputFileName, outputText) != 0){
        return 1;
    }

    return 0;
}