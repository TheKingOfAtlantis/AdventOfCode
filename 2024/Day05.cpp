

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

int main(int argc, char* argv[]) {

    std::string input;
    for(std::string line; std::getline(std::cin, line) && !line.empty();)
        input += line + '\n';

    return 0;
}

