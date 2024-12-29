

// https://adventofcode.com/2024/day/5

#include <string>
#include <format>
#include <regex>

#include <iostream>
#include <sstream>
#include <print>

#include <algorithm>
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

    constexpr bool mustPreceed(int otherPage) const noexcept {
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

bool isOrdered(int lhs, int rhs, const std::set<PageRule>& rules) {
    auto it = rules.find(rhs);
    if(it != rules.cend())
        if(it->mustPreceed(lhs)) return false;
    return true;
}

bool isOrdered(std::span<int> pages, const std::set<PageRule>& rules) {
    for(int i = 0; i < pages.size(); i++)
        for(int j = 0; j < i; j++) {
            if(!isOrdered(pages[j], pages[i], rules)) 
                return false;
        }
    return true;
}

auto& getMiddle(std::span<int> pages) {
    return pages[pages.size()/2];
}

int main(int argc, char* argv[]) {
    std::vector<PageOrdering> pageOrdering;
    std::vector<std::vector<int>> updateSet;

    ParserState state = ParserState::PageOrdering;
    std::string input;
    parseStream(std::cin, [&](const std::string& line) {
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

    auto rules = determineRules(pageOrdering);

    auto part1Result = std::ranges::fold_left_first(
        updateSet 
            | std::views::filter([&rules](std::span<int> pgs) { return isOrdered(pgs, rules); })
            | std::views::transform([](std::span<int> pgs) { return getMiddle(pgs); }),
        std::plus<>{}
    );

    auto part2Result = std::ranges::fold_left_first(updateSet
        | std::views::filter([&rules](std::span<int> pgs) { return !isOrdered(pgs, rules); })
        | std::views::transform([&rules](std::vector<int>& pages) {
            std::ranges::sort(pages, [&rules](auto lhs, auto rhs) { return isOrdered(lhs, rhs, rules); } );
            return getMiddle(pages);
        }),
        std::plus<>{}
    );

    if(part1Result) std::cout << "Part 1: " << *part1Result;
    else std::cout << "Failed to get part 1 result";
    std::cout << '\n';
    if(part2Result) std::cout << "Part 2: " << *part2Result;
    else std::cout << "Failed to get part 2 result";

    return 0;
}

