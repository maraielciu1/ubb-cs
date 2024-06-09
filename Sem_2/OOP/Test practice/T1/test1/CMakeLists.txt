cmake_minimum_required(VERSION 3.26)
project(t1_maraielciu1_1)

set(CMAKE_CXX_STANDARD 14)

add_executable(t1_maraielciu1_1
        main.cpp
        Domain/DynamicArray.h
        Repo/Repo.cpp
        Repo/Repo.h
        Service/Service.cpp
        Service/Service.h
        UI/UI.cpp
        UI/UI.h
        Tests/Test.cpp
        Tests/Test.h
        Domain/Player.cpp
        Domain/Player.h)
