cmake_minimum_required(VERSION 3.26)
project(SortedIndexedList)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(SortedIndexedList
        App.cpp
        ExtendedTest.cpp
        ExtendedTest.h
        ListIterator.cpp
        ListIterator.h
        ShortTest.cpp
        ShortTest.h
        SortedIndexedList.h
        SortedIndexeList.cpp)
