cmake_minimum_required(VERSION 3.26)
project(appliences)

set(CMAKE_CXX_STANDARD 23)

add_executable(appliences main.cpp
        Appliance.cpp
        Appliance.h
        Service.cpp
        Service.h
        UI.cpp
        UI.h)
