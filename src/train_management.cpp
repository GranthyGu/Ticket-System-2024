// Created by GranthyGu on 2025/5/16

#include "train_management.hpp"

train_id::train_id() {}
train_id::train_id(std::string str) {
    for (int i = 0; i < str.size(); i++) {
        id[i] = str[i];
    }
}
train_id::train_id(const train_id& other) {
    for (int i = 0; i < 20; i++) {
        id[i] = other.id[i];
    }
}
train_id& train_id::operator=(const train_id& other) {
    for (int i = 0; i < 20; i++) {
        id[i] = other.id[i];
    }
    return *this;
}
bool train_id::operator<(const train_id& other) const {
    return strcmp(id,other.id) < 0;
}
bool train_id::operator>(const train_id& other) const {
    return strcmp(id,other.id) > 0;
}
bool train_id::operator==(const train_id& other) const {
    return strcmp(id,other.id) == 0;
}

station::station() {}
station::station(std::string str, std::string str_, Time t, Time t_) {
    for (int i = 0; i < str.size(); i++) {
        station_name[i] = str[i];
    }
    train_id id_(str_);
    id = id_;
    time_arrival = t;
    time_leave = t_;
}
station::station(std::string str, train_id id_, Time t, Time t_) {
    for (int i = 0; i < str.size(); i++) {
        station_name[i] = str[i];
    }
    id = id_;
    time_arrival = t;
    time_leave = t_;
}
station::station(const station& other) {
    for (int i = 0; i < 30; i++) {
        station_name[i] = other.station_name[i];
    }
    id = other.id;
    time_arrival = other.time_arrival;
    time_leave = other.time_leave;
}
station& station::operator=(const station& other) {
    for (int i = 0; i < 30; i++) {
        station_name[i] = other.station_name[i];
    }
    id = other.id;
    time_arrival = other.time_arrival;
    time_leave = other.time_leave;
    return *this;
}
bool station::operator<(const station& other) const {
    int res = strcmp(station_name, other.station_name);
    if (res != 0) {
        return res < 0;
    }
    return id < other.id;
}
bool station::operator>(const station& other) const {
    int res = strcmp(station_name, other.station_name);
    if (res != 0) {
        return res > 0;
    }
    return id > other.id;
}
bool station::operator==(const station& other) const {
    int res = strcmp(station_name, other.station_name);
    if (res != 0) {
        return false;
    }
    return id == other.id;
}

information::information() {
    for (int i = 0; i < 100; i++) {
        prices[i] = 0;
    }
}
information::information(std::string num, std::string type_, std::string stations_,
                         std::string sale_date, std::string price) {
    station_num = std::stoi(num);
    type = type_[0];
    std::string begin, end;
    for (int i = 0; i < 5; i++) {
        begin += sale_date[i];
    }
    for (int i = 6; i < 11; i++) {
        end += sale_date[i];
    }
    date begin_(begin), end_(end);
    sale_date_begin = begin_;
    sale_date_end = end_;
    std::string temp;
    int flag = 0;
    for (int i = 0; i < stations_.size(); i++) {
        if (stations_[i] != '|') {
            temp += stations_[i];
        } else {
            for (int j = 0; j < temp.size(); j++) {
                stations[flag][j] = temp[j];
            }
            flag++;
            temp.clear();
        }
    }
    for (int j = 0; j < temp.size(); j++) {
        stations[flag][j] = temp[j];
    }
    std::string temp_;
    sjtu::vector<int> prices_;
    for (int i = 0; i < price.size(); i++) {
        if (price[i] != '|') {
            temp_ += price[i];
        } else {
            prices_.push_back(std::stoi(temp_));
            temp_.clear();
        }
    }
    prices_.push_back(std::stoi(temp_));
    for (int i = 1; i <= prices_.size(); i++) {
        prices[i] = prices[i - 1] + prices_[i - 1];
    }
}
information::information(const information& other) {
    station_num = other.station_num;
    type = other.type;
    sale_date_begin = other.sale_date_begin;
    sale_date_end = other.sale_date_end;
    for (int i = 0; i < 100; i++) {
        prices[i] = other.prices[i];
    }
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 30; j++) {
            stations[i][j] = other.stations[i][j];
        }
    }
}
information& information::operator=(const information& other) {
    station_num = other.station_num;
    type = other.type;
    sale_date_begin = other.sale_date_begin;
    sale_date_end = other.sale_date_end;
    for (int i = 0; i < 100; i++) {
        prices[i] = other.prices[i];
    }
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 30; j++) {
            stations[i][j] = other.stations[i][j];
        }
    }
    return *this;
}
sjtu::vector<std::string> information::get_stations() {
    sjtu::vector<std::string> v;
    for (int i = 0; i < station_num; i++) {
        std::string temp(stations[i]);
        v.push_back(temp);
    }
    return v;
}

