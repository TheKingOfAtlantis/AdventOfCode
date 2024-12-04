
// https://adventofcode.com/2024/day/1

#include <string>
#include <iostream>

#include <ranges>
#include <algorithm>
#include <numeric>
#include <vector>
#include <unordered_map>

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

    if(result) std::cout << "Total Distance: "<< *result << std::endl;
    else std::cout << "Failed to get result" << std::endl;

    std::unordered_map<int, std::size_t> count;
    std::size_t similarityScore = 0;
    for(auto val : lhs) {
        if(!count.contains(val)) {
            std::size_t score = 0;
            for(int i = 0; i < rhs.size(); i++)
                if(rhs[i] == val)
                    score++;
            score *= val;
            count[val] = score;
            similarityScore += score;
        } else similarityScore += count[val];
    }
    
    std::cout << "Similarity Score: "<< similarityScore << std::endl;
    
    return 0;
}
