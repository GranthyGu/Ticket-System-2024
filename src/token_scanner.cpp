// Created by GranthyGu on 2025/5/8

#include "token_scanner.hpp"

token_scanner::token_scanner(const std::string& input) {
    size_t idx = 0;
    if (idx >= input.size() || input[idx] != '[') throw std::invalid_argument("Invalid input: missing '['");
    idx++;

    std::string time_str;
    while (idx < input.size() && input[idx] != ']') {
        if (!std::isdigit(input[idx])) throw std::invalid_argument("Invalid time format");
        time_str += input[idx++];
    }
    if (idx >= input.size() || input[idx] != ']') throw std::invalid_argument("Invalid input: missing ']'");
    time = std::stoi(time_str);
    idx++;

    // 跳过可能的空格
    while (idx < input.size() && std::isspace(input[idx])) idx++;

    // 解析操作名
    while (idx < input.size() && !std::isspace(input[idx])) {
        operation += input[idx++];
    }

    // 跳过空格
    while (idx < input.size() && std::isspace(input[idx])) idx++;

    // 解析参数 -x value
    while (idx < input.size()) {
        if (input[idx] != '-') throw std::invalid_argument("Invalid input: expected '-'");
        idx++;

        if (idx >= input.size() || !std::isalpha(input[idx])) throw std::invalid_argument("Expected key after '-'");
        char key = input[idx++];
        
        while (idx < input.size() && std::isspace(input[idx])) idx++;
        std::string value;
        while (idx < input.size() && !std::isspace(input[idx])) {
            value += input[idx++];
        }
        key_argument.push_back({key, value});
        while (idx < input.size() && std::isspace(input[idx])) idx++;
    }
}