train_information::train_information() {
    released = 0;
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 92; j++) {
            seat_num[i][j] = 0;
        }
    }
}
train_information::train_information(std::string str1, std::string str2, std::string str3, std::string str4) {
    int num = std::stoi(str1);
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 92; j++) {
            seat_num[i][j] = num;
        }
    }
    Time start(str2);
    start_time = start;
    sjtu::vector<int> arrive, leave;
    std::string temp;
    for (int i = 0; i < str3.size(); i++) {
        if (str3[i] != '|') {
            temp += str3[i];
        } else {
            arrive.push_back(std::stoi(temp));
            temp.clear();
        }
    }
    arrive.push_back(std::stoi(temp));
    temp.clear();
    if (arrive.size() > 1) {
        for (int i = 0; i < str4.size(); i++) {
            if (str4[i] != '|') {
                temp += str4[i];
            } else {
                leave.push_back(std::stoi(temp));
                temp.clear();
            }
        }
        leave.push_back(std::stoi(temp));
    }
    arriving_time[0] = start;
    leaving_time[0] = start;
    for (int i = 1; i < arrive.size(); i++) {
        start.add_minute(arrive[i - 1]);
        arriving_time[i] = start;
        start.add_minute(leave[i - 1]);
        leaving_time[i] = start;
    }
    start.add_minute(arrive[arrive.size() - 1]);
    arriving_time[arrive.size()] = start;
}
train_information::train_information(const train_information& other) {
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 92; j++) {
            seat_num[i][j] = other.seat_num[i][j];
        }
        arriving_time[i] = other.arriving_time[i];
        leaving_time[i] = other.leaving_time[i];
    }
    start_time = other.start_time;
    released = other.released;
}
train_information& train_information::operator=(const train_information& other) {
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 92; j++) {
            seat_num[i][j] = other.seat_num[i][j];
        }
        arriving_time[i] = other.arriving_time[i];
        leaving_time[i] = other.leaving_time[i];
    }
    start_time = other.start_time;
    released = other.released;
    return *this;
}

