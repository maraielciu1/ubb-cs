cmake_minimum_required(VERSION 3.26)
project(task_manager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt")


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(task_manager main.cpp
        gui.cpp
        gui.h
        gui.ui
        Task.cpp
        Task.h
        Repo.cpp
        Repo.h
        Service.cpp
        Service.h)
target_link_libraries(task_manager
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

