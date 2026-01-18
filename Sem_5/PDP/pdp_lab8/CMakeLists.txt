cmake_minimum_required(VERSION 3.29)
project(pdp_lab8)

set(CMAKE_CXX_STANDARD 20)

# Find MPI package
find_package(MPI REQUIRED)

# Add executable
add_executable(pdp_lab8 main.cpp)

# Link MPI libraries
target_link_libraries(pdp_lab8 MPI::MPI_CXX)

# Include MPI headers
target_include_directories(pdp_lab8 PRIVATE ${MPI_CXX_INCLUDE_DIRS})
