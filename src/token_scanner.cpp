// Created by GranthyGu on 2025/5/8

#include "token_scanner.hpp"

token_scanner::token_scanner(const std::string& input) {
    size_t idx = 0;
    idx++;
    std::string time_str;
    while (idx < input.size() && input[idx] != ']') {
        time_str += input[idx++];
    }
    time = std::stoi(time_str);
    idx++;
    while (idx < input.size() && std::isspace(input[idx])) idx++;
    while (idx < input.size() && !std::isspace(input[idx])) {
        operation += input[idx++];
    }
    while (idx < input.size() && std::isspace(input[idx])) idx++;
    while (idx < input.size()) {
        idx++;
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