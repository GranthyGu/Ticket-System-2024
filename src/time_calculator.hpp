// Created by GranthyGu on 2025/5/16

#ifndef TIME_CALCULATOR_HPP
#define TIME CALCULATOR_HPP

#include <iostream>
#include <string>

class Time {
public:
    int hour;
    int minute;
    int day = 0;
    Time();
    Time(int, int);
    Time(std::string);
    Time operator=(const Time&);
    bool operator<(const Time&);
    bool operator>(const Time&);
    void add_minute(int);
};
class date {
public:
    int month;
    int day;
    date();
    date(int, int);
    date(std::string);
    date operator=(const date&);
    bool operator<(const date&);
    bool operator>(const date&);
    bool operator!=(const date&);
    void add_day(int);
};

class Time_calculator {
public:
    date date_;
    Time time_;
    Time_calculator();
    Time_calculator(std::string, std::string);
    Time_calculator(date, Time);
    Time_calculator operator=(const Time_calculator&);
    bool operator<(const Time_calculator&);
    bool operator>(const Time_calculator&);
    Time_calculator add_time(int);
};

#endif    //TIME_CALCULATOR_HPP