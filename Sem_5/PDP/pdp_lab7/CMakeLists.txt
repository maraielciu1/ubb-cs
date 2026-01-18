cmake_minimum_required(VERSION 3.15)
project(pdp_lab7)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find MPI
find_package(MPI REQUIRED)

# Include MPI directories
include_directories(${MPI_CXX_INCLUDE_PATH})

# Add executable
add_executable(pdp_lab7
    main.cpp
    polynomial.cpp
    polynomial_mpi.cpp
    bigint.cpp
    bigint_mpi.cpp
)

# Link MPI libraries
target_link_libraries(pdp_lab7 ${MPI_CXX_LIBRARIES})

# Set MPI compiler flags
set_target_properties(pdp_lab7 PROPERTIES
    COMPILE_FLAGS "${MPI_CXX_COMPILE_FLAGS}"
    LINK_FLAGS "${MPI_CXX_LINK_FLAGS}"
)

