
// https://adventofcode.com/2024/day/4

#include <string>
#include <regex>
#include <iostream>
#include <format>
#include <print>

std::stringstream puzzle(
    "MMMSXXMASM\n"
    "MSAMXMSMSA\n"
    "AMXSXMAAMM\n"
    "MSAMASMSMX\n"
    "XMASAMXAMM\n"
    "XXAMMXXAMA\n"
    "SMSMSASXSS\n"
    "SAXAMASAAA\n"
    "MAMMMXMMMM\n"
    "MXMXAXMASX"
);

std::size_t matches(const std::string& str, const std::regex& regex) {
    std::sregex_iterator begin(str.begin(), str.end(), regex);
    std::sregex_iterator end{};
    
    std::size_t count = 0;
    for(auto it = begin; it != end; it++) count++;
    return count;
}

int main() {
    std::size_t rows = 0, columns = 0;
    std::string elements = "";

    std::string input;
    while(std::getline(puzzle, input) && !input.empty()) {
        if(rows == 0)
            columns = input.size() - 1;
        elements += input + '\n';
        rows++;
    };


    std::size_t counts = 0;

    counts += matches(elements, std::regex("XMAS"));
    counts += matches(elements, std::regex("SAMX"));

    for(int i = -1; i <= 1; i++) counts += matches(elements, std::regex(std::format(R"(X([\w\n]{{{0}}})M([\w\n]{{{0}}})A([\w\n]{{{0}}})S)", columns + 1 + i)));
    for(int i = -1; i <= 1; i++) counts += matches(elements, std::regex(std::format(R"(S([\w\n]{{{0}}})A([\w\n]{{{0}}})M([\w\n]{{{0}}})X)", columns + 1 + i)));


    std::cout << "Number of XMAS entries: " << counts;

    return 0;
}
