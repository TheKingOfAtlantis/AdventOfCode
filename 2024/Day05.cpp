

// https://adventofcode.com/2024/day/5

#include <string>
#include <format>
#include <regex>

#include <iostream>
#include <sstream>
#include <print>

#include <ranges>
#include <vector>
#include <set>

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

using PageOrdering = std::pair<int, int>;

struct PageRule {
    int page;
    mutable std::set<int> followers;

    bool mustPreceed(int otherPage) noexcept {
        return followers.contains(otherPage);
    }
};

template<> struct std::less<PageRule> : std::less<int> {
    using is_transparent = std::true_type;

    constexpr bool operator()(const PageRule& lhs, const PageRule& rhs) const {
        return std::less<int>::operator()(lhs.page, rhs.page);
    };
    constexpr bool operator()(const PageRule& lhs, int rhs) const {
        return std::less<int>::operator()(lhs.page, rhs);
    }
    constexpr bool operator()(int lhs, const PageRule& rhs) const {
        return std::less<int>::operator()(lhs, rhs.page);
    };
};

std::set<PageRule> determineRules(std::span<PageOrdering> orderings) {
    std::set<PageRule> rules;

    for(auto ordering : orderings) {
        if(auto it = rules.find(ordering.first); it != rules.cend())
            it->followers.insert(ordering.second);
        else rules.insert({ordering.first, { ordering.second }});
    }

    return rules;
}

int main(int argc, char* argv[]) {
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

    auto rules = determineRules(pageOrdering)

    for(auto rule : rules) {
        std::cout << rule.page << ": ";
        for(auto page : rule.followers)
            std::cout << page << " ";
        std::cout << "\n";
    }


    return 0;
}

