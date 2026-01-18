#!/bin/bash

# Parser Comparison Script
# Compares LL(1) Parser (Lab 7) vs Yacc Parser (Lab 8)

echo "=========================================="
echo "   PARSER COMPARISON: LL(1) vs YACC"
echo "=========================================="
echo ""

# Test file
PIF_FILE="valid_pif.txt"
TEST_SRC="valid_test.src"

# Parser paths
LL1_PARSER="../lab7/ll1_parser/ll1_parser"
YACC_PARSER="../lab8/parser/parser"

# Number of iterations
ITERATIONS=100

echo "Test Configuration:"
echo "  - Input file: $TEST_SRC"
echo "  - PIF tokens: $(wc -l < $PIF_FILE)"
echo "  - Iterations: $ITERATIONS"
echo ""

# Check if parsers exist
if [ ! -f "$LL1_PARSER" ]; then
    echo "Error: LL(1) parser not found at $LL1_PARSER"
    exit 1
fi

if [ ! -f "$YACC_PARSER" ]; then
    echo "Error: Yacc parser not found at $YACC_PARSER"
    exit 1
fi

echo "=========================================="
echo "   LL(1) PARSER (Lab 7)"
echo "=========================================="
echo ""

# Clean output directory
rm -f out/productions.txt out/parse_tree_table.txt out/derivations.txt 2>/dev/null

# Single run to check correctness
echo "Running single test..."
$LL1_PARSER $PIF_FILE > /tmp/ll1_output.txt 2>&1
LL1_EXIT_CODE=$?

if [ $LL1_EXIT_CODE -eq 0 ]; then
    echo "✓ Parsing: SUCCESSFUL"
    
    # Check output files
    if [ -f "out/productions.txt" ]; then
        PROD_COUNT=$(grep -c ":" out/productions.txt)
        echo "✓ Productions generated: $PROD_COUNT"
    fi
    
    if [ -f "out/parse_tree_table.txt" ]; then
        NODE_COUNT=$(tail -1 out/parse_tree_table.txt | grep -o '[0-9]*' | head -1)
        echo "✓ Parse tree nodes: $NODE_COUNT"
    fi
    
    if [ -f "out/derivations.txt" ]; then
        DERIV_STEPS=$(tail -1 out/derivations.txt | grep -o '[0-9]*' | head -1)
        echo "✓ Derivation steps: $DERIV_STEPS"
    fi
else
    echo "✗ Parsing: FAILED"
    cat /tmp/ll1_output.txt
    exit 1
fi

echo ""
echo "Running performance test ($ITERATIONS iterations)..."

# Measure time for LL(1) parser
START_TIME=$(date +%s.%N)
for i in $(seq 1 $ITERATIONS); do
    $LL1_PARSER $PIF_FILE > /dev/null 2>&1
done
END_TIME=$(date +%s.%N)

LL1_TOTAL_TIME=$(echo "$END_TIME - $START_TIME" | bc)
LL1_AVG_TIME=$(echo "scale=6; $LL1_TOTAL_TIME / $ITERATIONS" | bc)
LL1_PER_SEC=$(echo "scale=2; $ITERATIONS / $LL1_TOTAL_TIME" | bc)

echo "  Total time: ${LL1_TOTAL_TIME}s"
echo "  Average time per parse: ${LL1_AVG_TIME}s"
echo "  Parses per second: $LL1_PER_SEC"

# Save LL(1) outputs
cp out/productions.txt out/ll1_productions.txt 2>/dev/null
cp out/parse_tree_table.txt out/ll1_parse_tree.txt 2>/dev/null
cp out/derivations.txt out/ll1_derivations.txt 2>/dev/null

echo ""
echo "=========================================="
echo "   YACC PARSER (Lab 8)"
echo "=========================================="
echo ""

# Clean output directory
rm -f out/productions.txt out/generated_program.c 2>/dev/null

# Single run to check correctness
echo "Running single test..."
$YACC_PARSER $PIF_FILE > /tmp/yacc_output.txt 2>&1
YACC_EXIT_CODE=$?

if [ $YACC_EXIT_CODE -eq 0 ]; then
    echo "✓ Parsing: SUCCESSFUL"
    
    # Check output files
    if [ -f "out/productions.txt" ]; then
        PROD_COUNT=$(grep -c ":" out/productions.txt)
        echo "✓ Productions generated: $PROD_COUNT"
    fi
    
    if [ -f "out/generated_program.c" ]; then
        C_LINES=$(wc -l < out/generated_program.c)
        echo "✓ C code generated: $C_LINES lines"
        
        # Try to compile
        gcc out/generated_program.c -o out/generated_program -lm 2>/dev/null
        if [ $? -eq 0 ]; then
            echo "✓ C compilation: SUCCESSFUL"
        else
            echo "✗ C compilation: FAILED"
        fi
    fi
else
    echo "✗ Parsing: FAILED"
    cat /tmp/yacc_output.txt
fi

echo ""
echo "Running performance test ($ITERATIONS iterations)..."

# Measure time for Yacc parser
START_TIME=$(date +%s.%N)
for i in $(seq 1 $ITERATIONS); do
    $YACC_PARSER $PIF_FILE > /dev/null 2>&1
