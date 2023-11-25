#include "algorithms/LZWCompression.h"

#include <map>
#include <vector>
#include <iostream>
#include <numeric>

LZWCompression::LZWCompression(std::unique_ptr<Converters::IStringEncoder<uint32_t>> _converter)
    :m_converter(std::move(_converter)){

}

void LZWCompression::encode(const std::string& input, std::string& output) {
    // Check if an input string is empty
    if (input.empty()) {
        output.clear();
        return;
    }

    // Initialize dictionary with ASCII characters
    std::map<std::string, int> dictionary;
    int dictionarySize = 256;
    for (int i = 0; i < dictionarySize; ++i) {
        dictionary[std::string(1, i)] = i;
    }

    std::vector<int> encodedValues;
    std::string currentString;

    // Iterate over each character in the input string
    for (const auto& character : input) {
        std::string newString = currentString + character;
        if (dictionary.count(newString)) {
            currentString = newString;
        } else {
            encodedValues.emplace_back(dictionary[currentString]);
            dictionary[newString] = dictionarySize++;
            currentString = character;
        }
    }

    // If there is a leftover string, encode it
    if (!currentString.empty()) {
        encodedValues.emplace_back(dictionary[currentString]);
    }

    // Convert the encoded values into a string
    output = std::accumulate(encodedValues.begin(), encodedValues.end(), std::string{},
    [this](const std::string& a, int b) {
        return a + this->m_converter->encode(b);
    });
}

void LZWCompression::decode(const std::string& input, std::string& output) {
    output.clear();

    // Check if an input string is empty
    if (input.empty()) {
        return;
    }

    // Initialize inverse dictionary with ASCII characters
    std::map<int, std::string> inverseDictionary;
    int dictionarySize = 256;
    for (int i = 0; i < dictionarySize; ++i) {
        inverseDictionary[i] = std::string(1, i);
    }

    std::vector<int> decodedValues;

    // Decode every 4-byte chunk of input
    /// TODO: do not use fixed 4 bytes  

    for (size_t i = 0; i < input.size(); i += 4) {
        decodedValues.push_back(m_converter->decode(input.substr(i, 4)));
    }

    std::string currentString = inverseDictionary[decodedValues[0]];
    std::string decodedString = currentString;

    // Iterate over the decoded values
    for (size_t i = 1; i < decodedValues.size(); ++i) {
        int key = decodedValues[i];
        std::string entry;

        if (inverseDictionary.count(key)) {
            entry = inverseDictionary[key];
        } else if (key == dictionarySize) {
            entry = currentString + currentString[0];
        } else {
            std::cerr << "Error in decoding: unexpected key '" << key << "' at index " << i << ".\n";
            return;
        }

        decodedString += entry;
        inverseDictionary[dictionarySize++] = currentString + entry[0];
        currentString = entry;
    }

    output = decodedString;
}
