cmake_minimum_required(VERSION 3.26)
project(test3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(test3 main.cpp
        Equation.cpp
        Equation.h
        Repo.cpp
        Repo.h
        Service.cpp
        Service.h
        gui.cpp
        gui.h
        gui.ui)
target_link_libraries(test3
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

