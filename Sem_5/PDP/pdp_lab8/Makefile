# Makefile for DSM Lab 8

.PHONY: all build clean run help

# Default target
all: build

# Build the project
build:
	@echo "Building DSM Lab 8..."
	@mkdir -p build
	@cd build && cmake .. && make
	@echo "✓ Build complete!"

# Clean build artifacts
clean:
	@echo "Cleaning build directory..."
	@rm -rf build
	@echo "✓ Clean complete!"

# Build and run with 4 MPI processes
run: build
	@echo "Running with 4 MPI processes..."
	@echo "======================================"
	@mpirun -np 4 ./build/pdp_lab8
	@echo "======================================"
	@echo "✓ Execution complete!"

# Rebuild from scratch
rebuild: clean build

# Show help
help:
	@echo "DSM Lab 8 - Makefile Commands"
	@echo "=============================="
	@echo "  make          - Build the project"
	@echo "  make build    - Build the project"
	@echo "  make run      - Build and run with 4 processes"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make rebuild  - Clean and rebuild"
	@echo "  make help     - Show this help message"

