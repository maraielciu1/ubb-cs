cmake_minimum_required(VERSION 3.26)
project(oop_a8_9_maraielciu1)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

qt6_wrap_ui(UI_HEADERS MainWindow.ui)
include_directories(${CMAKE_BINARY_DIR})  # To find the generated headers

include_directories(Domain)
include_directories(Repository)
include_directories(Service)
include_directories(UI)

add_executable(oop_a8_9_maraielciu1
        Domain/Tutorial.cpp
        Domain/Tutorial.h
        Domain/TutorialValidator.cpp
        Domain/TutorialValidator.h
        Domain/Utils.cpp
        Domain/Utils.h
        Repository/CSVWatchList.cpp
        Repository/CSVWatchList.h
        Repository/Exceptions.cpp
        Repository/Exceptions.h
        Repository/FileRepo.cpp
        Repository/FileRepo.h
        Repository/FileWatchList.cpp
        Repository/FileWatchList.h
        Repository/HTMLWatchList.cpp
        Repository/HTMLWatchList.h
        Repository/Repo.cpp
        Repository/Repo.h
        Repository/WatchList.cpp
        Repository/WatchList.h
        Service/Service.cpp
        Service/Service.h
        UI/Ui.cpp
        UI/Ui.h
        main.cpp
        GUI/AdminWindow.cpp
        GUI/AdminWindow.h
        GUI/TutorialsList.cpp
        GUI/TutorialsList.h
        GUI/UserWindow.cpp
        GUI/UserWindow.h
        GUI/mainWindow.cpp
        GUI/mainWindow.h
        GUI/mainWindow.ui
        GUI/qcustomplot.cpp
        GUI/qcustomplot.h
        Service/Action.cpp
        Service/Action.h
        Service/Action.h
        GUI/WatchListModel.cpp
        GUI/WatchListModel.h
        GUI/WatchListWindow.cpp
        GUI/WatchListWindow.h
)

target_link_libraries(oop_a8_9_maraielciu1 Qt::Core)
target_link_libraries(oop_a8_9_maraielciu1 Qt::Widgets)
target_link_libraries(oop_a8_9_maraielciu1 Qt::Gui)
