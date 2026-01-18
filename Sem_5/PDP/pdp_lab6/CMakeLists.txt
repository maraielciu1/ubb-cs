cmake_minimum_required(VERSION 3.10)
project(pdp_lab6)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable threading
find_package(Threads REQUIRED)

# Add executable
add_executable(pdp_lab6 main.cpp)

# Link threading library
target_link_libraries(pdp_lab6 Threads::Threads)

# Set compiler flags for better performance
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(pdp_lab6 PRIVATE -Wall -Wextra -O2)
endif()


