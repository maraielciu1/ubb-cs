#!/bin/bash

# Performance comparison script for Hamiltonian Cycle Finder
# Compares C++ (std::async) vs Java (ForkJoinPool) implementations

set -e  # Exit on error

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "=========================================="
echo "Hamiltonian Cycle Finder - Performance Comparison"
echo "=========================================="
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Build C++ program
echo -e "${BLUE}[1/4]${NC} Building C++ program..."
mkdir -p cmake-build-debug
cd cmake-build-debug
if [ ! -f pdp_lab6 ] || [ ../main.cpp -nt pdp_lab6 ]; then
    cmake .. > /dev/null 2>&1
    make > /dev/null 2>&1
fi
cd ..

# Compile Java program
echo -e "${BLUE}[2/4]${NC} Compiling Java program..."
if [ ! -f HamiltonianForkJoin.class ] || [ HamiltonianForkJoin.java -nt HamiltonianForkJoin.class ]; then
    javac HamiltonianForkJoin.java
fi

# Run C++ program and capture output
echo -e "${BLUE}[3/4]${NC} Running C++ implementation..."
CPP_OUTPUT=$(./cmake-build-debug/pdp_lab6 2>&1)

# Run Java program and capture output
echo -e "${BLUE}[4/4]${NC} Running Java implementation..."
JAVA_OUTPUT=$(java HamiltonianForkJoin 2>&1)

echo ""
echo "=========================================="
echo "PERFORMANCE COMPARISON RESULTS"
echo "=========================================="
echo ""

# Create temporary files for parsing
TMPDIR=$(mktemp -d)
trap "rm -rf $TMPDIR" EXIT

CPP_TIMES="$TMPDIR/cpp_times.txt"
JAVA_TIMES="$TMPDIR/java_times.txt"

# Extract C++ timings (test name and time pairs)
current_test=""
while IFS= read -r line; do
    if [[ $line =~ ^=====\ Test:\ (.+)\ ===== ]]; then
        current_test="${BASH_REMATCH[1]}"
    elif [[ $line =~ Execution\ time:\ ([0-9.]+)\ ms ]]; then
        echo "$current_test|${BASH_REMATCH[1]}" >> "$CPP_TIMES"
    fi
done <<< "$CPP_OUTPUT"

# Extract Java timings
current_test=""
while IFS= read -r line; do
    if [[ $line =~ ^=====\ Test:\ (.+)\ ===== ]]; then
        current_test="${BASH_REMATCH[1]}"
    elif [[ $line =~ Execution\ time:\ ([0-9.]+)\ ms ]]; then
        echo "$current_test|${BASH_REMATCH[1]}" >> "$JAVA_TIMES"
    fi
done <<< "$JAVA_OUTPUT"

# Print comparison table
printf "%-45s %15s %15s %15s\n" "Test Graph" "C++ (ms)" "Java (ms)" "Speedup"
echo "----------------------------------------------------------------------------------------"

# Process each C++ test result
while IFS='|' read -r test_name cpp_time; do
    # Find matching Java time
    java_time=$(grep "^${test_name}|" "$JAVA_TIMES" 2>/dev/null | cut -d'|' -f2 || echo "N/A")
    
    if [[ "$java_time" != "N/A" && -n "$cpp_time" ]]; then
        # Calculate speedup using awk (more portable than bc)
        speedup=$(awk "BEGIN {if ($cpp_time > 0) printf \"%.2f\", $java_time / $cpp_time; else print \"N/A\"}" 2>/dev/null || echo "N/A")
        
        if [[ "$speedup" != "N/A" ]]; then
            # Compare using awk
            comparison=$(awk "BEGIN {if ($speedup > 1.01) print 1; else if ($speedup < 0.99) print -1; else print 0}" 2>/dev/null || echo "0")
            
            if [ "$comparison" = "1" ]; then
                # C++ is faster
                color=$GREEN
                speedup_str="${speedup}x (C++ faster)"
            elif [ "$comparison" = "-1" ]; then
                # Java is faster
                speedup_val=$(awk "BEGIN {printf \"%.2f\", 1 / $speedup}" 2>/dev/null || echo "N/A")
                color=$YELLOW
                speedup_str="${speedup_val}x (Java faster)"
            else
                color=$NC
                speedup_str="1.00x (equal)"
            fi
        else
            color=$NC
            speedup_str="N/A"
        fi
        
        printf "%-45s %15s %15s ${color}%15s${NC}\n" "$test_name" "$cpp_time" "$java_time" "$speedup_str"
    else
        printf "%-45s %15s %15s %15s\n" "$test_name" "${cpp_time:-N/A}" "${java_time:-N/A}" "N/A"
    fi
done < "$CPP_TIMES"

echo ""
echo "=========================================="
echo "DETAILED OUTPUT"
echo "=========================================="
echo ""

echo -e "${BLUE}--- C++ Implementation Output ---${NC}"
echo "$CPP_OUTPUT"
echo ""

echo -e "${BLUE}--- Java Implementation Output ---${NC}"
echo "$JAVA_OUTPUT"
echo ""

# Calculate averages if we have data
cpp_total=0
java_total=0
count=0

while IFS='|' read -r test_name cpp_time; do
    java_time=$(grep "^${test_name}|" "$JAVA_TIMES" 2>/dev/null | cut -d'|' -f2 || echo "")
    if [[ -n "$java_time" && -n "$cpp_time" ]]; then
        cpp_total=$(awk "BEGIN {print $cpp_total + $cpp_time}" 2>/dev/null || echo "$cpp_total")
        java_total=$(awk "BEGIN {print $java_total + $java_time}" 2>/dev/null || echo "$java_total")
        count=$((count + 1))
    fi
done < "$CPP_TIMES"

if [ $count -gt 0 ]; then
    cpp_avg=$(awk "BEGIN {printf \"%.2f\", $cpp_total / $count}" 2>/dev/null || echo "N/A")
    java_avg=$(awk "BEGIN {printf \"%.2f\", $java_total / $count}" 2>/dev/null || echo "N/A")
    
    echo "=========================================="
    echo "SUMMARY"
    echo "=========================================="
    printf "Average C++ time:  %s ms\n" "$cpp_avg"
    printf "Average Java time: %s ms\n" "$java_avg"
    
    if [[ "$cpp_avg" != "N/A" && "$java_avg" != "N/A" ]]; then
        overall_speedup=$(awk "BEGIN {printf \"%.2f\", $java_avg / $cpp_avg}" 2>/dev/null || echo "N/A")
        if [[ "$overall_speedup" != "N/A" ]]; then
            comparison=$(awk "BEGIN {if ($overall_speedup > 1.01) print 1; else if ($overall_speedup < 0.99) print -1; else print 0}" 2>/dev/null || echo "0")
            if [ "$comparison" = "1" ]; then
                printf "Overall: C++ is %.2fx faster\n" "$overall_speedup"
            elif [ "$comparison" = "-1" ]; then
                java_speedup=$(awk "BEGIN {printf \"%.2f\", 1 / $overall_speedup}" 2>/dev/null || echo "N/A")
                printf "Overall: Java is %.2fx faster\n" "$java_speedup"
            else
                printf "Overall: Performance is approximately equal\n"
            fi
        fi
    fi
    echo ""
fi

echo "=========================================="
