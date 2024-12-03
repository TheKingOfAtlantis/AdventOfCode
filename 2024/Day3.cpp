
// https://adventofcode.com/2024/day/3

#include <string>
#include <iostream>
#include <sstream>

#include <ranges>
#include <algorithm>
#include <numeric>
#include <span>
#include <vector>
#include <unordered_map>

/**
 * @brief Finds the next valid mul instruction
 * 
 * @return Pair of values
 * @retval first - Start of mul instruction
 * @retval second - Length of mul instruction
 */
std::pair<std::size_t, std::size_t> findNextMul(std::string_view str) {
    std::size_t next = 0;
    while(true) {
        next = str.find("mul(", next + 1);
        auto idx = next + 4;
        
        for(auto maxDigits = 3; std::isdigit(str[idx]) && maxDigits--; idx++);
        if(str[idx++] != ',') continue;
        for(auto maxDigits = 3; std::isdigit(str[idx]) && maxDigits--; idx++);
        if(str[idx] != ')') continue;

        return std::make_pair(next, idx - next + 1);
    };
}

int main() {
    constexpr std::string_view test = "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))";

    auto [start, len] = findNextMul(test);
    auto inst = test.substr(start, len);

    return 0;
}
