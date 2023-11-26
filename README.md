# File Compression Utility 

This utility is a versatile file compression tool, equipped with support for Huffman and LZW algorithms.


## Building the Project 
To build the project, clone the repository and execute the following commands in your terminal:
```
mkdir build 
cd build 
cmake ..
cmake --build . -j${nproc}
```
he following options can be enabled:

- `USE_ASAN` : Utilize address sanitizers if available
- `USE_UBSAN` : Employ undefined behavior sanitizer if available
- `USE_CLANG_TIDY` : Utilize clang-tidy if available
- `USE_CPPCHECK` : Employ cppcheck if available
- `BUILD_TESTS` : Build Tests

To use these options, execute:

```bash
cmake -DBUILD_TESTS=ON -USE_UBSAN=OFF ..
cmake --build . -j$(nproc)
```

## Executing Unit Tests

To run the unit tests, you first need to enable the `BUILD_TESTS` switch as mentioned above. After the project build is complete, the test executables will be found in the `build/tests/` directory. For instance, to run the serializer test, use the following command:
```bash
cd build 
./tests/tests_serializer
```

## Usage 
Here are some examples of how to use the utility
```bash
$ cd build 
$ ./compression -h # see help
$ ./compression --encode --algorithm huffman --input file1.txt --output file2.txt # encodes using Huffman Algorithm
$ ./compression --decode --algorithm huffman --input file2.txt --output file1.txt # decodes using Huffman Algorithm

# you can also use short formed options
$ ./compression -e -a LZW  -i file1.txt -o file2.txt # encodes using LZW Algorithm

# A mixed form is also valid 
$ ./compression -d --human-readable  -a huffman --input file2.txt --output file1.txt # decodes using Huffman Algorithm and produces human readable output

$ cat file.txt | ./compression -e  # it also works with pipes (in this command it used stdin and stdout)

```
## Understanding Serialization
In the realm of computing, the process of serialization is akin to transforming data into a format that can be easily stored or transmitted. The integerToStringSerializer class provides such functionality, specifically for integral (whole number) values, converting them into string representations.

Consider a situation where you are given an integer, 305419896. This number might be difficult to manage in certain contexts, especially when you need to store or transmit it. This is where serialization comes into play. The integerToStringSerializer takes this integer and transforms it into a more manageable string format.

The conversion can happen in two ways:

- *Human-readable*: In this mode, the integer is converted into a string that is easy for a human to understand. In this case, our example integer 305419896 (which corresponds to 0x12345678 in hexadecimal format) would be transformed into the string 12345678. This string is a direct, human-readable representation of the hexadecimal value of the integer.

- *Non-human-readable*: In this mode, the conversion is focused more on compactness rather than readability. The resulting string may not be easily understandable by a human but is very useful for computer processing or for situations where storage space is at a premium. In this case, our example integer 305419896 would be transformed into the string x4Vh.

So, in simpler terms, the integerToStringSerializer class is a tool that takes an integer and converts it into a string format. This can be a direct, readable format (12345678) or a more compact, non-readable format (x4Vh). The choice between the two depends on whether readability or storage efficiency is more important in your specific use case.
## Compression Methods
### Huffman algorithm 
For those unfamiliar with Huffman coding, [this video](https://www.youtube.com/watch?v=JsTptu56GM8) serves as an excellent resource.

The serialization process is straightforward and human-readable, and follows this format:

   -  4 or 8 initial bytes: tree length (depending on whether the human-readable option (-r or --human-readable) is specified)
   - Encoded tree: trees are encoded by creating a pair of their character and their code, followed by a `space` for separation.
   - Encoded data: a sequence of 1's and 0's containing the encoded data.

Here is a sample valid file:
```bash
$ echo -en "GOOD" | ./compression -er
0000000b
O11 D10 G0 
0111110
```

### LZW 

If you are not familiar with LZW coding, [this video](https://www.youtube.com/watch?v=1KzUikIae6k) provides a thorough explanation.

LZW serialization is quite straightforward, consisting of a sequence of 4 or 8 bytes (this again depends on whether the human-readable option is specified).




## Requirements

A C++17 compiler is required, which should be widely supported. The only C++17 feature used here is `std::string_view`. If a modern compiler is not available, replacing all `std::string_view` with `const string&` should allow the use of a C++11 compiler.

## Dependencies 

This program is self-contained and doesn't require any third-party libraries for compilation. It uses [cxxopts](https://github.com/jarro2783/cxxopts) for parsing command-line arguments and [GoogleTest](https://github.com/google/googletest) for unit testing. cxxopts is included in the project, and GoogleTest is automatically downloaded by CMake.