train_management::train_management() {
    basic_information.set_file_name("train_information_1");
    advanced_information.set_file_name("train_information_2");
    released_station_train_id_list.set_file_name("train_released_list");
    station_name.set_file_name("station_names");
    num = station_name.size() + 1;
}
void train_management::add_train(const token_scanner& ts) {
    std::string trainID, station_num_, seat_num_, stations_, prices_, start_time_,
                travel_times_, stop_over_times_, sale_date_, type_;
    for (int i = 0; i < ts.key_argument.size(); i++) {
        if (ts.key_argument[i].first == 'i') {
            trainID= ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'n') {
            station_num_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'm') {
            seat_num_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 's') {
            stations_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'p') {
            prices_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'x') {
            start_time_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 't') {
            travel_times_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'o') {
            stop_over_times_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'd') {
            sale_date_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'y') {
            type_ = ts.key_argument[i].second;
        }
    }
    information info(station_num_, type_, stations_, sale_date_, prices_);
    train_information info_(seat_num_, start_time_, travel_times_, stop_over_times_);
    train_id id(trainID);
    sjtu::vector<std::pair<train_id, information> > v = basic_information.find(id, id);
    if (!v.empty()) {
        std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
        return;
    }
    basic_information.insert(id, info);
    advanced_information.insert(id, info_);
    std::cout << '[' << ts.time << ']' << ' ' << 0 << std::endl;
    return;
}
void train_management::delete_train(const token_scanner& ts) {
    std::string ID = ts.key_argument[0].second;
    train_id id(ID);
    sjtu::vector<std::pair<train_id, train_information> > v = advanced_information.find(id, id);
    if (v.empty() || v[0].second.released == 1) {
        std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
        return;
    }
    basic_information.remove(id);
    advanced_information.remove(id);
    std::cout << '[' << ts.time << ']' << ' ' << 0 << std::endl;
    return;
}
void train_management::release_train(const token_scanner& ts) {
    std::string ID = ts.key_argument[0].second;
    train_id id(ID);
    sjtu::vector<std::pair<train_id, train_information> > v = advanced_information.find(id, id);
    sjtu::vector<std::pair<train_id, information> > v_ = basic_information.find(id, id);
    if (v.empty() || v[0].second.released == 1) {
        std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
        return;
    }
    information info = v_[0].second;
    train_information info_ = v[0].second;
    advanced_information.remove(id);
    info_.released = 1;
    advanced_information.insert(id, info_);
    sjtu::vector<std::string> station_info = info.get_stations();
    for (int i = 0; i < station_info.size(); i++) {
        bool flag = station_name.insert(num, station(station_info[i], ID, Time(), Time()));
        num += flag;
        station station_(station_info[i], ID, info_.arriving_time[i], info_.leaving_time[i]);
        released_station_train_id_list.insert(station_, std::make_pair(i, info.prices[i]));
    }
    std::cout << '[' << ts.time << ']' << ' ' << 0 << std::endl;
    return;
}
void train_management::query_train(const token_scanner& ts) {
    std::string ID, date_;
    for (int i = 0; i < ts.key_argument.size(); i++) {
        if (ts.key_argument[i].first == 'i') {
            ID= ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'd') {
            date_ = ts.key_argument[i].second;
        }
    }
    train_id id(ID);
    date date_of_train(date_);
    sjtu::vector<std::pair<train_id, train_information> > v = advanced_information.find(id, id);
    sjtu::vector<std::pair<train_id, information> > v_ = basic_information.find(id, id);
    if (v.empty()) {
        std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
        return;
    }
    information info = v_[0].second;
    train_information info_ = v[0].second;
    if (info.sale_date_begin > date_of_train || info.sale_date_end < date_of_train) {
        std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
        return;
    }
    sjtu::vector<std::string> stations_info = info.get_stations();
    std::cout << '[' << ts.time << ']' << ' ' << ID << ' ' << info.type << std::endl;
    for (int i = 0; i < info.station_num; i++) {
        std::cout << stations_info[i] << ' ';
        if (i == 0) {
            std::cout << "xx-xx xx:xx -> ";
        } else {
            Time t = info_.arriving_time[i];
            date day_ = date_of_train;
            day_.add_day(t.day);
            std::cout << day_.to_string() << ' ' << t.to_string() << " -> ";
        }
        if (i == info.station_num - 1) {
            std::cout << "xx-xx xx:xx ";
        } else {
            Time t = info_.leaving_time[i];
            date day_ = date_of_train;
            day_.add_day(t.day);
            std::cout << day_.to_string() << ' ' << t.to_string() << ' ';
        }
        std::cout << info.prices[i] << ' ';
        int delta = date_of_train.delta_day();
        if (i == info.station_num - 1) {
            std::cout << 'x' << std::endl;
        } else {
            std::cout << info_.seat_num[i][delta] << std::endl;
        }
    }
}

