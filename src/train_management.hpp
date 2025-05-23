// Created by GranthyGu on 2025/5/10

#ifndef TRAIN_MANAGEMENT_HPP
#define TRAIN_MANAGEMENT_HPP

#include "b_plus_tree.hpp"
#include "STL/vector.hpp"
#include "STL/map.hpp"
#include "token_scanner.hpp"
#include "time_calculator.hpp"
#include <string>
#include <iostream>

class train_id {
public:
    char id[21] = {0};
    train_id();
    train_id(std::string);
    train_id(const train_id&);
    train_id& operator=(const train_id&);
    bool operator<(const train_id&) const;
    bool operator>(const train_id&) const;
    bool operator==(const train_id&) const;
};

class train_information{
public:
    int station_num = 0;
    char type = 0;
    char stations[100][30] = {0};
    date sale_date_begin;
    date sale_date_end;
    int prices[101] = {0};    // prefix_sum
    int released = 0;
    int seat_num[100][92];
    Time start_time;
    Time arriving_time[101];   // arriving_time[i] saves the time to the ith station.
    Time leaving_time[101];    // leaving_time[i] saves the time leave the ith station.
    train_information();
    train_information(std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string);
    train_information& operator=(const train_information&);
    void write_to_file(std::fstream& File, long pos);
    void read_from_file(std::fstream& File, long pos);
    sjtu::vector<std::string> get_stations();
};

class station {
public:
    char station_name[31] = {0};
    train_id id;
    Time time_arrival;
    Time time_leave;
    station();
    station(std::string, std::string, Time, Time);
    station(std::string, train_id, Time, Time);
    station(const station&);
    station& operator=(const station&);
    bool operator<(const station&) const;
    bool operator>(const station&) const;
    bool operator==(const station&) const;
};
struct temp {
    station begin;
    station end;
    int time_ = 0;
    int price = 0;
    int index_begin = -1;
    int index_end = -1;
};

class train_management {
private:
    B_plus_tree<train_id, long, 120, 100> advanced_information;
    B_plus_tree<station, std::pair<int, int>, 60, 50> released_station_train_id_list;
    sjtu::vector<temp> query_ticket_(std::string, std::string, date);
    sjtu::vector<std::pair<temp, int>> query_ticket__(std::string, std::string, date, Time time);
    std::fstream File;
public:
    train_management();
    void add_train(const token_scanner&);
    void delete_train(const token_scanner&);
    void release_train(const token_scanner&);
    void query_train(const token_scanner&);
    void query_ticket(const token_scanner&);
    void query_transfer(const token_scanner&);
    void refund_ticket(const train_id&, std::string, std::string, date, int);
    std::pair<date, int> query_buy(std::string, std::string, date, train_id, int, bool);
    std::pair<Time, Time> query_time(const train_id&, std::string);
    int query_price(const train_id&, std::string, std::string);
    void exit();
    void clear();
};

#endif    //TRAIN_MANAGEMENT_HPP