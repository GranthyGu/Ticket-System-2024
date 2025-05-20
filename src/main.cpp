// Created by GranthyGu on 2025/5/8

#include "token_scanner.hpp"
#include "STL/vector.hpp"
#include "b_plus_tree.hpp"
#include "ticket_management.hpp"
#include "account_management.hpp"
#include "train_management.hpp"

int main() {
    // freopen("in.txt", "r", stdin);   // 读入in.dat文件作为输入 
	// freopen("out.dat", "w", stdout);  // 将输入写入out.dat文件中 
    train_management train;
    account_management account;
    ticket_management ticket;
    while (true) {
        std::string order;
        if (!getline(std::cin, order)) {
            break;
        }
        token_scanner ts(order);
        if (ts.operation == "add_user") {
            account.add_user(ts);
        } else if (ts.operation == "login") {
            bool flag = account.log_in(ts);
            if (flag) {ticket.log_in_for_account(ts);}
        } else if (ts.operation == "logout") {
            bool flag = account.log_out(ts);
            if (flag) {ticket.log_out_for_account(ts);}
        } else if (ts.operation == "query_profile") {
            account.query_profile(ts);
        } else if (ts.operation == "modify_profile") {
            account.modify_profile(ts);
        } else if (ts.operation == "add_train") {
            train.add_train(ts);
        } else if (ts.operation == "delete_train") {
            train.delete_train(ts);
        } else if (ts.operation == "release_train") {
            train.release_train(ts);
        } else if (ts.operation == "query_train") {
            train.query_train(ts);
        } else if (ts.operation == "query_ticket") {
            train.query_ticket(ts);
        } else if (ts.operation == "query_transfer") {
            train.query_transfer(ts);
        } else if (ts.operation == "buy_ticket") {
            ticket.buy_ticket(ts);
        } else if (ts.operation == "query_order") {
            ticket.query_order(ts);
        } else if (ts.operation == "refund_ticket") {
            ticket.refund_ticket(ts);
        } else if (ts.operation == "clean") {
            ticket.clear();
        } else if (ts.operation == "exit") {
            ticket.exit();
            account.exit();
            train.exit();
            std::cout << '[' << ts.time << ']' << " bye" << std::endl;
        }
    }
}