bool compare_time(const temp &a, const temp &b) {
    if (a.time_ != b.time_) {
        return a.time_ < b.time_;
    }
    return a.begin.id < b.begin.id;
}
bool compare_price(const temp &a, const temp &b) {
    if (a.price != b.price) {
        return a.price < b.price;
    }
    return a.begin.id < b.begin.id;
}
bool compare_time_transfer(const std::pair<std::pair<temp, temp>, int>& a, const std::pair<std::pair<temp, temp>, int>& b) {
    int a_time = 1440 * a.second + a.first.first.time_ + a.first.second.time_ + (a.first.second.begin.time_leave.hour - a.first.first.end.time_arrival.hour) * 60 
                 + (a.first.second.begin.time_leave.minute - a.first.first.end.time_arrival.minute); 
    int b_time = 1440 * b.second + b.first.first.time_ + b.first.second.time_ + (b.first.second.begin.time_leave.hour - b.first.first.end.time_arrival.hour) * 60 
                 + (b.first.second.begin.time_leave.minute - b.first.first.end.time_arrival.minute); 
    int a_price = a.first.first.price + a.first.second.price;
    int b_price = b.first.first.price + b.first.second.price;
    if (a_time != b_time) {
        return a_time < b_time;
    }
    if (a_price != b_price) {
        return a_price < b_price;
    }
    if (!(a.first.first.begin.id == b.first.first.begin.id)) {
        return a.first.first.begin.id < b.first.first.begin.id;
    }
    return a.first.second.begin.id < b.first.second.begin.id;
}
bool compare_price_transfer(const std::pair<std::pair<temp, temp>, int>& a, const std::pair<std::pair<temp, temp>, int> b) {
    int a_time = 1440 * a.second + a.first.first.time_ + a.first.second.time_ + (a.first.second.begin.time_leave.hour - a.first.first.end.time_arrival.hour) * 60 
                 + (a.first.second.begin.time_leave.minute - a.first.first.end.time_arrival.minute); 
    int b_time = 1440 * b.second + b.first.first.time_ + b.first.second.time_ + (b.first.second.begin.time_leave.hour - b.first.first.end.time_arrival.hour) * 60 
                 + (b.first.second.begin.time_leave.minute - b.first.first.end.time_arrival.minute); 
    int a_price = a.first.first.price + a.first.second.price;
    int b_price = b.first.first.price + b.first.second.price;
    if (a_price != b_price) {
        return a_price < b_price;
    }
    if (a_time != b_time) {
        return a_time < b_time;
    }
    if (!(a.first.first.begin.id == b.first.first.begin.id)) {
        return a.first.first.begin.id < b.first.first.begin.id;
    }
    return a.first.second.begin.id < b.first.second.begin.id;
}
template <typename Compare, class T>
void quick_sort(sjtu::vector<T> &v, int left, int right, Compare cmp) {
    if (left >= right) return;
    T pivot = v[left];
    int i = left - 1, j = right + 1;
    while (true) {
        do { i++; } while (cmp(v[i],pivot));
        do { j--; } while (cmp(pivot,v[j]));
        if (i >= j) break;
        std::swap(v[i], v[j]);
    }
    quick_sort(v, left, j,cmp);
    quick_sort(v, j + 1, right,cmp);
}
void sort_by_time(sjtu::vector<temp> &v) {
    quick_sort(v, 0, v.size() - 1, compare_time);
}
void sort_by_price(sjtu::vector<temp> &v) {
    quick_sort(v, 0, v.size() - 1, compare_price);
}
void sort_by_time_transfer(sjtu::vector<std::pair<std::pair<temp, temp>, int> > &v) {
    quick_sort(v, 0, v.size() - 1, compare_time_transfer);
}
void sort_by_price_transfer(sjtu::vector<std::pair<std::pair<temp, temp>, int> > &v) {
    quick_sort(v, 0, v.size() - 1, compare_price_transfer);
}

