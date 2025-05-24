cmake_minimum_required(VERSION 3.16)
project(code)

set(CMAKE_CXX_STANDARD 17)

add_executable(code
        src/account_management.cpp
        src/ticket_management.cpp
        src/train_management.cpp
        src/main.cpp
        src/token_scanner.cpp
        src/time_calculator.cpp

)