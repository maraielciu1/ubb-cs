# Parser Comparison - How to Run the Demo

## Quick Start

```bash
cd parser_comparison

# Option 1: Full comprehensive test (recommended)
./test_all_programs.sh

# Option 2: Performance benchmark (100 iterations)
./compare_parsers.sh
```

## What Each Script Does

### 1. `test_all_programs.sh` - Comprehensive Testing ✨

Tests both parsers with 4 different programs:
- ✅ **test_simple.src** - Simple valid program
- ✅ **test_complex.src** - Complex program (IF/WHILE/MEAN)
- ❌ **test_lexical_error.src** - Program with @ symbol (invalid)
- ❌ **test_syntax_error.src** - Program with missing semicolon

**Output**:
- Shows each test case with source code
- Runs scanner → LL(1) parser → Yacc parser
- Shows success/failure for each stage
- Compares productions and outputs
- Saves all results to `results/` directory

**Example Output**:
```
==========================================
TEST: test_simple
Description: Simple program (valid)
==========================================

Source Code:
---
START
    INT !x = 5;
    INT !y = 10;
    !x = !x + !y;
STOP
---

  ✓ Scanner: SUCCESS
  ✓ LL(1): SUCCESS
    Productions: 36
    Parse tree nodes: 61
  ✓ Yacc: SUCCESS
    Productions: 24
    C code: 28 lines
    Compilation: SUCCESS

Result Summary:
  Scanner: ✓ SUCCESS
  LL(1):   ✓ SUCCESS
  Yacc:    ✓ SUCCESS
```

### 2. `compare_parsers.sh` - Performance Benchmark 🚀

Runs both parsers 100 times on a valid program to measure speed:
- Measures execution time
- Calculates average per parse
- Shows parses per second
- Compares outputs

**Example Output**:
```
==========================================
   PARSER COMPARISON: LL(1) vs YACC
==========================================

LL(1) Parser:
  Total time: 0.189431s
  Average time per parse: 0.001894s
  Parses per second: 527.89

Yacc Parser:
  Total time: 0.179613s
  Average time per parse: 0.001796s
  Parses per second: 556.75

Winner: Yacc Parser
  Yacc is 1.05x FASTER than LL(1)
```

## Test Programs

### Simple Program (test_simple.src)
```
START
    INT !x = 5;
    INT !y = 10;
    !x = !x + !y;
STOP
```
- 3 statements
- Basic arithmetic
- Tests: declarations, assignment, operators

### Complex Program (test_complex.src)
```
START
    INT !counter = 0;
    INT !sum = 0;
    FLOAT !average = 0.0;
    
    !counter = 5;
    !sum = !counter * 2;
    
    IF (!sum > 8) {
        WRITE(!sum);
    }
    
    WHILE (!counter > 0) {
        !counter = !counter - 1;
    }
    
    !average = MEAN(!sum, !counter, 10);
    WRITE(!average);
STOP
```
- Multiple statements
- Control flow (IF, WHILE)
- Statistical function (MEAN)
- I/O (WRITE)
- Tests: complex features, nested structures

### Lexical Error Program (test_lexical_error.src)
```
START
    INT !x = 5;
    INT !y@ = 10;    // @ is invalid!
    !x = !x + !y@;
STOP
```
- Contains invalid character (@)
- Tests: Scanner error detection

### Syntax Error Program (test_syntax_error.src)
```
START
    INT !x = 5;
    INT !y = 10     // Missing semicolon!
    !x = !x + !y;
STOP
```
- Missing semicolon after line 3
- Tests: Parser error detection

## Output Files

After running the scripts, check these directories:

### `results/` - Test outputs
```
results/
├── test_simple_ll1_productions.txt    # LL(1) productions
├── test_simple_ll1_tree.txt           # LL(1) parse tree
├── test_simple_yacc_productions.txt   # Yacc productions
├── test_simple_yacc_generated.c       # Yacc C code
├── test_complex_ll1_productions.txt
├── test_complex_yacc_generated.c
└── ...
```

### `out/` - Latest parser outputs
```
out/
├── productions.txt          # Productions from last run
├── parse_tree_table.txt     # LL(1) parse tree (if successful)
├── derivations.txt          # LL(1) derivations (if successful)
└── generated_program.c      # Yacc C code (if successful)
```

## Viewing Results

### LL(1) Parser Outputs

**Productions**:
```bash
cat results/test_simple_ll1_productions.txt
```
Shows: `1: program -> START statement_list STOP`

**Parse Tree**:
```bash
cat results/test_simple_ll1_tree.txt
```
Shows: Node relationships (father-sibling representation)

### Yacc Parser Outputs

**Productions**:
```bash
cat results/test_simple_yacc_productions.txt
```
Shows: `1: type -> INT`

**Generated C Code**:
```bash
cat results/test_simple_yacc_generated.c
```
Shows: Complete C program with runtime library

**Compile and Run**:
```bash
cd results
gcc test_simple_yacc_generated.c -o program -lm
./program
```

## Expected Results

| Test Case | Scanner | LL(1) | Yacc |
|-----------|---------|-------|------|
| test_simple | ✅ | ✅ | ✅ |
| test_complex | ✅ | ✅ | ✅ |
| test_lexical_error | ❌ | ⊘ | ⊘ |
| test_syntax_error | ✅ | ❌ | ❌ |

Legend:
- ✅ = Success
- ❌ = Error detected (as expected)
- ⊘ = Skipped (due to previous stage failure)

## Key Comparisons to Note

### 1. Productions Count
- **LL(1)**: More productions (36 for simple, 108 for complex)
  - Due to right-recursive grammar
  - Includes epsilon productions
- **Yacc**: Fewer productions (24 for simple, 68 for complex)
  - Uses left-recursive grammar
  - More direct rules

### 2. Output Types
- **LL(1)**: Parse tree structure
  - Explicit node relationships
  - Father-sibling representation
  - Complete derivation sequence
- **Yacc**: Executable C code
  - Translated program
  - Includes runtime library
  - Ready to compile and run

### 3. Error Messages
- **LL(1)**: Specific
  - "Syntax error: expected SEMICOLON, got IDENTIFIER"
- **Yacc**: Generic
  - "[SYNTAX ERROR] at line 3: syntax error"

### 4. Performance
- **LL(1)**: ~1.89ms per parse
- **Yacc**: ~1.80ms per parse (5% faster)

## Troubleshooting

### "Permission denied"
```bash
chmod +x *.sh
```

### "Scanner not found"
Check that lab8 scanner is built:
```bash
cd ../lab8/scanner
make
```

### "Parser not found"
Check that both parsers are built:
```bash
cd ../lab7/ll1_parser && make
cd ../../lab8/parser && make
```

### Results directory exists but empty
Run the test scripts again - they create files automatically.

## Documentation

For detailed analysis, see:
- **PARSER_COMPARISON_FINAL.md** - Complete comparison (root directory)
- **manual_test_comparison.md** - Test case details (this directory)
- **README.md** - Quick summary (this directory)

---

**Quick Demo Command**:
```bash
cd parser_comparison && ./test_all_programs.sh
```

This will run all tests and show you everything! 🎉
