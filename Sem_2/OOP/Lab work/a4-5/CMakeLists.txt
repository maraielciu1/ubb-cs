cmake_minimum_required(VERSION 3.26)
project(oop_a4_5_maraielciu1)

set(CMAKE_CXX_STANDARD 14)

include_directories(Domain)

add_executable(oop_a4_5_maraielciu1
        Domain/Tutorial.cpp
        Domain/Tutorial.h
        Repository/Repo.h
        Tests/TestTutorial.cpp
        Tests/TestTutorial.h
        main.cpp
        Repository/Repo.cpp
        Domain/DynamicArray.cpp
        Domain/DynamicArray.h
        Tests/TestDynamicArray.cpp
        Tests/TestDynamicArray.h
        Tests/TestRepo.cpp
        Tests/TestRepo.h
        Service/Service.cpp
        Service/Service.h
        Tests/TestService.cpp
        Tests/TestService.h
        UI/Ui.cpp
        UI/Ui.h)
