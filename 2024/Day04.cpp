
// https://adventofcode.com/2024/day/4

#include <string>
#include <regex>
#include <iostream>
#include <sstream>
#include <format>
#include <print>

#include <ranges>

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

constexpr std::string_view XMAS = "XMAS";

std::vector<int> getSkipSizes(int lineLength) {
    return std::vector{
        1, -1, 
        -lineLength - 1, -lineLength, -lineLength + 1,
         lineLength - 1,  lineLength,  lineLength + 1
    };
}
auto getSkipSizes(std::string_view str) {
    const int lineLength = str.find_first_of('\n') + 1;
    return getSkipSizes(lineLength);
} 

bool hasMatch(
    std::string_view str,
    int root,
    int skip
) {
    for(int j = 1; j < XMAS.length(); j++) {
        const auto idx = root + j * skip;
        if(idx < 0 || idx > str.length()) return false;
        if(XMAS[j] != str[idx]) return false;
    }
    return true;
}

std::string substring(
    std::string_view str,
    int root,
    int skip,
    bool match
) {
    std::string out = "";
    for(int j = 0; j < XMAS.length(); j++) {
        const auto idx = root + j * skip;
        if(idx < 0 || idx > str.length()) out += '_';
        else if(str[idx] == '\n') out += 'n';
        else out += str[idx];
    }

    if(skip < 0) std::reverse(out.begin(), out.end());

    return std::format("{} [root:{:3}, skip: {:-2}, match: {}]", out, root, skip, match);
}

template<typename OnMatch>
    requires std::invocable<OnMatch, std::size_t /*root*/, int /* skip */>
void getMatches(
    std::string_view str,
    OnMatch&& onMatch
) {
    const auto skipSizes = getSkipSizes(str);
    std::size_t count = 0;
    for(int i = 0; i < str.length(); i++) {
        if(str[i] != XMAS[0]) continue; // First character always wrong
        for(auto skip : skipSizes)
            if(hasMatch(str, i, skip)) onMatch(i, skip);
    }
}

std::size_t matches(std::string_view str) {
    std::size_t count = 0;
    getMatches(str, [&count](auto,auto) { count++; });
    return count;
}

std::string verification(std::string_view str) {
    std::string out(str);
    getMatches(str, [&out](auto root, auto skip) {
        for(int j = 0; j < XMAS.length(); j++) {
            const auto idx = root + j * skip;
            out[idx] = '.';
    }
    });
    return out;
}

std::string invertHeatmap(
    std::string_view original,
    std::string_view heatmap
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
            columns = input.size();
        elements += input + '\n';
        rows++;
    };

    auto matchCount = matches(elements);
    std::cout << "Number of XMAS entries: " << matchCount << '\n';

    return 0;
}
