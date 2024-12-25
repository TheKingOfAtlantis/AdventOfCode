
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
constexpr std::string_view MAS  = "MAS";

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

bool hasXMASMatch(
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

bool hasCrossMASMatch(
    std::string_view str,
    int root,
    std::size_t lineWidth
) {
    return false;
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
void getXMASMatches(
    std::string_view str,
    OnMatch&& onMatch
) {
    const auto skipSizes = getSkipSizes(str);
    std::size_t count = 0;
    for(int i = 0; i < str.length(); i++) {
        if(str[i] != XMAS[0]) continue; // First character always wrong
        for(auto skip : skipSizes)
            if(hasXMASMatch(str, i, skip)) onMatch(i, skip);
    }
}

template<typename OnMatch>
    requires std::invocable<OnMatch, std::size_t /*root*/, int /* skip */>
void getCrossMASMatches(
    std::string_view str,
    OnMatch&& onMatch
) {
    std::size_t count = 0;
    for(int i = 0; i < str.length(); i++) {
        if(hasCrossMASMatch(str, i, skip)) onMatch(i, 0);
    }
}

struct OnMatchCounter {
    std::size_t& count;
    constexpr void operator()(auto, auto) noexcept { count++; };
};

template<typename Matcher>
std::size_t matches(std::string_view str, Matcher&& matcher) {
    std::size_t count = 0;
    matcher(str, OnMatchCounter{count});
    return count;
}

struct XMASHeatmapMarker {
    std::string& heatmap;
    void operator()(std::size_t root, int skip) {
        for(int j = 0; j < XMAS.length(); j++) {
            const auto idx = root + j * skip;
            heatmap[idx] = '.';
        }
    };
};

struct CrossMASHeatmapMarker {
    std::string& heatmap;
    void operator()(std::size_t root, int lineLength) {
        std::vector points{0, lineLength - 1, lineLength + 1, -lineLength - 1, -lineLength + 1};
        for(auto offset : points)
             heatmap[root + offset] = '.';
    };
};

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

template<typename HeatmapMarker, typename Matcher>
std::string verification(std::string_view str, Matcher matcher) {
    std::string out(str);
    matcher(str, HeatmapMarker{out});
    return invertHeatmap(str, out);
}

int main() {
    std::string elements = "";
    for(std::string input; std::getline(std::cin, input) && !input.empty();)
        elements += input + '\n';

    std::cout << "Number of XMAS entries: "  << matches(elements, getXMASMatches<OnMatchCounter>)     << '\n'
              << "Number of X-MAS entries: " << matches(elements, getCrossMASMatches<OnMatchCounter>) << '\n';

    return 0;
}
