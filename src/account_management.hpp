// Created by GranthyGu on 2025/5/8

#ifndef ACCOUNT_MANAGEMENT_HPP
#define ACCOUNT_MANAGEMENT_HPP

#include "b_plus_tree.hpp"
#include "STL/vector.hpp"
#include "token_scanner.hpp"
#include <string>

class username {
public:
    char username_[20];
    username();
    username(std::string);
    username operator=(const username&);
    bool operator<(const username&);
    bool operator>(const username&);
    bool operator==(const username&);
};

class password {
public:
    char password_[30];
    password();
    password(std::string);
    password operator=(const password&);
};

class name {
public:
    char name_[20];
    name();
    name(std::string);
    name operator=(const name&);
};

class mail_address {
public:
    char mail_address_[30];
    mail_address();
    mail_address(std::string);
    mail_address operator=(const mail_address&);
};

class privilege {
public:
    int p;
    privilege();
    privilege(std::string);
    privilege operator=(const privilege&);
};

class account_management {
private:
    B_plus_tree<username, password, 142, 80> username_password_tree;
    B_plus_tree<username, name, 142, 95> username_name_tree;
    B_plus_tree<username, mail_address, 142, 80> username_mail_address_tree;
    B_plus_tree<username, int, 142, 150> username_privilege_tree;
public:
    sjtu::vector<std::pair<username,int>> log_in_stack;
    account_management();
    void add_user(const token_scanner&);
    void log_in(const token_scanner&);
    void log_out(const token_scanner&);
    void query_profile(const token_scanner&);
    void modify_profile(const token_scanner&);
    bool log_in_statu(const username&);
    void exit();
    void clear();
};

#endif      //ACCOUNT_MANAGEMENT_HPP