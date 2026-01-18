cmake_minimum_required(VERSION 3.29)
project(lab5_pdp)

set(CMAKE_CXX_STANDARD 20)

# Add executable
add_executable(lab5_pdp 
    main.cpp
    polynomial.cpp
    benchmark.cpp
    bigint.cpp
    bigint_polynomial.cpp
)

# Note: Uses std::thread and std::async for parallelization
# No external dependencies required - standard C++20 is sufficient
