cmake_minimum_required(VERSION 3.29)
project(lab7)

set(CMAKE_CXX_STANDARD 20)

# Find MPI
find_package(MPI REQUIRED COMPONENTS CXX)

# Add executable
add_executable(lab7 main.cpp)

# Link MPI libraries
target_link_libraries(lab7 PUBLIC MPI::MPI_CXX)
