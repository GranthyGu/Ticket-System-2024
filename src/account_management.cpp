// Created by GranthyGu on 2025/5/10

#include "account_management.hpp"

username_password::username_password() {}
username_password::username_password(std::string str1, std::string str2) {
    for (int i = 0; i < str1.length(); i++) {
        username[i] = str1[i];
    }
    for (int i = 0; i < str2.length(); i++) {
        password[i] = str2[i];
    }
}
username_password username_password::operator=(const username_password& other) {
    for (int i = 0; i < 20; i++) {
        username[i] = other.username[i];
    }
    for (int i = 0; i < 30; i++) {
        password[i] = other.password[i];
    }
}
bool username_password::operator<(const username_password& other) {
    for (int i = 0; i < 20; i++) {
        if (username[i] != other.username[i]) {
            return username[i] < other.username[i];
        }
    }
    return false;
}
bool username_password::operator>(const username_password& other) {
    for (int i = 0; i < 20; i++) {
        if (username[i] != other.username[i]) {
            return username[i] > other.username[i];
        }
    }
    return false;
}
bool username_password::operator==(const username_password& other) {
    for (int i = 0; i < 20; i++) {
        if (username[i] != other.username[i]) {
            return false;
        }
    }
    return true;
}

username_name::username_name() {}
username_name::username_name(std::string str1, std::string str2) {
    for (int i = 0; i < str1.length(); i++) {
        username[i] = str1[i];
    }
    for (int i = 0; i < str2.length(); i++) {
        name[i] = str2[i];
    }
}
username_name username_name::operator=(const username_name& other) {
    for (int i = 0; i < 20; i++) {
        username[i] = other.username[i];
    }
    for (int i = 0; i < 20; i++) {
        name[i] = other.name[i];
    }
}
bool username_name::operator<(const username_name& other) {
    for (int i = 0; i < 20; i++) {
        if (username[i] != other.username[i]) {
            return username[i] < other.username[i];
        }
    }
    return false;
}
bool username_name::operator>(const username_name& other) {
    for (int i = 0; i < 20; i++) {
        if (username[i] != other.username[i]) {
            return username[i] > other.username[i];
        }
    }
    return false;
}
bool username_name::operator==(const username_name& other) {
    for (int i = 0; i < 20; i++) {
        if (username[i] != other.username[i]) {
            return false;
        }
    }
    return true;
}

username_mail_address::username_mail_address() {}
username_mail_address::username_mail_address(std::string str1, std::string str2) {
    for (int i = 0; i < str1.length(); i++) {
        username[i] = str1[i];
    }
    for (int i = 0; i < str2.length(); i++) {
        mail_address[i] = str2[i];
    }
}
username_mail_address username_mail_address::operator=(const username_mail_address& other) {
    for (int i = 0; i < 20; i++) {
        username[i] = other.username[i];
    }
    for (int i = 0; i < 30; i++) {
        mail_address[i] = other.mail_address[i];
    }
}
bool username_mail_address::operator<(const username_mail_address& other) {
    for (int i = 0; i < 20; i++) {
        if (username[i] != other.username[i]) {
            return username[i] < other.username[i];
        }
    }
    return false;
}
bool username_mail_address::operator>(const username_mail_address& other) {
    for (int i = 0; i < 20; i++) {
        if (username[i] != other.username[i]) {
            return username[i] > other.username[i];
        }
    }
    return false;
}
bool username_mail_address::operator==(const username_mail_address& other) {
    for (int i = 0; i < 20; i++) {
        if (username[i] != other.username[i]) {
            return false;
        }
    }
    return true;
}

username_privilege::username_privilege() {}
username_privilege::username_privilege(std::string str1, std::string str2) {
    for (int i = 0; i < str1.length(); i++) {
        username[i] = str1[i];
    }
    privilege = std::stoi(str2);
}
username_privilege username_privilege::operator=(const username_privilege& other) {
    for (int i = 0; i < 20; i++) {
        username[i] = other.username[i];
    }
    privilege = other.privilege;
}
bool username_privilege::operator<(const username_privilege& other) {
    for (int i = 0; i < 20; i++) {
        if (username[i] != other.username[i]) {
            return username[i] < other.username[i];
        }
    }
    return false;
}
bool username_privilege::operator>(const username_privilege& other) {
    for (int i = 0; i < 20; i++) {
        if (username[i] != other.username[i]) {
            return username[i] > other.username[i];
        }
    }
    return false;
}
bool username_privilege::operator==(const username_privilege& other) {
    for (int i = 0; i < 20; i++) {
        if (username[i] != other.username[i]) {
            return false;
        }
    }
    return true;
}

