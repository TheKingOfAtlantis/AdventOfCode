
// https://adventofcode.com/2024/day/3

#include <string>
#include <regex>
#include <iostream>

int main() {
    std::string input, instructions;
    while(std::getline(std::cin, input) && !input.empty()) {
        instructions += input;
    }

    auto matcher = std::regex(R"(do\(\)|don't\(\)|mul\((\d{1,3}),(\d{1,3})\))");
    std::sregex_iterator begin(instructions.cbegin(), instructions.cend(), matcher);

    std::size_t prodSum = 0;
    bool enable = true;
    for(auto it = begin; it != std::sregex_iterator{}; it++) {
        auto match = *it;
        if(match.str() == "do()") enable = true;
        else if(match.str() == "don't()") enable = false;
        else if(enable) prodSum += std::stoi(match[1]) * std::stoi(match[2]);
    }

    std::cout << "Cumulative Sum of mul(...): " << prodSum << std::endl;

    return 0;
}
