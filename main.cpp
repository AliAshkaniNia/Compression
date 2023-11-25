#include <cxxopts.hpp>
#include <iostream>
#include <string>
#include "algorithms/LZWCompression.h"
#include "algorithms/huffmanCompression.h"
#include "utility/integerToStringEncoder.h"
#include "utility/unixFileHandler.h"

struct CompressionArgs
{
    bool is_encode;
    bool human_readable_output;
    std::string algorithmName;
    std::string inputFileName;
    std::string outputFileName;
};

int parseArguments(int argc, char *argv[], CompressionArgs &args)
{
    // Command line options
    cxxopts::Options options("compression", "Simple Compression Utility");

    options.add_options()("h,help", "Show help")("a,algorithm", "Compression algorithm (can be huffman or LZW)", cxxopts::value<std::string>()->default_value("huffman"))("r,human-readable", "Human readable output")("e,encode", "Encode")("d,decode", "Decode")("i,input", "Input file (Will be stdin if left empty)", cxxopts::value<std::string>())("o,output", "Output file (Will be stdout if left empty)", cxxopts::value<std::string>());

    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        return 1;
    }

    if (!(result.count("encode") ^ result.count("decode")))
    {
        std::cerr << "You must choose one between encode and decode." << std::endl;
        std::cout << options.help() << std::endl;
        return 1;
    }

    args.human_readable_output = result.count("human-readable") > 0;
    args.is_encode = result.count("encode") > 0;

    args.algorithmName = result["algorithm"].as<std::string>();

    if (args.algorithmName != "huffman" && args.algorithmName != "LZW")
    {
        std::cerr << "Invalid algorithm. Use -h or --help for help." << std::endl;
        return 1;
    }

    args.inputFileName = "";
    args.outputFileName = "";

    if (result.count("input"))
    {
        args.inputFileName = result["input"].as<std::string>();
    }

    if (result.count("output"))
    {
        args.outputFileName = result["output"].as<std::string>();
    }

    return 0;
}

int runEngine(const CompressionArgs &args)
{
    std::string inputContent, outputContent;

    std::unique_ptr<CompressionAlgorithm> compression;

    auto converter =
        std::make_unique<Converters::IntegerToStringEncoder<uint32_t>>(args.human_readable_output);

    auto fileHandler = std::make_unique<FileHandlers::UnixFileHandler>();
    fileHandler->init(args.inputFileName, args.outputFileName);

    if (args.algorithmName == "huffman")
    {
        compression = std::make_unique<HuffmanCompression>(std::move(converter));
    }
    else if (args.algorithmName == "LZW")
    {
        compression = std::make_unique<LZWCompression>(std::move(converter));
    }else{
        std::cerr << "Invalid algorithm. Use -h or --help for help." << std::endl;
        return 1;
    }

    if (fileHandler->load(inputContent) != 0)
        return 1;

    if (args.is_encode)
    {
        compression->encode(inputContent, outputContent);
    }
    else
    {
        compression->decode(inputContent, outputContent);
    }

    if (fileHandler->save(outputContent) != 0)
        return 1;

    return 0;
}

int main(int argc, char *argv[])
{
    CompressionArgs args;

    if (parseArguments(argc, argv, args) != 0)
        return 1;

    return runEngine(args);
}