account_management::account_management() {
    username_password_tree.set_file_name("username_password");
    username_name_tree.set_file_name("username_name");
    username_mail_address_tree.set_file_name("username_mail_address");
    username_privilege_tree.set_file_name("username_privilege");
}
void account_management::add_user(const token_scanner& ts) {
    if (username_password_tree.empty()) {
        std::string username, password, name, mail_address;
        for (int i = 0; i < ts.key_argument.size(); i++) {
            if (ts.key_argument[i].first == 'u') {
                username = ts.key_argument[i].second;
            } else if (ts.key_argument[i].first == 'p') {
                password = ts.key_argument[i].second;
            } else if (ts.key_argument[i].first == 'n') {
                name = ts.key_argument[i].second;
            } else if (ts.key_argument[i].first == 'm') {
                mail_address = ts.key_argument[i].second;
            }
        }
        username_password_tree.insert(username_password(username, password));
        username_name_tree.insert(username_name(username, name));
        username_mail_address_tree.insert(username_mail_address(username, mail_address));
        username_privilege_tree.insert(username_privilege(username, "10"));
        std::cout << '[' << ts.time << ']' << ' ' << 0 << std::endl;
    } else {
        std::string cur_username, username, password, name, mail_address, privilege;
        for (int i = 0; i < ts.key_argument.size(); i++) {
            if (ts.key_argument[i].first == 'u') {
                username = ts.key_argument[i].second;
            } else if (ts.key_argument[i].first == 'p') {
                password = ts.key_argument[i].second;
            } else if (ts.key_argument[i].first == 'n') {
                name = ts.key_argument[i].second;
            } else if (ts.key_argument[i].first == 'm') {
                mail_address = ts.key_argument[i].second;
            } else if (ts.key_argument[i].first == 'c') {
                cur_username = ts.key_argument[i].second;
            } else if (ts.key_argument[i].first == 'g') {
                privilege = ts.key_argument[i].second;
            }
        }
        bool flag = false;
        for (int i = 0; i < log_in_stack.size(); i++) {
            if (std::string(log_in_stack[i].username) == cur_username) {
                flag = true;
                if (log_in_stack[i].privilege <= std::stoi(privilege)) {
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
        flag = username_privilege_tree.insert(username_privilege(username, privilege));
        if (flag == false) {
            std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
            return;
        }
        username_name_tree.insert(username_name(username, name));
        username_mail_address_tree.insert(username_mail_address(username, mail_address));
        username_password_tree.insert(username_password(username, password));
        std::cout << '[' << ts.time << ']' << ' ' << 0 << std::endl;
        return;
    }
}
void account_management::log_in(const token_scanner& ts) {
    std::string username, password;
    for (int i = 0; i < ts.key_argument.size(); i++) {
        if (ts.key_argument[i].first == 'u') {
            username = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'p') {
            password = ts.key_argument[i].second;
        }
    }
    for (int i = 0; i < log_in_stack.size(); i++) {
        if (std::string(log_in_stack[i].username) == username) {
            std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
            return;
        }
    }
    sjtu::vector<username_password> find_result = username_password_tree.find(username_password(username, "1"), username_password(username, "1"));
    if (find_result.empty()) {
        std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
        return;
    }
    std::string pw(find_result[0].password);
    if (pw != password) {
        std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
        return;
    }
    sjtu::vector<username_privilege> find_result_ = username_privilege_tree.find(username_privilege(username, "1"), username_privilege(username, "1"));
    log_in_stack.push_back(username_privilege(username, std::to_string(find_result_[0].privilege)));
    std::cout << '[' << ts.time << ']' << ' ' << 0 << std::endl;
    return;
}
void account_management::log_out(const token_scanner& ts) {
    std::string username = ts.key_argument[0].second;
    for (int i = 0; i < log_in_stack.size(); i++) {
        if (std::string(log_in_stack[i].username) == username) {
            log_in_stack.erase(i);
            std::cout << '[' << ts.time << ']' << ' ' << 0 << std::endl;
            return;
        }
    }
    std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
    return;
}
void account_management::query_profile(const token_scanner& ts) {
    std::string username, cur_username;
    for (int i = 0; i < ts.key_argument.size(); i++) {
        if (ts.key_argument[i].first == 'u') {
            username = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'c') {
            cur_username = ts.key_argument[i].second;
        }
    }
    bool flag = false;
    int current_privilege;
    for (int i = 0; i < log_in_stack.size(); i++) {
        if (std::string(log_in_stack[i].username) == cur_username) {
            flag = true;
            current_privilege = log_in_stack[i].privilege;
            break;
        }
    }
    if (flag == false) {
        std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
        return;
    }
    if (cur_username == username) {
        std::cout << '[' << ts.time << ']' << ' ' << username 
                << ' ' << username_name_tree.find(username_name(username, "1"), username_name(username, "1"))[0].name
                << ' ' << username_mail_address_tree.find(username_mail_address(username, "1"), username_mail_address(username, "1"))[0].mail_address
                << ' ' << current_privilege << std::endl;
        return;
    } else {
        sjtu::vector<username_privilege> find_result = username_privilege_tree.find(username_privilege(username, "1"), username_privilege(username, "1"));
        if (find_result.empty()) {
            std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
            return;
        }
        int privilege = find_result[0].privilege;
        if (privilege >= current_privilege) {
            std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
            return;
        }
        std::cout << '[' << ts.time << ']' << ' ' << username 
                << ' ' << username_name_tree.find(username_name(username, "1"), username_name(username, "1"))[0].name
                << ' ' << username_mail_address_tree.find(username_mail_address(username, "1"), username_mail_address(username, "1"))[0].mail_address
                << ' ' << privilege << std::endl;
        return;
    }
}
void account_management::modify_profile(const token_scanner& ts) {
    bool change_password, change_name, change_mail, change_privilege;
    change_mail = change_name = change_password = change_privilege = false;
    std::string cur_username, username, password, name, mail, privilege;
    for (int i = 0; i < ts.key_argument.size(); i++) {
        if (ts.key_argument[i].first == 'u') {
            username = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'c') {
            cur_username = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'p') {
            change_password = true;
            password = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'n') {
            change_name = true;
            name = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'm') {
            change_mail = true;
            mail = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'g') {
            change_privilege = true;
            privilege = ts.key_argument[i].second;
        }
    }
    bool flag = false;
    int current_privilege;
    for (int i = 0; i < log_in_stack.size(); i++) {
        if (std::string(log_in_stack[i].username) == cur_username) {
            flag = true;
            current_privilege = log_in_stack[i].privilege;
            break;
        }
    }
    if (flag == false) {
        std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
        return;
    }
    if (std::stoi(privilege) >= current_privilege) {
        std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
        return;
    }
    if (cur_username != username) {
        sjtu::vector<username_privilege> find_result = username_privilege_tree.find(username_privilege(username, "1"), username_privilege(username, "1"));
        if (find_result.empty()) {
            std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
            return;
        }
        int privilege_ = find_result[0].privilege;
        if (privilege_ >= current_privilege) {
            std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
            return;
        }
    }
    if (change_password) {
        username_password new_password(username, password);
        username_password_tree.remove(username_password(username, "1"));
        username_password_tree.insert(new_password);
    }
    if (change_name) {
        username_name new_name(username, password);
        username_name_tree.remove(username_name(username, "1"));
        username_name_tree.insert(new_name);
    }
    if (change_mail) {
        username_mail_address new_mail(username, mail);
        username_mail_address_tree.remove(username_mail_address(username, "1"));
        username_mail_address_tree.insert(new_mail);
    }
    if (change_privilege) {
        username_privilege new_privilege(username, privilege);
        username_privilege_tree.remove(username_privilege(username, "1"));
        username_privilege_tree.insert(new_privilege);
    }
    std::cout << '[' << ts.time << ']' << ' ' << username 
            << ' ' << username_name_tree.find(username_name(username, "1"), username_name(username, "1"))[0].name
            << ' ' << username_mail_address_tree.find(username_mail_address(username, "1"), username_mail_address(username, "1"))[0].mail_address
            << ' ' << username_privilege_tree.find(username_privilege(username, "1"), username_privilege(username, "1"))[0].privilege << std::endl;
    return;
}