cmake_minimum_required(VERSION 3.26)
project(seminar7)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(seminar7 main.cpp
        Subject.cpp
        Subject.h
        observer.cpp
        observer.h
        ChatSesion.cpp
        ChatSesion.h
        chatwindow.cpp
        chatwindow.h
        chatwindow.ui
)
target_link_libraries(seminar7
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

