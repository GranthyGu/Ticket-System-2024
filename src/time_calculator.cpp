// Created by GranthyGu on 2025/5/16

#include "time_calculator.hpp"

Time::Time() {
    hour = minute = 0;
    day = 0;
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
Time::Time(const Time& other) {
    day = other.day;
    hour = other.hour;
    minute = other.minute;
}
Time& Time::operator=(const Time& other) {
    day = other.day;
    hour = other.hour;
    minute = other.minute;
    return *this;
}
bool Time::operator<(const Time& other) const {
    if (day != other.day) {
        return day < other.day;
    }
    if (hour != other.hour) {
        return hour < other.hour;
    }
    return minute < other.minute;
}
bool Time::operator>(const Time& other) const {
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
    day += day_;
}
std::string Time::to_string() const {
    std::string time_;
    if (hour < 10) {
        time_ += '0';
    }
    time_ += std::to_string(hour);
    time_ += ':';
    if (minute < 10) {
        time_ += '0';
    }
    time_ += std::to_string(minute);
    return time_;
}
int Time::operator-(const Time& other) const {
    int ans = 0;
    int delta_d = day - other.day;
    ans += delta_d * 1440;
    int delta_hour = hour - other.hour;
    ans += delta_hour * 60;
    int delta_minute = minute - other.minute;
    ans += delta_minute;
    return ans;
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
date::date(const date& other) {
    month = other.month;
    day = other.day;
}
date& date::operator=(const date& other) {
    month = other.month;
    day = other.day;
    return *this;
}
bool date::operator<(const date& other) const {
    if (month != other.month) {
        return month < other.month;
    }
    return day < other.day;
}
bool date::operator>(const date& other) const {
    if (month != other.month) {
        return month > other.month;
    }
    return day > other.day;
}
bool date::operator!=(const date& other) const {
    return (month != other.month) || (day != other.day);
}
bool date::operator==(const date& other) const {
    return (month == other.month) && (day == other.day);
}
void date::add_day(int d) {
    int cur_day = day + d;
    if (month == 6) {
        month += (cur_day - 1) / 30;
        day = (cur_day - 1) % 30 + 1;
        if (month == 8) {
            day--;
            if (day == 0) {
                month = 7;
                day = 31;
            }
        }
    } else {
        month += (cur_day - 1) / 31;
        day = (cur_day - 1) % 31 + 1;
    }
}
std::string date::to_string() const {
    std::string time_;
    if (month < 10) {
        time_ += '0';
    }
    time_ += std::to_string(month);
    time_ += '-';
    if (day < 10) {
        time_ += '0';
    }
    time_ += std::to_string(day);
    return time_;
}
void date::minus_day(int d) {
    if (day > d) {
        day -= d;
    } else {
        int minus_ = d - day;
        month--;
        if (month == 6) {
            day = 30 - minus_;
        } else {
            day = 31 - minus_;
        }
    }
    return;
}
int date::delta_day() const {
    int delta_month = month - 6;
    int delta_day = day - 1;
    if (delta_month == 2) {
        return 61 + delta_day;
    } else if (delta_month == -1) {
        return day - 32;
    } else {
        return delta_month * 30 + delta_day;
    }
}