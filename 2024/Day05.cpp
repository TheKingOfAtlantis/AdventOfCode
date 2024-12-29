

// https://adventofcode.com/2024/day/5

#include <string>
#include <regex>
#include <iostream>
#include <sstream>
#include <format>
#include <print>

std::stringstream example(
R"(47|53
97|13
97|61
97|47
75|29
61|13
75|53
29|13
97|29
53|29
61|53
97|53
61|29
47|13
75|47
97|75
47|61
75|61
47|29
75|13
53|13

75,47,61,53,29
97,61,53,29,13
75,29,13
75,97,47,61,53
61,13,29
97,13,75,29,47)"
);

void parseStream(std::istream& stream, std::invocable<std::string> auto&& lineReader) {
    for(std::string line; std::getline(stream, line) && !stream.eof();)
        lineReader(line);
}

enum class ParserState {
    PageOrdering,
    OutputSelection
};

int main(int argc, char* argv[]) {
    using PageOrdering = std::pair<int, int>;
    std::vector<PageOrdering> pageOrdering;

    ParserState state = ParserState::PageOrdering;
    std::string input;
    parseStream(std::cin, [&input](const std::string& line) {
        input += line + '\n';
        switch(state) {
        case ParserState::PageOrdering:
            if(line == "") state = ParserState::OutputSelection;
            int lhs, rhs;
            std::sscanf(line.c_str(), "%d|%d", &lhs, &rhs);
            pageOrdering.emplace_back(lhs, rhs);
            break;
        case ParserState::OutputSelection:
            break;
        }
    });


    return 0;
}