sjtu::vector<temp> train_management::query_ticket_(std::string start, std::string end, date date_) {
    std::string min_id, max_id;
    for (int i = 0; i < 20; i++) {
        min_id += '\0';
        max_id += '~';
    }
    Time t;
    station minimal_start(start, min_id, t, t), maximal_start(start, max_id, t, t);
    station minimal_end(end, min_id, t, t), maximal_end(end, max_id, t, t);
    sjtu::vector<std::pair<station, std::pair<int, int> > > start_train = released_station_train_id_list.find(minimal_start, maximal_start);
    sjtu::vector<std::pair<station, std::pair<int, int> > > end_train = released_station_train_id_list.find(minimal_end, maximal_end);
    sjtu::vector<temp> train_satisfied;
    for (int i = 0; i < start_train.size(); i++) {
        station train = start_train[i].first;
        for (int j = 0; j < end_train.size(); j++) {
            station train_end = end_train[j].first;
            if (train_end.id == train.id) {
                if (train_end.time_arrival > train.time_arrival) {
                    date d(date_);
                    d.minus_day(train.time_leave.day);
                    sjtu::vector<std::pair<train_id, information> > v_ = basic_information.find(train.id, train.id);
                    information info = v_[0].second;
                    if (info.sale_date_begin > d || info.sale_date_end < d) {
                        break;
                    }
                    int price = end_train[j].second.second - start_train[i].second.second;
                    int t = train_end.time_arrival - train.time_leave;
                    int index_begin = start_train[i].second.first;
                    int index_end = end_train[j].second.first;
                    train_satisfied.push_back({train, train_end, t, price, index_begin, index_end});
                }
                break;
            }
        }
    }
    return train_satisfied;
}
sjtu::vector<std::pair<temp, int> > train_management::query_ticket__(std::string start, std::string end, date date_, Time time_) {
    std::string min_id, max_id;
    for (int i = 0; i < 20; i++) {
        min_id += '\0';
        max_id += '~';
    }
    Time t;
    station minimal_start(start, min_id, t, t), maximal_start(start, max_id, t, t);
    station minimal_end(end, min_id, t, t), maximal_end(end, max_id, t, t);
    sjtu::vector<std::pair<station, std::pair<int, int> > > start_train = released_station_train_id_list.find(minimal_start, maximal_start);
    sjtu::vector<std::pair<station, std::pair<int, int> > > end_train = released_station_train_id_list.find(minimal_end, maximal_end);
    sjtu::vector<std::pair<temp, int> > train_satisfied;
    for (int i = 0; i < start_train.size(); i++) {
        station train = start_train[i].first;
        for (int j = 0; j < end_train.size(); j++) {
            station train_end = end_train[j].first;
            if (train_end.id == train.id) {
                if (train_end.time_arrival > train.time_arrival) {
                    date d(date_);
                    d.minus_day(train.time_leave.day);
                    sjtu::vector<std::pair<train_id, information> > v_ = basic_information.find(train.id, train.id);
                    information info = v_[0].second;
                    if (info.sale_date_end < d || (info.sale_date_end == d && (train_end.time_leave.hour > time_.hour || (train_end.time_leave.hour == time_.hour && train_end.time_leave.minute > time_.minute)))) {
                        break;
                    }
                    int price = end_train[j].second.second - start_train[i].second.second;
                    int t = train_end.time_arrival - train.time_leave;
                    int index_begin = start_train[i].second.first;
                    int index_end = end_train[j].second.first;
                    int day_;
                    if (info.sale_date_begin > d) {
                        day_ = info.sale_date_begin.delta_day() - d.delta_day();
                    } else {
                        if ((train_end.time_leave.hour < time_.hour || (train_end.time_leave.hour == time_.hour && train_end.time_leave.minute < time_.minute))) {
                            day_ = 1;
                        }
                    }
                    train_satisfied.push_back({{train, train_end, t, price, index_begin, index_end}, day_});
                }
                break;
            }
        }
    }
    return train_satisfied;
}
void train_management::query_ticket(const token_scanner& ts) {
    std::string start, end, date_, sort_;
    for (int i = 0; i < ts.key_argument.size(); i++) {
        if (ts.key_argument[i].first == 's') {
            start= ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'd') {
            date_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 't') {
            end = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'p') {
            sort_ = ts.key_argument[i].second;
        }
    }
    date d(date_);
    sjtu::vector<temp> train_satisfied = query_ticket_(start, end, d);
    if (train_satisfied.empty()) {
        std::cout << '[' << ts.time << ']' << ' ' << 0 << std::endl;
        return;
    }
    if (sort_ == "cost") {
        sort_by_price(train_satisfied);
    } else if (sort_ == "time") {
        sort_by_time(train_satisfied);
    }
    std::cout << '[' << ts.time << ']' << ' ' << train_satisfied.size() << std::endl;
    for (int i = 0; i < train_satisfied.size(); i++) {
        std::cout << train_satisfied[i].begin.id.id << ' ' << train_satisfied[i].begin.station_name << ' ';
        date day(date_);
        date day_ = day;
        day_.minus_day(train_satisfied[i].begin.time_leave.day);
        std::cout << day.to_string() << ' ' << train_satisfied[i].begin.time_leave.to_string() 
                  << " -> " << train_satisfied[i].end.station_name << ' ';
        day.add_day(train_satisfied[i].end.time_arrival.day - train_satisfied[i].begin.time_leave.day);
        std::cout << day.to_string() << ' ' << train_satisfied[i].end.time_arrival.to_string() << ' ' << train_satisfied[i].price << ' ';
        sjtu::vector<std::pair<train_id, train_information> > v = advanced_information.find(train_satisfied[i].begin.id, train_satisfied[i].begin.id);
        train_information info_ = v[0].second;
        int seat_ = 1e9;
        int delta = day_.delta_day();
        for (int j = train_satisfied[i].index_begin; j < train_satisfied[i].index_end; j++) {
            seat_ = std::min(seat_, info_.seat_num[j][delta]);
        }
        std::cout << seat_ << std::endl;
    }
}
void train_management::query_transfer(const token_scanner& ts) {
    std::string start, end, date_, sort_;
    for (int i = 0; i < ts.key_argument.size(); i++) {
        if (ts.key_argument[i].first == 's') {
            start= ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'd') {
            date_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 't') {
            end = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'p') {
            sort_ = ts.key_argument[i].second;
        }
    }
    date d(date_);
    sjtu::vector<std::pair<std::pair<temp, temp>, int> > train_satisfied;
    for (int i = 1; i <= num; i++) {
        sjtu::vector<std::pair<int, station> > stations = station_name.find(i, i);
        if (stations.empty()) {
            continue;
        }
        std::string station_(stations[0].second.station_name);
        if (station_ == start || station_ == end) {
            continue;
        }
        sjtu::vector<temp> train_satisfied_first = query_ticket_(start, station_, d);
        for (int j = 0; j < train_satisfied_first.size(); j++) {
            temp train_ = train_satisfied_first[j];
            date arrive_transfer_date(date_);
            arrive_transfer_date.add_day(train_.end.time_arrival.day - train_.begin.time_leave.day);
            sjtu::vector<std::pair<temp, int> > train_satisfied_second = query_ticket__(station_, end, arrive_transfer_date, train_.end.time_arrival);
            for (int k = 0; k < train_satisfied_second.size(); k++) {
                if (train_.begin.id == train_satisfied_second[k].first.begin.id) {
                    continue;
                }
                train_satisfied.push_back({{train_, train_satisfied_second[k].first}, train_satisfied_second[k].second});
            }
        }
    }
    if (train_satisfied.size() == 0) {
        std::cout << '[' << ts.time << ']' << ' ' << 0 << std::endl;
        return;
    }
    if (sort_ == "time") {
        sort_by_time_transfer(train_satisfied);
    } else if (sort_ == "cost") {
        sort_by_price_transfer(train_satisfied);
    }
    std::cout << '[' << ts.time << "] ";
    std::cout << train_satisfied[0].first.first.begin.id.id << ' ' << train_satisfied[0].first.first.begin.station_name << ' ';
    date day(date_);
    date day_(date_);
    day_.minus_day(train_satisfied[0].first.first.begin.time_leave.day);
    std::cout << day.to_string() << ' ' << train_satisfied[0].first.first.begin.time_leave.to_string() 
                << " -> " << train_satisfied[0].first.first.end.station_name << ' ';
    day.add_day(train_satisfied[0].first.first.end.time_arrival.day - train_satisfied[0].first.first.begin.time_arrival.day);
    std::cout << day.to_string() << ' ' << train_satisfied[0].first.first.end.time_arrival.to_string() << ' ' << train_satisfied[0].first.first.price << ' ';
    sjtu::vector<std::pair<train_id, train_information> > v = advanced_information.find(train_satisfied[0].first.first.begin.id, train_satisfied[0].first.first.begin.id);
    train_information info_ = v[0].second;
    int seat_ = 1e9;
    int delta = day_.delta_day();
    for (int j = train_satisfied[0].first.first.index_begin; j < train_satisfied[0].first.first.index_end; j++) {
        seat_ = std::min(seat_, info_.seat_num[j][delta]);
    }
    std::cout << seat_ << std::endl;
    std::cout << train_satisfied[0].first.second.begin.id.id << ' ' << train_satisfied[0].first.second.begin.station_name << ' ';
    day.add_day(train_satisfied[0].second);
    day_ = day;
    day_.minus_day(train_satisfied[0].first.second.begin.time_leave.day);
    std::cout << day.to_string() << ' ' << train_satisfied[0].first.second.begin.time_leave.to_string() 
                << " -> " << train_satisfied[0].first.second.end.station_name << ' ';
    day.add_day(train_satisfied[0].first.second.end.time_arrival.day - train_satisfied[0].first.second.begin.time_arrival.day);
    std::cout << day.to_string() << ' ' << train_satisfied[0].first.second.end.time_arrival.to_string() << ' ' << train_satisfied[0].first.second.price << ' ';
    sjtu::vector<std::pair<train_id, train_information> > v_ = advanced_information.find(train_satisfied[0].first.second.begin.id, train_satisfied[0].first.second.begin.id);
    train_information info__ = v_[0].second;
    seat_ = 1e9;
    delta = day_.delta_day();
    for (int j = train_satisfied[0].first.second.index_begin; j < train_satisfied[0].first.second.index_end; j++) {
        seat_ = std::min(seat_, info__.seat_num[j][delta]);
    }
    std::cout << seat_ << std::endl;
    return;
}
std::pair<date, int> train_management::query_buy(std::string start, std::string end, date date_, train_id id, int num, bool flag) {
    Time t;
    station start_(start, id, t, t);
    station end_(end, id, t, t);
    sjtu::vector<std::pair<station, std::pair<int, int> > > start_train = released_station_train_id_list.find(start_, start_);
    sjtu::vector<std::pair<station, std::pair<int, int> > > end_train = released_station_train_id_list.find(end_, end_);
    if (start_train.empty() || end_train.empty()) {
        return {date(), -1};
    }
    sjtu::vector<temp> train_satisfied;
    station train_start = start_train[0].first;
    station train_end = end_train[0].first;
    if (start_train[0].second.first >= end_train[0].second.first) {
        return {date(), -1};
    }
    if (flag) date_.minus_day(train_start.time_leave.day);
    sjtu::vector<std::pair<train_id, information> > v_ = basic_information.find(id, id);
    information info = v_[0].second;
    if (info.sale_date_begin > date_ || info.sale_date_end < date_) {
        return {date(), -1};
    }
    int delta_date = date_.delta_day();
    sjtu::vector<std::pair<train_id, train_information> > find_result = advanced_information.find(id, id);
    int minimal = 1e9;
    for (int i = start_train[0].second.first; i < end_train[0].second.first; i++) {
        minimal = std::min(minimal, find_result[0].second.seat_num[i][delta_date]);
    }
    if (minimal >= num){
        for (int i = start_train[0].second.first; i < end_train[0].second.first; i++) {
            find_result[0].second.seat_num[i][delta_date] -= num;
        }
        advanced_information.remove(id);
        advanced_information.insert(id, find_result[0].second);
        int price_ = end_train[0].second.second - start_train[0].second.second;
        return {date_, price_ * num};
    } else {
        return {date_, 0};
    }
}
std::pair<Time, Time> train_management::query_time(const train_id& train, std::string station_) {
    sjtu::vector<std::pair<train_id, information> > result_ = basic_information.find(train, train);
    sjtu::vector<std::pair<train_id, train_information> > result = advanced_information.find(train, train);
    sjtu::vector<std::string> stations = result_[0].second.get_stations();
    int index;
    for (index = 0; index < stations.size(); index++) {
        if (stations[index] == station_) {
            break;
        }
    }
    Time time_arrive = result[0].second.arriving_time[index];
    Time time_leave = result[0].second.leaving_time[index];
    return {time_arrive, time_leave};
}
int train_management::query_price(const train_id& train, std::string begin, std::string end) {
    sjtu::vector<std::pair<train_id, information> > result_ = basic_information.find(train, train);
    sjtu::vector<std::pair<train_id, train_information> > result = advanced_information.find(train, train);
    sjtu::vector<std::string> stations = result_[0].second.get_stations();
    int index, index_;
    for (int i = 0; i < stations.size(); i++) {
        if (stations[i] == begin) {
            index = i;
        }
        if (stations[i] == end) {
            index_ = i;
        }
    }
    return result_[0].second.prices[index_] - result_[0].second.prices[index];
}
void train_management::refund_ticket(const train_id& train, std::string begin, std::string end, date day, int num) {
    sjtu::vector<std::pair<train_id, information> > result_ = basic_information.find(train, train);
    sjtu::vector<std::pair<train_id, train_information> > result = advanced_information.find(train, train);
    sjtu::vector<std::string> stations = result_[0].second.get_stations();
    int index, index_;
    train_information info = result[0].second;
    for (int i = 0; i < stations.size(); i++) {
        if (stations[i] == begin) {
            index = i;
        }
        if (stations[i] == end) {
            index_ = i;
        }
    }
    day.minus_day(info.leaving_time[index].day);
    int delta = day.delta_day();
    for (int i = index; i < index_; i++) {
        info.seat_num[i][delta] += num;
    }
    advanced_information.remove(train);
    advanced_information.insert(train, info);
}
void train_management::exit() {
    basic_information.~B_plus_tree();
    advanced_information.~B_plus_tree();
    released_station_train_id_list.~B_plus_tree();
    station_name.~B_plus_tree();
}
void train_management::clear() {
    basic_information.clear();
    advanced_information.clear();
    released_station_train_id_list.clear();
    station_name.clear();
}