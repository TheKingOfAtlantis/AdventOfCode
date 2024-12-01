
// https://adventofcode.com/2024/day/1

#include <string>
#include <iostream>

#include <vector>
#include <ranges>
#include <algorithm>
#include <numeric>

int main() {
    std::vector<int> lhs, rhs;

    std::string input;
    while(std::getline(std::cin, input)) {
        auto delimiter = input.find(' ');
        lhs.push_back(std::stoi(input.substr(0, delimiter)));
        rhs.push_back(std::stoi(input.substr(delimiter + 1)));
    }

    std::ranges::sort(lhs);
    std::ranges::sort(rhs);

    auto result = std::ranges::fold_left_first(
        std::views::zip(lhs, rhs) | std::views::transform([](auto values) { return std::abs(std::get<0>(values) - std::get<1>(values)); }),
        std::plus<int>{}
    );

    return 0;
}