done
END_TIME=$(date +%s.%N)

YACC_TOTAL_TIME=$(echo "$END_TIME - $START_TIME" | bc)
YACC_AVG_TIME=$(echo "scale=6; $YACC_TOTAL_TIME / $ITERATIONS" | bc)
YACC_PER_SEC=$(echo "scale=2; $ITERATIONS / $YACC_TOTAL_TIME" | bc)

echo "  Total time: ${YACC_TOTAL_TIME}s"
echo "  Average time per parse: ${YACC_AVG_TIME}s"
echo "  Parses per second: $YACC_PER_SEC"

# Save Yacc outputs
cp out/productions.txt out/yacc_productions.txt 2>/dev/null
cp out/generated_program.c out/yacc_generated.c 2>/dev/null

echo ""
echo "=========================================="
echo "   PERFORMANCE COMPARISON"
echo "=========================================="
echo ""

# Compare times
if (( $(echo "$LL1_TOTAL_TIME < $YACC_TOTAL_TIME" | bc -l) )); then
    SPEEDUP=$(echo "scale=2; $YACC_TOTAL_TIME / $LL1_TOTAL_TIME" | bc)
    echo "Winner: LL(1) Parser"
    echo "  LL(1) is ${SPEEDUP}x FASTER than Yacc"
else
    SPEEDUP=$(echo "scale=2; $LL1_TOTAL_TIME / $YACC_TOTAL_TIME" | bc)
    echo "Winner: Yacc Parser"
    echo "  Yacc is ${SPEEDUP}x FASTER than LL(1)"
fi

echo ""
echo "Detailed Comparison:"
printf "%-30s %15s %15s\n" "Metric" "LL(1)" "Yacc"
printf "%-30s %15s %15s\n" "------------------------------" "---------------" "---------------"
printf "%-30s %15.6fs %15.6fs\n" "Total time ($ITERATIONS runs)" "$LL1_TOTAL_TIME" "$YACC_TOTAL_TIME"
printf "%-30s %15.6fs %15.6fs\n" "Average time per parse" "$LL1_AVG_TIME" "$YACC_AVG_TIME"
printf "%-30s %15.2f %15.2f\n" "Parses per second" "$LL1_PER_SEC" "$YACC_PER_SEC"

echo ""
echo "Output Comparison:"
printf "%-30s %15s %15s\n" "Feature" "LL(1)" "Yacc"
printf "%-30s %15s %15s\n" "------------------------------" "---------------" "---------------"

# Productions
if [ -f "out/ll1_productions.txt" ] && [ -f "out/yacc_productions.txt" ]; then
    LL1_PRODS=$(grep -c ":" out/ll1_productions.txt)
    YACC_PRODS=$(grep -c ":" out/yacc_productions.txt)
    printf "%-30s %15s %15s\n" "Productions used" "$LL1_PRODS" "$YACC_PRODS"
fi

# Parse tree
if [ -f "out/ll1_parse_tree.txt" ]; then
    NODES=$(tail -1 out/ll1_parse_tree.txt | grep -o 'Total nodes: [0-9]*' | grep -o '[0-9]*')
    printf "%-30s %15s %15s\n" "Parse tree nodes" "$NODES" "N/A"
fi

# C code generation
if [ -f "out/yacc_generated.c" ]; then
    C_LINES=$(wc -l < out/yacc_generated.c)
    printf "%-30s %15s %15s\n" "C code lines" "N/A" "$C_LINES"
fi

echo ""
echo "=========================================="
echo "   KEY DIFFERENCES"
echo "=========================================="
echo ""
echo "LL(1) Parser:"
echo "  • Top-down predictive parsing"
echo "  • Generates explicit parse tree"
echo "  • Outputs: productions, derivations, parse tree table"
echo "  • $(($(wc -l < ll1_parser.c) + $(wc -l < main.c) + $(wc -l < pif_reader.c))) lines of hand-written code" 2>/dev/null || echo "  • ~1400 lines of hand-written code"
echo ""
echo "Yacc Parser:"
echo "  • Bottom-up LALR(1) parsing"
echo "  • Generates executable C code"
echo "  • Outputs: productions, translated C program"
echo "  • $(wc -l < ../lab8/parser/parser.y | awk '{print $1}') lines of grammar specification" 2>/dev/null || echo "  • ~366 lines of grammar specification"

echo ""
echo "=========================================="
echo "   SUMMARY"
echo "=========================================="
echo ""

if [ $LL1_EXIT_CODE -eq 0 ] && [ $YACC_EXIT_CODE -eq 0 ]; then
    echo "✓ Both parsers successfully parsed the test program"
    echo ""
    echo "Recommendations:"
    echo "  • Use LL(1) parser for: parse tree analysis, educational purposes"
    echo "  • Use Yacc parser for: code generation, production compilers"
else
    if [ $LL1_EXIT_CODE -ne 0 ]; then
        echo "✗ LL(1) parser failed"
    fi
    if [ $YACC_EXIT_CODE -ne 0 ]; then
        echo "✗ Yacc parser failed"
    fi
fi

echo ""
echo "Test completed at: $(date)"
echo "=========================================="
