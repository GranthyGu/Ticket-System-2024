// Created by GranthyGu on 2025/5/8

#ifndef ACCOUNT_MANAGEMENT_HPP
#define ACCOUNT_MANAGEMENT_HPP

#include "b_plus_tree.hpp"
#include "vector/vector.hpp"
#include "token_scanner.hpp"
#include <string>

class username_password {
public:
    char username[20];
    char password[30];
    username_password();
    username_password(std::string, std::string);
    username_password operator=(const username_password&);
    bool operator<(const username_password&);
    bool operator>(const username_password&);
    bool operator==(const username_password&);
};
class username_name {
public:
    char username[20];
    char name[20];
    username_name();
    username_name(std::string, std::string);
    username_name operator=(const username_name&);
    bool operator<(const username_name&);
    bool operator>(const username_name&);
    bool operator==(const username_name&);
};
class username_mail_address {
public:
    char username[20];
    char mail_address[30];
    username_mail_address();
    username_mail_address(std::string, std::string);
    username_mail_address operator=(const username_mail_address&);
    bool operator<(const username_mail_address&);
    bool operator>(const username_mail_address&);
    bool operator==(const username_mail_address&);
};
class username_privilege {
public:
    char username[20];
    int privilege;
    username_privilege();
    username_privilege(std::string, std::string);
    username_privilege operator=(const username_privilege&);
    bool operator<(const username_privilege&);
    bool operator>(const username_privilege&);
    bool operator==(const username_privilege&);
};
class account_management {
private:
    B_plus_tree<username_password, 69, 69> username_password_tree;
    B_plus_tree<username_name, 83, 83> username_name_tree;
    B_plus_tree<username_mail_address, 69, 69> username_mail_address_tree;
    B_plus_tree<username_privilege, 120, 120> username_privilege_tree;
public:
    sjtu::vector<username_privilege> log_in_stack;
    account_management();
    void add_user(const token_scanner&);
    void log_in(const token_scanner&);
    void log_out(const token_scanner&);
    void query_profile(const token_scanner&);
    void modify_profile(const token_scanner&);
};

#endif      //ACCOUNT_MANAGEMENT_HPP