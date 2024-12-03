
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
        next = str.find("mul(", next);
        if(next == std::string_view::npos) return {next, 0};

        auto idx = next + 4;
        
        for(auto maxDigits = 3; std::isdigit(str[idx]) && maxDigits--; idx++);
        if(str[idx++] != ',') {
            next = idx;
            continue;
        }
        for(auto maxDigits = 3; std::isdigit(str[idx]) && maxDigits--; idx++);
        if(str[idx] != ')') {
            next = idx;
            continue;
        }
        return {next, idx - next + 1};
    };
}

auto parseMul(std::string_view str) {
    constexpr std::string_view mul = "mul(";
    str.remove_prefix(mul.size());
    str.remove_suffix(1);

    auto delimiter = str.find(",");
    auto lhs = str.substr(0, delimiter);
    auto rhs = str.substr(delimiter + 1);

    return std::stoi(std::string(lhs)) * std::stoi(std::string(rhs));
}

auto findDont(std::string_view str) {
    return str.find("don't()");
}
auto findDo(std::string_view str) {
    return str.find("do()");
}

std::pair<std::size_t, std::size_t> findNextDoRegion(std::string_view str) {
    constexpr std::string_view _do = "do()";

    auto start = findDo(str) + _do.size();
    auto end   = findDont(str);

    if(start == std::string_view::npos) return {start, 0};
    if(end == std::string_view::npos) return {start, std::string_view::npos};

    return std::make_pair(start, end - start);
}

int main() {

    std::string input, instructions;
    while(std::getline(std::cin, input)) {
        instructions += input;
    }

    instructions = "do()" + instructions;

    std::string valid = "";
    while(!instructions.empty()) {
        auto [start, len] = findNextDoRegion(instructions);
        if(len == 0) break;
        valid += instructions.substr(start, len);
        if(len == std::string_view::npos) break;
        instructions = instructions.substr(start + len + sizeof("don't()") - 1);
    }

    instructions = valid;

    std::size_t prodSum = 0;
    while(!instructions.empty()) {
        auto [start, len] = findNextMul(instructions);
        if(start == std::string_view::npos) break;

        auto inst = instructions.substr(start, len);
        prodSum += parseMul(inst);
        instructions = instructions.substr(start + len);
    }

    std::cout << "Cumulative Sum of mul(...): " << prodSum << std::endl;

    return 0;
}
