

// https://adventofcode.com/2024/day/5

#include <string>
#include <regex>
#include <iostream>
#include <sstream>
#include <format>
#include <print>

std::stringstream example(
    ""
);

void parseStream(std::istream& stream, std::invocable<std::string> auto&& lineReader) {
    for(std::string line; !stream.eof(); std::getline(stream, line))
        lineReader(line);
}

int main(int argc, char* argv[]) {

    std::string input;
    parseStream(std::cin, [&input](const std::string& line) {
        input += line + '\n';
    });

    return 0;
}

