// Created by GranthyGu on 2025/5/19

#include "ticket_management.hpp"

key_for_ticket::key_for_ticket() {}
key_for_ticket::key_for_ticket(int t, std::string str1, std::string str2) {
    time_ = t;
    train_id temp(str1);
    id = temp;
    date temp_(str2);
    date_ = temp_;
}
key_for_ticket::key_for_ticket(const key_for_ticket& other) {
    time_ = other.time_;
    id = other.id;
    date_ = other.date_;
}
key_for_ticket& key_for_ticket::operator=(const key_for_ticket& other) {
    time_ = other.time_;
    id = other.id;
    date_ = other.date_;
    return *this;
}
bool key_for_ticket::operator<(const key_for_ticket& other) const {
    if (!(id == other.id)) {
        return id < other.id;
    }
    if (date_ != other.date_) {
        return date_ < other.date_;
    }
    return time_ < other.time_;
}
bool key_for_ticket::operator>(const key_for_ticket& other) const {
    if (!(id == other.id)) {
        return id > other.id;
    }
    if (date_ != other.date_) {
        return date_ > other.date_;
    }
    return time_ > other.time_;
}
bool key_for_ticket::operator==(const key_for_ticket& other) const {
    if (!(id == other.id)) {
        return false;
    }
    if (date_ != other.date_) {
        return false;
    }
    return time_ == other.time_;
}

info_for_ticket::info_for_ticket() {}
info_for_ticket::info_for_ticket(std::string d, std::string str1, std::string str2, std::string str3, std::string str4) {
    for (int i = 0; i < str1.size(); i++) {
        user_name[i] = str1[i];
    }
    for (int i = 0; i < str2.size(); i++) {
        from[i] = str2[i];
    }
    for (int i = 0; i < str3.size(); i++) {
        to[i] = str3[i];
    }
    date temp(d);
    date_ = temp;
    num = std::stoi(str4);
}
info_for_ticket::info_for_ticket(const info_for_ticket& other) {
    date_ = other.date_;
    for (int i = 0; i < 20; i++) {
        user_name[i] = other.user_name[i];
    }
    for (int i = 0; i < 30; i++) {
        from[i] = other.from[i];
        to[i] = other.to[i];
    }
    num = other.num;
}
info_for_ticket& info_for_ticket::operator=(const info_for_ticket& other) {
    date_ = other.date_;
    for (int i = 0; i < 20; i++) {
        user_name[i] = other.user_name[i];
    }
    for (int i = 0; i < 30; i++) {
        from[i] = other.from[i];
        to[i] = other.to[i];
    }
    num = other.num;
    return *this;
}

key_for_ticket_user::key_for_ticket_user() {}
key_for_ticket_user::key_for_ticket_user(int t, std::string str1) {
    time_ = t;
    for (int i = 0; i < 20; i++) {
        user_name[i] = str1[i];
    }
}
key_for_ticket_user::key_for_ticket_user(const key_for_ticket_user& other) {
    time_ = other.time_;
    for (int i = 0; i < 20; i++) {
        user_name[i] = other.user_name[i];
    }
}
key_for_ticket_user& key_for_ticket_user::operator=(const key_for_ticket_user& other) {
    time_ = other.time_;
    for (int i = 0; i < 20; i++) {
        user_name[i] = other.user_name[i];
    }
    return *this;
}
bool key_for_ticket_user::operator<(const key_for_ticket_user& other) const {
    if(const auto res = strcmp(user_name, other.user_name))
        return res < 0;
    return time_ < other.time_;
}
bool key_for_ticket_user::operator>(const key_for_ticket_user& other) const {
    if(const auto res = strcmp(user_name, other.user_name))
        return res > 0;
    return time_ > other.time_;
}
bool key_for_ticket_user::operator==(const key_for_ticket_user& other) const {
    return strcmp(user_name, other.user_name) == 0 && time_ == other.time_;
}

