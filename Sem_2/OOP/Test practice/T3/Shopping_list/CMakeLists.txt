cmake_minimum_required(VERSION 3.26)
project(shopping_list)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(shopping_list main.cpp
        gui.cpp
        gui.h
        gui.ui
        Item.cpp
        Item.h
        Repo.cpp
        Repo.h
        Service.cpp
        Service.h)
target_link_libraries(shopping_list
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

