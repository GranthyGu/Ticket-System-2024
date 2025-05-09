// Created by GranthyGu on 2025/5/8

#include "token_scanner.hpp"
#include "vector/vector.hpp"
#include <string>
#include <iostream>
int main() {
    token_scanner ts(">[666] add_train -i HAPPY_TRAINA -n 3 -m 1000 -s 上院|中院|下院 -p 114|514 -x 19:19 -t 600|600 -o 5 -d 06-01|08-17 -y G");
    sjtu::vector<std::pair<char, std::string>> v = ts.key_argument;
    std::cout << ts.time << std::endl;
    std::cout << ts.operation << std::endl;
    for (auto tmp : v)
    {
        std::cout << tmp.first << ' ' << tmp.second << std::endl;
    }
    

}