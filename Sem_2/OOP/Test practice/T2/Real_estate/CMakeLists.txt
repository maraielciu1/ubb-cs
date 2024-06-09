cmake_minimum_required(VERSION 3.26)
project(real_estate)

set(CMAKE_CXX_STANDARD 23)

add_executable(real_estate main.cpp
        Client.cpp
        Client.h
        Dwelling.h
        RealEstateAgency.cpp
        RealEstateAgency.h
        UI.cpp
        UI.h
)