info_for_ticket_user::info_for_ticket_user() {}
info_for_ticket_user::info_for_ticket_user(std::string str1, std::string str2, std::string str3, std::string str4, std::string str5) {
    train_id temp(str1);
    id = temp;
    date temp_(str2);
    date_ = temp_;
    for (int i = 0; i < str3.size(); i++) {
        from[i] = str3[i];
    }
    for (int i = 0; i < str4.size(); i++) {
        to[i] = str4[i];
    }
    num = std::stoi(str5);
}
info_for_ticket_user::info_for_ticket_user(const info_for_ticket_user& other) {
    id = other.id;
    date_ = other.date_;
    for (int i = 0; i < 30; i++) {
        from[i] = other.from[i];
        to[i] = other.to[i];
    }
    num = other.num;
    statu = other.statu;
}
info_for_ticket_user& info_for_ticket_user::operator=(const info_for_ticket_user& other) {
    id = other.id;
    date_ = other.date_;
    for (int i = 0; i < 30; i++) {
        from[i] = other.from[i];
        to[i] = other.to[i];
    }
    num = other.num;
    statu = other.statu;
    return *this;
}

ticket_management::ticket_management() {
    ticket_list_by_user.set_file_name("ticket_information");
    standby_by_train_date.set_file_name("pending_queue");
}
std::pair<date, int> ticket_management::buy(const key_for_ticket_user& key, const info_for_ticket_user& ticket) {
    train_id id = ticket.id;
    date day = ticket.date_;
    std::string begin_station(ticket.from);
    std::string end_station(ticket.to);
    return train_manage.query_buy(begin_station, end_station, day, id, ticket.num, true);
}
std::pair<date, int> ticket_management::query_pending(const key_for_ticket& key, const info_for_ticket& ticket) {
    train_id id = key.id;
    date day = key.date_;
    std::string begin_(ticket.from);
    std::string end_(ticket.to);
    return train_manage.query_buy(begin_, end_, day, id, ticket.num, false);
}
void ticket_management::buy_ticket(const token_scanner& ts) {
    std::string user_name, ID, date_, from, to, num, pending = "false";
    for (int i = 0; i < ts.key_argument.size(); i++) {
        if (ts.key_argument[i].first == 'u') {
            user_name = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'i') {
            ID = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'd') {
            date_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'f') {
            from = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 't') {
            to = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'n') {
            num = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'q') {
            pending = ts.key_argument[i].second;
        }
    }
    if (!account_manage.log_in_statu(username(user_name))) {
        std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
        return;
    }
    key_for_ticket_user ticket_user(ts.time, user_name);
    info_for_ticket_user info_user(ID, date_, from, to, num);
    std::pair<date, int> flag = buy(ticket_user, info_user);
    if (flag.second == -1) {
        std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
    } else if (flag.second == 0) {
        if (pending == "false") {
            std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
        } else {
            info_user.statu = 2;
            key_for_ticket key_(ts.time, ID, flag.first.to_string());
            info_for_ticket ticket_(flag.first.to_string(), user_name, from, to, num);
            standby_by_train_date.insert(key_, ticket_);
            ticket_list_by_user.insert(ticket_user, info_user);
            std::cout << '[' << ts.time << ']' << ' ' << "queue" << std::endl;
        }
    } else {
        ticket_list_by_user.insert(ticket_user, info_user);
        std::cout << '[' << ts.time << ']' << ' ' << flag.second << std::endl;
    }
}
void ticket_management::query_order(const token_scanner& ts) {
    std::string user_name;
    user_name = ts.key_argument[0].second;
    if (!account_manage.log_in_statu(username(user_name))) {
        std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
        return;
    }
    key_for_ticket_user minimal(-1, user_name);
    key_for_ticket_user maximal(1e9, user_name);
    sjtu::vector<std::pair<key_for_ticket_user, info_for_ticket_user>> result = ticket_list_by_user.find(minimal, maximal);
    std::cout << '[' << ts.time << ']' << ' ' << result.size() << std::endl;
    for (int i = result.size() - 1; i >= 0; i--) {
        key_for_ticket_user user = result[i].first;
        info_for_ticket_user info = result[i].second;
        if (info.statu == 1) {
            std::cout << "[success] ";
        } else if (info.statu == 2) {
            std::cout << "[pending] ";
        } else if (info.statu == 3) {
            std::cout << "[refunded] ";
        }
        std::string begin(info.from), end(info.to);
        Time time_leave = train_manage.query_time(info.id, begin).second;
        Time time_arrive = train_manage.query_time(info.id, end).first;
        date d = info.date_;
        d.add_day(time_arrive.day - time_leave.day);
        std::cout << info.id.id << ' ' << info.from << ' ' 
                  << info.date_.to_string() << ' ' << time_leave.to_string() << " -> "
                  << info.to << ' ' << d.to_string() << ' ' << time_arrive.to_string() << ' '
                  << train_manage.query_price(info.id, begin, end) << ' ' << info.num << std::endl;
    }
    return;
}
void ticket_management::refund_ticket(const token_scanner& ts) {
    std::string user_name;
    int number = 1;
    for (int i = 0; i < ts.key_argument.size(); i++) {
        if (ts.key_argument[i].first == 'u') {
            user_name = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'n') {
            number = std::stoi(ts.key_argument[i].second);
        }
    }
    if (!account_manage.log_in_statu(username(user_name))) {
        std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
        return;
    }
    key_for_ticket_user minimal(-1, user_name);
    key_for_ticket_user maximal(1e9, user_name);
    sjtu::vector<std::pair<key_for_ticket_user, info_for_ticket_user>> result = ticket_list_by_user.find(minimal, maximal);
    if (result.size() < number) {
        std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
        return;
    }
    key_for_ticket_user key = result[result.size() - number].first;
    info_for_ticket_user info = result[result.size() - number].second;
    if (info.statu == 3) {
        std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
        return;
    }
    Time t = train_manage.query_time(info.id, std::string(info.from)).second;
    date d = info.date_;
    d.minus_day(t.day);
    if (info.statu == 2) {
        info.statu = 3;
        ticket_list_by_user.remove(key);
        ticket_list_by_user.insert(key, info);
        standby_by_train_date.remove(key_for_ticket(key.time_, info.id.id, d.to_string()));
        std::cout << '[' << ts.time << ']' << ' ' << 0 << std::endl;
        return;
    }
    info.statu = 3;
    ticket_list_by_user.remove(key);
    ticket_list_by_user.insert(key, info);
    train_manage.refund_ticket(info.id, info.from, info.to, info.date_, info.num);
    key_for_ticket minimal_(-1, std::string(info.id.id), d.to_string());
    key_for_ticket maximal_(1e9, std::string(info.id.id), d.to_string());
    sjtu::vector<std::pair<key_for_ticket, info_for_ticket>> result_ = standby_by_train_date.find(minimal_, maximal_);
    for (int i = 0; i < result_.size(); i++) {
        if (query_pending(result_[i].first, result_[i].second).second > 0) {
            key_for_ticket key_ = result_[i].first;
            info_for_ticket info_ = result_[i].second;
            standby_by_train_date.remove(key_);
            key_for_ticket_user minimal__(key_.time_, std::string(info_.user_name));
            sjtu::vector<std::pair<key_for_ticket_user, info_for_ticket_user>> r = ticket_list_by_user.find(minimal__, minimal__);
            key_for_ticket_user key__ = r[0].first;
            info_for_ticket_user info__ = r[0].second;
            info__.statu = 2;
            ticket_list_by_user.remove(key__);
            ticket_list_by_user.insert(key__, info__);
        }
    }
    std::cout << '[' << ts.time << ']' << ' ' << 0 << std::endl;
    return;
}
void ticket_management::exit() {
    standby_by_train_date.~B_plus_tree();
    ticket_list_by_user.~B_plus_tree();
}
void ticket_management::clear() {
    standby_by_train_date.clear();
    ticket_list_by_user.clear();
}
void ticket_management::add_train_(const token_scanner& ts) {
    train_manage.add_train(ts);
}
void ticket_management::delete_train_(const token_scanner& ts) {
    train_manage.delete_train(ts);
}
void ticket_management::release_train_(const token_scanner& ts) {
    train_manage.release_train(ts);
}
void ticket_management::query_train_(const token_scanner& ts) {
    train_manage.query_train(ts);
}
void ticket_management::query_ticket_(const token_scanner& ts) {
    train_manage.query_ticket(ts);
}
void ticket_management::query_transfer_(const token_scanner& ts) {
    train_manage.query_transfer(ts);
}
void ticket_management::add_user_(const token_scanner& ts) {
    account_manage.add_user(ts);
}
bool ticket_management::log_in_(const token_scanner& ts) {
    return account_manage.log_in(ts);
}
bool ticket_management::log_out_(const token_scanner& ts) {
    return account_manage.log_out(ts);
}
void ticket_management::query_profile_(const token_scanner& ts) {
    account_manage.query_profile(ts);
}
void ticket_management::modify_profile_(const token_scanner& ts) {
    account_manage.modify_profile(ts);
}