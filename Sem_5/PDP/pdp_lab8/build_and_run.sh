#!/bin/bash

# Build and Run Script for DSM Lab 8

set -e  # Exit on error

echo "======================================"
echo "  Distributed Shared Memory - Lab 8"
echo "======================================"
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if MPI is installed
if ! command -v mpirun &> /dev/null; then
    echo -e "${RED}ERROR: MPI is not installed!${NC}"
    echo "Please install MPI first:"
    echo "  macOS:   brew install open-mpi"
    echo "  Ubuntu:  sudo apt-get install libopenmpi-dev openmpi-bin"
    exit 1
fi

echo -e "${GREEN}✓ MPI found: $(mpirun --version | head -n 1)${NC}"
echo ""

# Clean previous build
if [ -d "build" ]; then
    echo -e "${YELLOW}Cleaning previous build...${NC}"
    rm -rf build
fi

# Create build directory
echo -e "${YELLOW}Creating build directory...${NC}"
mkdir -p build
cd build

# Configure with CMake
echo -e "${YELLOW}Configuring with CMake...${NC}"
cmake .. || {
    echo -e "${RED}ERROR: CMake configuration failed!${NC}"
    exit 1
}

# Build
echo -e "${YELLOW}Building project...${NC}"
make || {
    echo -e "${RED}ERROR: Build failed!${NC}"
    exit 1
}

echo -e "${GREEN}✓ Build successful!${NC}"
echo ""

# Run with 4 processes
echo -e "${YELLOW}Running with 4 MPI processes...${NC}"
echo "======================================"
echo ""

mpirun -np 4 ./pdp_lab8

echo ""
echo "======================================"
echo -e "${GREEN}✓ Execution completed!${NC}"

