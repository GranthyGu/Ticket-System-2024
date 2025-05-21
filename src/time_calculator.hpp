// Created by GranthyGu on 2025/5/16

#ifndef TIME_CALCULATOR_HPP
#define TIME CALCULATOR_HPP

#include <iostream>
#include <string>

class Time {
public:
    int hour = 0;
    int minute = 0;
    int day = 0;
    Time();
    Time(int, int);
    Time(std::string);
    Time& operator=(const Time&);
    bool operator<(const Time&) const;
    bool operator>(const Time&) const;
    void add_minute(int);
    std::string to_string();
    int& operator-(const Time&);
};
class date {
public:
    int month = 0;
    int day = 0;
    date();
    date(int, int);
    date(std::string);
    date& operator=(const date&);
    bool operator<(const date&) const;
    bool operator>(const date&) const;
    bool operator!=(const date&) const;
    bool operator==(const date&) const;
    void add_day(int);
    std::string to_string();
    void minus_day(int);
    int delta_day();
};

#endif    //TIME_CALCULATOR_HPP