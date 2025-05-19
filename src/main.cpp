// Created by GranthyGu on 2025/5/8

#include "token_scanner.hpp"
#include "STL/vector.hpp"
#include "b_plus_tree.hpp"
#include <string>
#include <iostream>
class key_value {
public:
    char key[65] = {};
    int value;
public:
    key_value() : value(INT_MIN) {}
    key_value(char* key_, int value_) : value(value_) {
        for (int i = 0; i < 65; i++) {
            key[i] = key_[i];
        }
    }
    key_value(std::string key_, int value_) : value(value_) {
        for (int i = 0; i < key_.length(); i++) {
            key[i] = key_[i];
        }
    }
    key_value operator=(const key_value& other) {
        for (int i = 0; i < 65; i++) {
            key[i] = other.key[i];
        }
        value = other.value;
        return *this;
    }
    bool operator<(const key_value& other) {
        for (int i = 0; i < 65; i++) {
            if (key[i] != other.key[i]) {
                return (key[i] < other.key[i]);
            }
        }
        return (value < other.value);
    }
    bool operator>(const key_value& other) {
        for (int i = 0; i < 65; i++) {
            if (key[i] != other.key[i]) {
                return (key[i] > other.key[i]);
            }
        }
        return (value > other.value);
    }
    bool operator==(const key_value& other) const {
        for (int i = 0; i < 65; i++) {
            if (key[i] != other.key[i]) {
                return false;
            }
        }
        return (value == other.value);
    }
};
int main() {
    int n;
    std::cin >> n;
    B_plus_tree<key_value, key_value, 51, 28> bpt("File_for_bpt");
    for (int i = 0; i < n; i++) {
        std::string operation;
        std::cin >> operation;
        if (operation == "insert") {
            std::string key;
            int value;
            std::cin >> key >> value;
            key_value tmp(key, value);
            bpt.insert(tmp, tmp);
        }
        if (operation == "delete") {
            std::string key;
            int value;
            std::cin >> key >> value;
            key_value tmp(key, value);
            bpt.remove(tmp);
        }
        if (operation == "find") {
            std::string key;
            std::cin >> key;
            key_value minimal(key, 5);
            key_value maximal(key, 5);
            sjtu::vector<std::pair<key_value, key_value>> tmp = bpt.find(minimal, maximal);
            if (tmp.size() == 0) {
                std::cout << "null" << std::endl;
            } else {
                for (int i = 0; i < tmp.size(); i++) {
                    std::cout << tmp[i].first.value << ' ';
                }
                std::cout << std::endl;
            }
        }
    }
    bpt.put_root();
}