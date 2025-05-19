// // Created by GranthyGu on 2025/5/8

// #include "token_scanner.hpp"
// #include "STL/vector.hpp"
// #include "b_plus_tree.hpp"
// #include "ticket_management.hpp"
// #include "account_management.hpp"
// #include "train_management.hpp"

// int main() {
//     B_plus_tree<username, int, 100, 100> h;
//     h.set_file_name("111");
//     h.insert(username("114514"), 10);
//     sjtu::vector<std::pair<username, int>> r = h.find(username("114514"), username("114514"));
//     std::cout << r[0].second << std::endl;
//     // freopen("in.txt", "r", stdin);   // 读入in.dat文件作为输入 
// 	// freopen("out.dat", "w", stdout);  // 将输入写入out.dat文件中 
//     // train_management train;
//     // account_management account;
//     // ticket_management ticket;
//     // while (true) {
//     //     std::string order;
//     //     if (!getline(std::cin, order)) {
//     //         break;
//     //     }
//     //     token_scanner ts(order);
//     //     if (ts.operation == "add_user") {
//     //         account.add_user(ts);
//     //     } else if (ts.operation == "login") {
//     //         account.log_in(ts);
//     //     } else if (ts.operation == "logout") {
//     //         account.log_out(ts);
//     //     } else if (ts.operation == "query_profile") {
//     //         account.query_profile(ts);
//     //     } else if (ts.operation == "modify_profile") {
//     //         account.modify_profile(ts);
//     //     } else if (ts.operation == "add_train") {
//     //         train.add_train(ts);
//     //     } else if (ts.operation == "delete_train") {
//     //         train.delete_train(ts);
//     //     } else if (ts.operation == "release_train") {
//     //         train.release_train(ts);
//     //     } else if (ts.operation == "query_train") {
//     //         train.query_train(ts);
//     //     } else if (ts.operation == "query_ticket") {
//     //         train.query_ticket(ts);
//     //     } else if (ts.operation == "query_transfer") {
//     //         train.query_transfer(ts);
//     //     } else if (ts.operation == "buy_ticket") {
//     //         ticket.buy_ticket(ts);
//     //     } else if (ts.operation == "query_order") {
//     //         ticket.query_order(ts);
//     //     } else if (ts.operation == "refund_ticket") {
//     //         ticket.refund_ticket(ts);
//     //     } else if (ts.operation == "clean") {
//     //         ticket.clear();
//     //     } else if (ts.operation == "exit") {
//     //         ticket.exit();
//     //         account.exit();
//     //         train.exit();
//     //     }
//     // }
// }
#include "b_plus_tree.hpp"

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
    B_plus_tree<key_value, key_value, 52, 26> bpt("File_for_bpt");
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
            key_value minimal(key, INT_MIN);
            key_value maximal(key, INT_MAX);
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