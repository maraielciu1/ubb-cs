#!/bin/bash

# Comprehensive Parser Testing Script
# Tests both LL(1) and Yacc parsers with multiple test cases

echo "=========================================="
echo "  COMPREHENSIVE PARSER TESTING"
echo "=========================================="
echo ""

SCANNER="../lab8/scanner/scanner"
LL1_PARSER="../lab7/ll1_parser/ll1_parser"
YACC_PARSER="../lab8/parser/parser"

# Test programs
TESTS=(
    "test_simple:Simple program (valid)"
    "test_complex:Complex program with IF/WHILE/MEAN (valid)"
    "test_lexical_error:Lexical error (@ symbol)"
    "test_syntax_error:Syntax error (missing semicolon)"
)

mkdir -p results

echo "Test Programs:"
for test in "${TESTS[@]}"; do
    IFS=':' read -r name desc <<< "$test"
    echo "  ✓ $name.src - $desc"
done
echo ""

# Function to run scanner
run_scanner() {
    local test_name=$1
    local src_file="${test_name}.src"
    
    echo "  Running scanner..."
    $SCANNER "$src_file" > results/${test_name}_scanner.log 2>&1
    local exit_code=$?
    
    if [ $exit_code -eq 0 ]; then
        if [ -f "../lab8/scanner/out/pif.txt" ]; then
            cp "../lab8/scanner/out/pif.txt" "results/${test_name}.pif"
            cp "../lab8/scanner/out/st.txt" "results/${test_name}.st"
            cp "../lab8/scanner/out/lexical_errors.txt" "results/${test_name}_lexical_errors.txt" 2>/dev/null
            
            # Check for lexical errors
            if [ -f "results/${test_name}_lexical_errors.txt" ] && [ -s "results/${test_name}_lexical_errors.txt" ]; then
                echo "  ✗ Lexical errors detected!"
                return 1
            fi
            echo "  ✓ Scanner: SUCCESS"
            return 0
        fi
    fi
    echo "  ✗ Scanner: FAILED"
    return 1
}

# Function to run LL(1) parser
run_ll1() {
    local test_name=$1
    local pif_file="results/${test_name}.pif"
    
    if [ ! -f "$pif_file" ]; then
        echo "  ⊘ LL(1): SKIPPED (no PIF file)"
        return 2
    fi
    
    echo "  Running LL(1) parser..."
    
    # Copy symbol table for parser
    cp "results/${test_name}.st" st.txt 2>/dev/null
    
    $LL1_PARSER "$pif_file" > results/${test_name}_ll1.log 2>&1
    local exit_code=$?
    
    if [ $exit_code -eq 0 ]; then
        echo "  ✓ LL(1): SUCCESS"
        if [ -f "out/productions.txt" ]; then
            cp out/productions.txt "results/${test_name}_ll1_productions.txt"
            local prod_count=$(grep -c ":" "results/${test_name}_ll1_productions.txt")
            echo "    Productions: $prod_count"
        fi
        if [ -f "out/parse_tree_table.txt" ]; then
            cp out/parse_tree_table.txt "results/${test_name}_ll1_tree.txt"
            local node_count=$(tail -1 "results/${test_name}_ll1_tree.txt" | grep -o '[0-9]*' | head -1)
            echo "    Parse tree nodes: $node_count"
        fi
        return 0
    else
        echo "  ✗ LL(1): FAILED"
        grep -i "error\|failed\|syntax" results/${test_name}_ll1.log | head -3
        return 1
    fi
}

