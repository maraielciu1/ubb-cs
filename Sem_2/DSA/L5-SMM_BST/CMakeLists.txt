cmake_minimum_required(VERSION 3.26)
project(SortedMultiMap__1_)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(SortedMultiMap__1_
        App.cpp
        ExtendedTest.cpp
        ExtendedTest.h
        ShortTest.cpp
        ShortTest.h
        SMMIterator.cpp
        SMMIterator.h
        SortedMultiMap.cpp
        SortedMultiMap.h)
