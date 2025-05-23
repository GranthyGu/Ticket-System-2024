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
    Time(const Time&);
    Time& operator=(const Time&);
    bool operator<(const Time&) const;
    bool operator>(const Time&) const;
    void add_minute(int);
    std::string to_string() const;
    int operator-(const Time&) const;
};
class date {
public:
    int month = 0;
    int day = 0;
    date();
    date(int, int);
    date(std::string);
    date(const date&);
    date& operator=(const date&);
    bool operator<(const date&) const;
    bool operator>(const date&) const;
    bool operator!=(const date&) const;
    bool operator==(const date&) const;
    void add_day(int);
    std::string to_string() const;
    void minus_day(int);
    int delta_day() const;
};

#endif    //TIME_CALCULATOR_HPP