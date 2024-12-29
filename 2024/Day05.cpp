

// https://adventofcode.com/2024/day/5

#include <string>
#include <regex>
#include <iostream>
#include <sstream>
#include <format>
#include <print>
#include <ranges>

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
    for(std::string line; std::getline(stream, line);)
        lineReader(line);
}

enum class ParserState {
    PageOrdering,
    OutputSelection
};

int main(int argc, char* argv[]) {
    using PageOrdering = std::pair<int, int>;
    std::vector<PageOrdering> pageOrdering;
    std::vector<std::vector<int>> updateSet;

    ParserState state = ParserState::PageOrdering;
    std::string input;
    parseStream(example, [&](const std::string& line) {
        input += line + '\n';
        switch(state) {
        case ParserState::PageOrdering:
            if(line == "") state = ParserState::OutputSelection;
            int lhs, rhs;
            std::sscanf(line.c_str(), "%d|%d", &lhs, &rhs);
            pageOrdering.emplace_back(lhs, rhs);
            break;
        case ParserState::OutputSelection:
            std::vector<int> pages;
            auto addPage = [&pages](auto start, auto end) {
                std::string val(start, end);
                pages.push_back(std::stoi(val));
            };
            
            auto start = line.begin();
            for(auto end = line.begin(); end != line.cend(); end++)
                if(*end == ',') {
                    addPage(start, end);
                    start = end + 1;
                }
            addPage(start, line.cend());
            updateSet.push_back(pages);
            break;
        }
    });

    for(auto [lhs, rhs] : pageOrdering)
        std::println("{}|{}", lhs, rhs);

    for(auto [i, update] : updateSet | std::views::enumerate) {
        std::string output = "";
        for(int j = 0; j < update.size() - 1; j++)
            output += std::format("{},", update[j]);
        output += std::format("{}\n", update.back());
        std::print("Set {}: {}", i, output);
    }

    return 0;
}

