// Created by GranthyGu on 2025/5/16

#include "time_calculator.hpp"

Time::Time() {
    hour = minute = 0;
}
Time::Time(int h, int m) {
    hour = h; 
    minute = m;
}
Time::Time(std::string str) {
    std::string h, m;
    h += str[0];
    h += str[1];
    m += str[3];
    m += str[4];
    hour = std::stoi(h);
    minute = std::stoi(m);
}
Time Time::operator=(const Time& other) {
    day = other.day;
    hour = other.hour;
    minute = other.minute;
    return *this;
}
bool Time::operator<(const Time& other) {
    if (day != other.day) {
        return day < other.day;
    }
    if (hour != other.hour) {
        return hour < other.hour;
    }
    return minute < other.minute;
}
bool Time::operator>(const Time& other) {
    if (day != other.day) {
        return day > other.day;
    }
    if (hour != other.hour) {
        return hour > other.hour;
    }
    return minute > other.minute;
}
void Time::add_minute(int m) {
    int h = (minute + m) / 60;
    int mm = (minute + m) % 60;
    minute = mm;
    int day_ = (hour + h) / 24;
    int hh = (hour + h) % 24;
    hour = hh;
    day = day_;
}

date::date() {
    month = 0;
    day = 0;
}
date::date(int m, int d) {
    month = m;
    day = d;
}
date::date(std::string str) {
    std::string h, m;
    h += str[0];
    h += str[1];
    m += str[3];
    m += str[4];
    month = std::stoi(h);
    day = std::stoi(m);
}
date date::operator=(const date& other) {
    month = other.month;
    day = other.day;
    return *this;
}
bool date::operator<(const date& other) {
    if (month != other.month) {
        return month < other.month;
    }
    return day < other.day;
}
bool date::operator>(const date& other) {
    if (month != other.month) {
        return month > other.month;
    }
    return day > other.day;
}
bool date::operator!=(const date& other) {
    return (month != other.month) || (day != other.day);
}
void date::add_day(int d) {
    int cur_day = day + d;
    if (month == 6) {
        month += (cur_day - 1) / 30;
        day = (cur_day - 1) % 30 + 1;
    } else {
        month += (cur_day - 1) / 31;
        day = (cur_day - 1) % 31 + 1;
    }
}

Time_calculator::Time_calculator() {}
Time_calculator::Time_calculator(std::string str1, std::string str2) {
    date d(str1);
    Time t(str2);
    date_ = d;
    time_ = t;
}
Time_calculator::Time_calculator(date d, Time t) {
    date_ = d;
    time_ = t;
}
Time_calculator Time_calculator::operator=(const Time_calculator& other) {
    date_ = other.date_;
    time_ = other.time_;
    return *this;
}
bool Time_calculator::operator<(const Time_calculator& other) {
    if (date_ != other.date_) {
        return date_ < other.date_;
    }
    return time_ < other.time_;
}
bool Time_calculator::operator>(const Time_calculator& other) {
    if (date_ != other.date_) {
        return date_ > other.date_;
    }
    return time_ > other.time_;
}
Time_calculator Time_calculator::add_time(int m) {
    Time_calculator new_calculator = *this;
    new_calculator.time_.add_minute(m);
    new_calculator.date_.add_day(new_calculator.time_.day);
    new_calculator.time_.day = 0;
    return new_calculator;
}