// Created by GranthyGu on 2025/5/8

#ifndef ACCOUNT_MANAGEMENT_HPP
#define ACCOUNT_MANAGEMENT_HPP

#include "b_plus_tree.hpp"
#include "STL/vector.hpp"
#include "token_scanner.hpp"
#include <string>

class username {
public:
    char username_[21] = {0};
    username();
    username(std::string);
    username& operator=(const username&);
    bool operator<(const username&) const;
    bool operator>(const username&) const;
    bool operator==(const username&) const;
};

class password {
public:
    char password_[31] = {0};
    password();
    password(std::string);
    password& operator=(const password&);
};

class name {
public:
    char name_[21] = {0};
    name();
    name(std::string);
    name& operator=(const name&);
};

class mail_address {
public:
    char mail_address_[31] = {0};
    mail_address();
    mail_address(std::string);
    mail_address& operator=(const mail_address&);
};

class account_management {
private:
    B_plus_tree<username, password, 60, 40> username_password_tree;
    B_plus_tree<username, name, 60, 40> username_name_tree;
    B_plus_tree<username, mail_address, 60, 40> username_mail_address_tree;
    B_plus_tree<username, int, 60, 40> username_privilege_tree;
public:
    sjtu::vector<std::pair<username,int>> log_in_stack;
    account_management();
    void add_user(const token_scanner&);
    bool log_in(const token_scanner&);
    bool log_out(const token_scanner&);
    void query_profile(const token_scanner&);
    void modify_profile(const token_scanner&);
    bool log_in_statu(const username&);
    void exit();
    void clear();
};

#endif      //ACCOUNT_MANAGEMENT_HPP