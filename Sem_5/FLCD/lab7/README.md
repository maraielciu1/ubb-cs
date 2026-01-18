# Lab 7: LL(1) Parser Implementation

This lab implements an LL(1) parser for the mini-DSL language with parse tree output as a table using father-sibling relations.

## Requirements

1. **Requirement 1**: Implement and test LL(1) parsing algorithm
   - Input: grammar from seminar, sequence (PIF)
   - Output: string of productions

2. **Requirement 2**: Test with grammar for mini-DSL and PIF
   - Input: grammar for mini-DSL, PIF
   - Output: Parse tree representation as table (father-sibling relations)
   - **Grade: 10/10** (maximum grade option)

## Structure

```
lab7/
├── ll1_parser/
│   ├── ll1_parser.h       # Data structures and declarations
│   ├── ll1_parser.c       # Core LL(1) parser implementation
│   ├── pif_reader.h       # PIF file reader header
│   ├── pif_reader.c       # PIF file reader implementation
│   ├── main.c             # Main program
│   ├── Makefile           # Build configuration
│   └── out/
│       └── parse_tree_table.txt  # Output: parse tree table
└── documentation.txt       # Detailed documentation
```

## Building

```bash
cd ll1_parser
make
```

## Usage

1. **Generate PIF file** using scanner from Lab 6:
   ```bash
   cd ../../lab6/scanner
   ./scanner test.src
   ```

2. **Run LL(1) parser**:
   ```bash
   cd ../../../lab7/ll1_parser
   ./ll1_parser ../../lab6/scanner/out/pif.txt
   ```

## Output Format

The parser generates `out/parse_tree_table.txt` with the parse tree represented as a table:

```
Parse Tree Table (Father-Sibling Representation)
================================================

Node ID   Symbol                          Father ID       Sibling ID
--------  ------------------------------  --------------- ---------------
1         program                          -1              -1
2         START                            1               3
3         statement_list                   1               4
4         STOP                             1               -1
...
```

### Table Columns

- **Node ID**: Unique identifier for each node
- **Symbol**: Grammar symbol (terminal or non-terminal)
- **Father ID**: ID of parent node (-1 for root)
- **Sibling ID**: ID of next sibling node (-1 if none)

This representation allows complete reconstruction of the parse tree structure.

## LL(1) Parsing Algorithm

The parser uses:
- **Top-down predictive parsing** (left-to-right, leftmost derivation)
- **1 symbol lookahead**
- **Parsing table** constructed from FIRST and FOLLOW sets
- **Stack-based algorithm** to build parse tree

## Grammar Conversion

The original grammar was converted from left recursion to right recursion for LL(1) compatibility:

- `expression -> expression + term` → `expression -> term expression_prime`
- `term -> term * factor` → `term -> factor term_prime`
- Similar conversions for `statement_list` and `condition`

## Features

- ✅ LL(1) parsing algorithm implementation
- ✅ FIRST and FOLLOW set computation
- ✅ Parsing table construction
- ✅ Parse tree building with parent-sibling relations
- ✅ Table output format (grade 10 option)
- ✅ PIF file input support
- ✅ Complete documentation

## Testing

Test with various programs:
- Simple assignments
- Expressions with operators
- If/else statements
- While loops
- Nested structures

## Documentation

See `documentation.txt` for detailed explanation of:
- LL(1) algorithm theory
- Grammar conversion process
- FIRST/FOLLOW computation
- Parse tree structure
- Implementation details

