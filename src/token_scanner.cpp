// Created by GranthyGu on 2025/5/8

#include "token_scanner.hpp"

token_scanner::token_scanner(const std::string& input) {
    int idx = 1;
    std::string time_;
    while (input[idx] != ']') {
        time_ += input[idx];
        idx++;
    }
    time = std::stoi(time_);
    idx += 2;
    while (input[idx] != ' ' && idx < input.size()) {
        operation += input[idx];
        idx++;
    }
    idx++;
    while (idx < input.length()) {
        std::pair<char, std::string> key_argument_temp;
        if (input[idx] == '-') {
            idx++;
            key_argument_temp.first = input[idx];
            idx += 2;
        }
        std::string argument;
        while (idx < input.length() && input[idx] != ' ') {
            argument += input[idx];
            idx++;
        }
        key_argument_temp.second = argument;
        key_argument.push_back(key_argument_temp);
        idx++;
    }
}