cmake_minimum_required(VERSION 3.26)
project(buildings)

set(CMAKE_CXX_STANDARD 23)

add_executable(buildings main.cpp
        Building.cpp
        Building.h
        Service.cpp
        Service.h
        UI.cpp
        UI.h)
