cmake_minimum_required(VERSION 3.26)
project(oop_a6_7_maraielciu1)

set(CMAKE_CXX_STANDARD 14)

include_directories(Domain)
include_directories(Repository)
include_directories(Service)
include_directories(UI)

add_executable(oop_a6_7_maraielciu1
        Domain/Tutorial.cpp
        Domain/Tutorial.h
        Repository/Repo.cpp
        Repository/Repo.h
        Service/Service.cpp
        Service/Service.h
        UI/Ui.cpp
        UI/Ui.h
        main.cpp
        Repository/FileRepo.cpp
        Repository/FileRepo.h
        Repository/Exceptions.cpp
        Repository/Exceptions.h
        Domain/Utils.cpp
        Domain/Utils.h
        Repository/WatchList.cpp
        Repository/WatchList.h
        Domain/TutorialValidator.cpp
        Domain/TutorialValidator.h
        Repository/FileWatchList.cpp
        Repository/FileWatchList.h
        Repository/CSVWatchList.cpp
        Repository/CSVWatchList.h
        Repository/HTMLWatchList.cpp
        Repository/HTMLWatchList.h
        Repository/DBRepo.cpp
        Repository/DBRepo.h
)

# Find SQLite3
find_package(SQLite3 REQUIRED)
include_directories(${SQLite3_INCLUDE_DIRS})

# Link SQLite3
target_link_libraries(oop_a6_7_maraielciu1 ${SQLite3_LIBRARIES})
