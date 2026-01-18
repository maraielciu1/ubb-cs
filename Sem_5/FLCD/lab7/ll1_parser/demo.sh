#!/bin/bash

echo "=========================================="
echo "Lab 7: LL(1) Parser Demonstration"
echo "=========================================="
echo ""

echo "Requirement 1: Arithmetic Grammar + Sequence"
echo "--------------------------------------------"
echo "Grammar: E → T E', E' → + T E' | ε, T → F T', T' → * F T' | ε, F → (E) | a"
echo "Sequence: a * (a + a)"
echo ""
cd "$(dirname "$0")"
make req1 >/dev/null 2>&1
./requirement1
echo ""

echo "=========================================="
echo ""

echo "Requirement 2: Mini-DSL Grammar + PIF"
echo "--------------------------------------"
echo "Input: PIF file from scanner"
echo "Output: Parse tree table (Grade 10 format)"
echo ""

# Generate PIF if needed
if [ ! -f "../../lab6/scanner/out/pif.txt" ]; then
    echo "Generating PIF file..."
    cd ../../lab6/scanner
    ./scanner test.src >/dev/null 2>&1
    cd ../../lab7/ll1_parser
fi

./ll1_parser ../../lab6/scanner/out/pif.txt
echo ""

echo "=========================================="
echo "Output Files:"
echo "  - Requirement 1: out/requirement1_parse_tree.txt"
echo "  - Requirement 2: out/parse_tree_table.txt"
echo "=========================================="

