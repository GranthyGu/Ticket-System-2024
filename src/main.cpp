// Created by GranthyGu on 2025/5/8

#include "ticket_management.hpp"
#pragma GCC optimize ("Ofast")

int main() {
    freopen("in.txt", "r", stdin);   // 读入in.dat文件作为输入 
	freopen("out.dat", "w", stdout);  // 将输入写入out.dat文件中 
    ticket_management ticket;
    while (true) {
        std::string order;
        getline(std::cin, order);
        token_scanner ts(order);
        if (ts.operation == "add_user") {
            ticket.add_user_(ts);
        } else if (ts.operation == "login") {
            bool flag = ticket.log_in_(ts);
        } else if (ts.operation == "logout") {
            bool flag = ticket.log_out_(ts);
        } else if (ts.operation == "query_profile") {
            ticket.query_profile_(ts);
        } else if (ts.operation == "modify_profile") {
            ticket.modify_profile_(ts);
        } else if (ts.operation == "add_train") {
            ticket.add_train_(ts);
        } else if (ts.operation == "delete_train") {
            ticket.delete_train_(ts);
        } else if (ts.operation == "release_train") {
            ticket.release_train_(ts);
        } else if (ts.operation == "query_train") {
            ticket.query_train_(ts);
        } else if (ts.operation == "query_ticket") {
            ticket.query_ticket_(ts);
        } else if (ts.operation == "query_transfer") {
            ticket.query_transfer_(ts);
        } else if (ts.operation == "buy_ticket") {
            ticket.buy_ticket(ts);
        } else if (ts.operation == "query_order") {
            ticket.query_order(ts);
        } else if (ts.operation == "refund_ticket") {
            ticket.refund_ticket(ts);
        } else if (ts.operation == "clean") {
            ticket.clear();
        } else if (ts.operation == "exit") {
            std::cout << '[' << ts.time << ']' << " bye" << std::endl;
            break;
        }
    }
}
