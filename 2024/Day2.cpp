
// https://adventofcode.com/2024/day/1

#include <string>
#include <iostream>
#include <sstream>

#include <ranges>
#include <algorithm>
#include <numeric>
#include <span>
#include <vector>
#include <unordered_map>

bool isSafe(int lhs, int rhs, bool increasing) {
    // Increase but was marked decreasing
    if(lhs > rhs && !increasing)
        return false;
    // Decrease but was marked Increasing
    if(lhs < rhs && increasing)
        return false;
    // More than 3
    auto change = abs(lhs - rhs);
    if(change > 3 || change < 1)
        return false;

    return true;
}

int findNotSafe(std::span<int> levels) {
    bool increasing = levels[1] > levels[0];
    for(int i = 1; i < levels.size(); i++) {
        if(!isSafe(levels[i], levels[i - 1], increasing))
            return i;
    }
    return -1;
}

int main() {
    int safetyScore = 0;
    for(std::string input; std::getline(std::cin, input);) {
        std::stringstream ss(input);    
        std::vector<int> levels;
        for(int i = 0; i < std::ranges::count(input, ' ') + 1; i++) {
            int current = 0;
            ss >> current;
            levels.push_back(current);
        }
        

        auto safetyTrial = [](auto levels, int notSafe) {
            levels.erase(levels.begin() + notSafe);
            return findNotSafe(levels);
        };

        int notSafe = findNotSafe(levels);
        for(int i = 0; i < levels.size(); i++) {
            if(safetyTrial(levels, i) == -1) {
                safetyScore++;
                break;
            }
        }
    }

    std::cout << "Safety score: " << safetyScore << std::endl;

    return 0;
}
