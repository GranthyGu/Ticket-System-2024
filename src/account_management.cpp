// Created by GranthyGu on 2025/5/10

#include "account_management.hpp"

username::username() {}
username::username(std::string str) {
    for (int i = 0; i < str.size(); i++) {
        username_[i] = str[i];
    }
}
username username::operator=(const username& other) {
    for (int i = 0; i < 20; i++) {
        username_[i] = other.username_[i];
    }
}
bool username::operator<(const username& other) {
    for (int i = 0; i < 20; i++) {
        if (username_[i] != other.username_[i]) {
            return (username_[i] < other.username_[i]);
        }
    }
    return false;
}
bool username::operator>(const username& other) {
    for (int i = 0; i < 20; i++) {
        if (username_[i] != other.username_[i]) {
            return (username_[i] > other.username_[i]);
        }
    }
    return false;
}
bool username::operator==(const username& other) {
    for (int i = 0; i < 20; i++) {
        if (username_[i] != other.username_[i]) {
            return false;
        }
    }
    return true;
}

password::password() {}
password::password(std::string str) {
    for (int i = 0; i < str.size(); i++) {
        password_[i] = str[i];
    }
}
password password::operator=(const password& other) {
    for (int i = 0; i < 30; i++) {
        password_[i] = other.password_[i];
    }
}

name::name() {}
name::name(std::string str) {
    for (int i = 0; i < str.size(); i++) {
        name_[i] = str[i];
    }
}
name name::operator=(const name& other) {
    for (int i = 0; i < 20; i++) {
        name_[i] = other.name_[i];
    }
}

mail_address::mail_address() {}
mail_address::mail_address(std::string str) {
    for (int i = 0; i < str.size(); i++) {
        mail_address_[i] = str[i];
    }
}
mail_address mail_address::operator=(const mail_address& other) {
    for (int i = 0; i < 30; i++) {
        mail_address_[i] = other.mail_address_[i];
    }
}

