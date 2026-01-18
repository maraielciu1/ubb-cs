#!/bin/bash

# Lab 8 Demo Script - Complete Translation Pipeline
# This script demonstrates the full workflow from source code to executable

echo "============================================"
echo "Lab 8: Code Generation Demo"
echo "============================================"
echo ""

# Step 1: Run the scanner
echo "[Step 1] Running Scanner..."
cd scanner
make clean > /dev/null 2>&1
make > /dev/null 2>&1

if [ $? -ne 0 ]; then
    echo "Error: Scanner build failed"
    exit 1
fi

./scanner test.src
if [ $? -ne 0 ]; then
    echo "Error: Scanner execution failed"
    exit 1
fi

echo "✓ Scanner completed successfully"
echo "  - Generated: out/pif.txt (token stream)"
echo "  - Generated: out/st.txt (symbol table)"
echo ""

# Step 2: Run the parser/code generator
echo "[Step 2] Running Parser/Code Generator..."
cd ../parser
make clean > /dev/null 2>&1
make > /dev/null 2>&1

if [ $? -ne 0 ]; then
    echo "Error: Parser build failed"
    exit 1
fi

./parser ../scanner/out/pif.txt
if [ $? -ne 0 ]; then
    echo "Error: Parser execution failed"
    exit 1
fi

echo "✓ Parser completed successfully"
echo "  - Generated: out/generated_program.c (C code)"
echo "  - Generated: out/productions.txt (parse derivation)"
echo ""

# Step 3: Compile the generated C code
echo "[Step 3] Compiling Generated C Code..."
gcc out/generated_program.c -o out/program -lm
if [ $? -ne 0 ]; then
    echo "Error: C compilation failed"
    exit 1
fi

echo "✓ C compilation successful"
echo "  - Generated: out/program (executable)"
echo ""

# Step 4: Run the generated program
echo "[Step 4] Running Generated Program..."
echo "----------------------------------------"
./out/program
echo "----------------------------------------"
echo ""

echo "============================================"
echo "Demo completed successfully!"
echo "============================================"
echo ""
echo "You can:"
echo "  - View the generated C code: cat parser/out/generated_program.c"
echo "  - View the parse tree: cat parser/out/productions.txt"
echo "  - Edit the source: vim scanner/test.src"
echo "  - Run the demo again: ./demo.sh"
