
// https://adventofcode.com/2024/day/4

#include <string>
#include <regex>
#include <iostream>
#include <sstream>
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

std::string verification(const std::string& str, const std::regex& regex, bool hasGroups = false) {
    std::string out;
    std::regex_replace(std::back_inserter(out), str.begin(), str.end(), regex, hasGroups ? ".$1.$2.$3." : "....");
    return out;
}

std::string combineVerificationHeatmap(
    const std::string& lhs, 
    const std::string& rhs
) {
    std::string out;
    for(int i = 0; i < lhs.length(); i++) {
        if(lhs[i] == '.' || rhs[i] == '.')
            out += '.';
        else out += lhs[i];
    }
    return out;
}

std::string invertHeatmap(
    const std::string& original,
    const std::string& heatmap
) {
    std::string out;
    for(int i = 0; i < original.length(); i++)
        if(heatmap[i] == '.' || original[i] == '\n') out += original[i];
        else out += '.';
    return out;
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

    std::cout << "'XMAS' matches:\n" << invertHeatmap(elements, verification(elements, std::regex("XMAS"))) << "\n\n"
              << "'SAMX' matches:\n" << invertHeatmap(elements, verification(elements, std::regex("SAMX"))) << "\n\n";

    std::size_t counts = 0;
    std::string heatmap = elements;

    auto find = [&](const std::string& pattern) {
        auto map = verification(elements, std::regex(pattern), true);
        heatmap = combineVerificationHeatmap(heatmap, map);

        auto match = matches(elements, std::regex(pattern));
        counts += match;

        std::cout << std::format("'{}[count: {}]':\n{}\n\n", pattern, match, invertHeatmap(elements, map));
    };
    
    find("XMAS");
    find("SAMX");

    for(int i = -1; i <= 1; i++) find(std::format(R"(X([\w\n]{{{0}}})M([\w\n]{{{0}}})A([\w\n]{{{0}}})S)", columns + 1 + i));
    for(int i = -1; i <= 1; i++) find(std::format(R"(S([\w\n]{{{0}}})A([\w\n]{{{0}}})M([\w\n]{{{0}}})X)", columns + 1 + i));

    std::cout << "Combined:\n" << invertHeatmap(elements, heatmap);

    std::cout << "Number of XMAS entries: " << counts;

    return 0;
}