account_management::account_management() {
    username_password_tree.set_file_name("username_password");
    username_name_tree.set_file_name("username_name");
    username_mail_address_tree.set_file_name("username_mail_address");
    username_privilege_tree.set_file_name("username_privilege");
}
void account_management::add_user(const token_scanner& ts) {
    if (username_password_tree.empty()) {
        std::string username_, password_, name_, mail_address_;
        for (int i = 0; i < ts.key_argument.size(); i++) {
            if (ts.key_argument[i].first == 'u') {
                username_ = ts.key_argument[i].second;
            } else if (ts.key_argument[i].first == 'p') {
                password_ = ts.key_argument[i].second;
            } else if (ts.key_argument[i].first == 'n') {
                name_ = ts.key_argument[i].second;
            } else if (ts.key_argument[i].first == 'm') {
                mail_address_ = ts.key_argument[i].second;
            }
        }
        username_password_tree.insert(username(username_), password(password_));
        username_name_tree.insert(username(username_), name(name_));
        username_mail_address_tree.insert(username(username_), mail_address(mail_address_));
        username_privilege_tree.insert(username(username_), 10);
        std::cout << '[' << ts.time << ']' << ' ' << 0 << std::endl;
    } else {
        std::string cur_username, username_, password_, name_, mail_address_, privilege_;
        for (int i = 0; i < ts.key_argument.size(); i++) {
            if (ts.key_argument[i].first == 'u') {
                username_ = ts.key_argument[i].second;
            } else if (ts.key_argument[i].first == 'p') {
                password_ = ts.key_argument[i].second;
            } else if (ts.key_argument[i].first == 'n') {
                name_ = ts.key_argument[i].second;
            } else if (ts.key_argument[i].first == 'm') {
                mail_address_ = ts.key_argument[i].second;
            } else if (ts.key_argument[i].first == 'c') {
                cur_username = ts.key_argument[i].second;
            } else if (ts.key_argument[i].first == 'g') {
                privilege_ = ts.key_argument[i].second;
            }
        }
        bool flag = false;
        for (int i = 0; i < log_in_stack.size(); i++) {
            if (std::string(log_in_stack[i].first.username_) == cur_username) {
                flag = true;
                if (log_in_stack[i].second <= std::stoi(privilege_)) {
                    std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
                    return;
                }
                break;
            }
        }
        if (flag == false) {
            std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
            return;
        }
        flag = username_privilege_tree.insert(username(username_), std::stoi(privilege_));
        if (flag == false) {
            std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
            return;
        }
        username_name_tree.insert(username(username_), name(name_));
        username_mail_address_tree.insert(username(username_), mail_address(mail_address_));
        username_password_tree.insert(username(username_), password(password_));
        std::cout << '[' << ts.time << ']' << ' ' << 0 << std::endl;
        return;
    }
}
void account_management::log_in(const token_scanner& ts) {
    std::string username_, password_;
    for (int i = 0; i < ts.key_argument.size(); i++) {
        if (ts.key_argument[i].first == 'u') {
            username_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'p') {
            password_ = ts.key_argument[i].second;
        }
    }
    for (int i = 0; i < log_in_stack.size(); i++) {
        if (std::string(log_in_stack[i].first.username_) == username_) {
            std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
            return;
        }
    }
    sjtu::vector<std::pair<username, password>> find_result = username_password_tree.find(username(username_), username(username_));
    if (find_result.empty()) {
        std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
        return;
    }
    std::string pw(find_result[0].second.password_);
    if (pw != password_) {
        std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
        return;
    }
    sjtu::vector<std::pair<username, int>> find_result_ = username_privilege_tree.find(username(username_), username(username_));
    log_in_stack.push_back(std::make_pair(username(username_), find_result_[0].second));
    std::cout << '[' << ts.time << ']' << ' ' << 0 << std::endl;
    return;
}
void account_management::log_out(const token_scanner& ts) {
    std::string username_ = ts.key_argument[0].second;
    for (int i = 0; i < log_in_stack.size(); i++) {
        if (std::string(log_in_stack[i].first.username_) == username_) {
            log_in_stack.erase(i);
            std::cout << '[' << ts.time << ']' << ' ' << 0 << std::endl;
            return;
        }
    }
    std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
    return;
}
void account_management::query_profile(const token_scanner& ts) {
    std::string username_, cur_username;
    for (int i = 0; i < ts.key_argument.size(); i++) {
        if (ts.key_argument[i].first == 'u') {
            username_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'c') {
            cur_username = ts.key_argument[i].second;
        }
    }
    bool flag = false;
    int current_privilege;
    for (int i = 0; i < log_in_stack.size(); i++) {
        if (std::string(log_in_stack[i].first.username_) == cur_username) {
            flag = true;
            current_privilege = log_in_stack[i].second;
            break;
        }
    }
    if (flag == false) {
        std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
        return;
    }
    if (cur_username == username_) {
        std::cout << '[' << ts.time << ']' << ' ' << username_
                << ' ' << username_name_tree.find(username(username_), username(username_))[0].second.name_
                << ' ' << username_mail_address_tree.find(username(username_), username(username_))[0].second.mail_address_
                << ' ' << current_privilege << std::endl;
        return;
    } else {
        sjtu::vector<std::pair<username, int>> find_result = username_privilege_tree.find(username(username_), username(username_));
        if (find_result.empty()) {
            std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
            return;
        }
        int privilege = find_result[0].second;
        if (privilege >= current_privilege) {
            std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
            return;
        }
        std::cout << '[' << ts.time << ']' << ' ' << username_
                << ' ' << username_name_tree.find(username(username_), username(username_))[0].second.name_
                << ' ' << username_mail_address_tree.find(username(username_), username(username_))[0].second.mail_address_
                << ' ' << privilege << std::endl;
        return;
    }
}
void account_management::modify_profile(const token_scanner& ts) {
    bool change_password, change_name, change_mail, change_privilege;
    change_mail = change_name = change_password = change_privilege = false;
    std::string cur_username, username_, password_, name_, mail_, privilege_;
    for (int i = 0; i < ts.key_argument.size(); i++) {
        if (ts.key_argument[i].first == 'u') {
            username_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'c') {
            cur_username = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'p') {
            change_password = true;
            password_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'n') {
            change_name = true;
            name_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'm') {
            change_mail = true;
            mail_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'g') {
            change_privilege = true;
            privilege_ = ts.key_argument[i].second;
        }
    }
    bool flag = false;
    int current_privilege;
    for (int i = 0; i < log_in_stack.size(); i++) {
        if (std::string(log_in_stack[i].first.username_) == cur_username) {
            flag = true;
            current_privilege = log_in_stack[i].second;
            break;
        }
    }
    if (flag == false) {
        std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
        return;
    }
    if (std::stoi(privilege_) >= current_privilege) {
        std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
        return;
    }
    if (cur_username != username_) {
        sjtu::vector<std::pair<username, int>> find_result = username_privilege_tree.find(username(username_), username(username_));
        if (find_result.empty()) {
            std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
            return;
        }
        int privilege_ = find_result[0].second;
        if (privilege_ >= current_privilege) {
            std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
            return;
        }
    }
    if (change_password) {
        username_password_tree.remove(username(username_));
        username_password_tree.insert(username(username_), password(password_));
    }
    if (change_name) {
        username_name_tree.remove(username(username_));
        username_name_tree.insert(username(username_), name(name_));
    }
    if (change_mail) {
        username_mail_address_tree.remove(username(username_));
        username_mail_address_tree.insert(username(username_), mail_address(mail_));
    }
    if (change_privilege) {
        username_privilege_tree.remove(username(username_));
        username_privilege_tree.insert(username(username_), std::stoi(privilege_));
    }
    std::cout << '[' << ts.time << ']' << ' ' << username_
            << ' ' << username_name_tree.find(username(username_), username(username_))[0].second.name_
            << ' ' << username_mail_address_tree.find(username(username_), username(username_))[0].second.mail_address_
            << ' ' << username_privilege_tree.find(username(username_), username(username_))[0].second << std::endl;
    return;
}