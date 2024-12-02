
// https://adventofcode.com/2024/day/1

#include <string>
#include <iostream>
#include <sstream>

#include <ranges>
#include <algorithm>
#include <numeric>
#include <vector>
#include <unordered_map>

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
        
        bool increasing = levels[1] > levels[0];
        bool safe = true;
        for(int i = 1; i < levels.size(); i++) {
            // Increase but was marked decreasing
            if(levels[i] > levels[i - 1] && !increasing) {
                safe = false;
                break;
            }
            // Decrease but was marked Increasing
            if(levels[i] < levels[i - 1] && increasing) {
                safe = false;
                break;
            }
            // More than 3
            auto change = abs(levels[i] - levels[i - 1]);
            if(change > 3 || change < 1) {
                safe = false;
                break;
            }
        }

        if(safe) safetyScore++;
    }

    std::cout << "Safety score: " << safetyScore << std::endl;

    return 0;
}