# Function to run Yacc parser
run_yacc() {
    local test_name=$1
    local pif_file="results/${test_name}.pif"
    
    if [ ! -f "$pif_file" ]; then
        echo "  ⊘ Yacc: SKIPPED (no PIF file)"
        return 2
    fi
    
    echo "  Running Yacc parser..."
    
    # Copy symbol table for parser
    cp "results/${test_name}.st" st.txt 2>/dev/null
    
    $YACC_PARSER "$pif_file" > results/${test_name}_yacc.log 2>&1
    local exit_code=$?
    
    if [ $exit_code -eq 0 ]; then
        echo "  ✓ Yacc: SUCCESS"
        if [ -f "out/productions.txt" ]; then
            cp out/productions.txt "results/${test_name}_yacc_productions.txt"
            local prod_count=$(grep -c ":" "results/${test_name}_yacc_productions.txt")
            echo "    Productions: $prod_count"
        fi
        if [ -f "out/generated_program.c" ]; then
            cp out/generated_program.c "results/${test_name}_yacc_generated.c"
            local c_lines=$(wc -l < "results/${test_name}_yacc_generated.c")
            echo "    C code: $c_lines lines"
            
            # Try to compile
            gcc "results/${test_name}_yacc_generated.c" -o "results/${test_name}_program" -lm 2>/dev/null
            if [ $? -eq 0 ]; then
                echo "    Compilation: SUCCESS"
            fi
        fi
        return 0
    else
        echo "  ✗ Yacc: FAILED"
        grep -i "error" results/${test_name}_yacc.log | head -3
        return 1
    fi
}

# Run all tests
echo "=========================================="
echo ""

for test in "${TESTS[@]}"; do
    IFS=':' read -r test_name desc <<< "$test"
    
    echo "=========================================="
    echo "TEST: $test_name"
    echo "Description: $desc"
    echo "=========================================="
    echo ""
    
    # Show source code
    echo "Source Code:"
    echo "---"
    cat "${test_name}.src"
    echo "---"
    echo ""
    
    # Run scanner
    run_scanner "$test_name"
    scanner_result=$?
    echo ""
    
    if [ $scanner_result -eq 0 ]; then
        # Show PIF info
        echo "PIF Information:"
        local token_count=$(wc -l < "results/${test_name}.pif")
        echo "  Tokens: $token_count"
        echo ""
        
        # Run both parsers
        run_ll1 "$test_name"
        ll1_result=$?
        echo ""
        
        run_yacc "$test_name"
        yacc_result=$?
        echo ""
        
        # Summary
        echo "Result Summary:"
        echo "  Scanner: ✓ SUCCESS"
        if [ $ll1_result -eq 0 ]; then
            echo "  LL(1):   ✓ SUCCESS"
        else
            echo "  LL(1):   ✗ FAILED"
        fi
        if [ $yacc_result -eq 0 ]; then
            echo "  Yacc:    ✓ SUCCESS"
        else
            echo "  Yacc:    ✗ FAILED"
        fi
    else
        echo "Result Summary:"
        echo "  Scanner: ✗ FAILED (lexical errors detected)"
        echo "  LL(1):   ⊘ SKIPPED"
        echo "  Yacc:    ⊘ SKIPPED"
        echo ""
        echo "Lexical Errors:"
        cat "results/${test_name}_lexical_errors.txt" 2>/dev/null || echo "  (error file not found)"
    fi
    
    echo ""
    echo ""
done

echo "=========================================="
echo "  COMPARISON SUMMARY"
echo "=========================================="
echo ""

printf "%-25s %-10s %-10s %-10s\n" "Test Case" "Scanner" "LL(1)" "Yacc"
printf "%-25s %-10s %-10s %-10s\n" "-------------------------" "----------" "----------" "----------"

for test in "${TESTS[@]}"; do
    IFS=':' read -r test_name desc <<< "$test"
    
    # Check scanner
    if [ -f "results/${test_name}.pif" ] && [ ! -s "results/${test_name}_lexical_errors.txt" ]; then
        scanner_status="✓ PASS"
    else
        scanner_status="✗ FAIL"
    fi
    
    # Check LL(1)
    if [ -f "results/${test_name}_ll1_productions.txt" ]; then
        ll1_status="✓ PASS"
    elif [ -f "results/${test_name}_ll1.log" ]; then
        ll1_status="✗ FAIL"
    else
        ll1_status="⊘ SKIP"
    fi
    
    # Check Yacc
    if [ -f "results/${test_name}_yacc_productions.txt" ]; then
        yacc_status="✓ PASS"
    elif [ -f "results/${test_name}_yacc.log" ]; then
        yacc_status="✗ FAIL"
    else
        yacc_status="⊘ SKIP"
    fi
    
    printf "%-25s %-10s %-10s %-10s\n" "$test_name" "$scanner_status" "$ll1_status" "$yacc_status"
done

echo ""
echo "Output files saved in: results/"
echo "Test completed at: $(date)"
echo "=========================================="
