// Created by GranthyGu on 2025/5/8

#ifndef TOKEN_SCANNER_HPP
#define TOKEN_SCANNER_HPP

/*
 * File: token_scanner.hpp
 * --------------------
 * This file exports a token_scanner class which can logically scan the input string.
 *
 * - Way to use:
 *      token_scanner ts(std::string str);
 *
 * - Way to get the logical tokens:
 *      sjtu::vector<std:pair<char, std::string>> tokens = ts.key_argument;
 */

#include <iostream>
#include <string>
#include "STL/vector.hpp"

class token_scanner {
public:
    int time;
    std::string operation;
    sjtu::vector<std::pair<char, std::string>> key_argument;
    token_scanner(const std::string& input);
};

#endif      //TOKEN_SCANNER_HPP