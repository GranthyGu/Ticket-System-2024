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
    char id[20];
    train_id();
    train_id(std::string);
    train_id operator=(const train_id&);
    bool operator<(const train_id&);
    bool operator>(const train_id&);
    bool operator==(const train_id&);
};

class information {
public:
    int station_num;
    char type;
    char stations[3005];
    Time_calculator sale_date_begin;
    Time_calculator sale_date_end;
    int prices[100];    // prefix_sum
    information();
    information(std::string, std::string, std::string, std::string);
    information operator=(const information&);
};

class train_information{
public:
    int seat_num[100];
    Time start_time;
    Time arrive_time[100];   // arrive_time[i] saves the time to the ith station.
    Time leaving_time[100];
    bool released = false;
    train_information();
    train_information(std::string, std::string, std::string, std::string);
    train_information operator=(const train_information&);
};

class station {
public:
    char station_name[30];
    station();
    station(std::string);
    station operator=(const station&);
    bool operator<(const station&);
    bool operator>(const station&);
    bool operator==(const station&);
};

class train_management {
private:
    B_plus_tree<train_id, information, 100, 100> basic_information;
    B_plus_tree<train_id, train_information, 100, 100> advanced_information;
    B_plus_tree<station, train_id, 100, 100> released_station_train_id_list;
    sjtu::map<std::string, std::string> station_name;
public:
    train_management();
    void add_train(const token_scanner&);
    void delete_train(const token_scanner&);
    void release_train(const token_scanner&);
    void query_train(const token_scanner&);
    void query_ticket(const token_scanner&);
    void query_transfer(const token_scanner&);
};

#endif    //TRAIN_MANAGEMENT_HPP