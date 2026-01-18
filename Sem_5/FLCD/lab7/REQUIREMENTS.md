# Lab 7 Requirements

## Requirement 1: Implement and Test LL(1) Algorithm

**Input**: Grammar from seminar, sequence

**Output**: String of productions

**Implementation**: `requirement1.c` + `arithmetic_grammar.c`

**Grammar**: 
```
E → E + T | T
T → T * F | F
F → (E) | a
```

**Converted to LL(1) form**:
```
E → T E'
E' → + T E' | ε
T → F T'
T' → * F T' | ε
F → (E) | a
```

**Test sequence**: `a * (a + a)`

**Usage**:
```bash
cd ll1_parser
make req1
./requirement1
```

**Output**: 
- Console: Productions applied during parsing
- File: `out/requirement1_parse_tree.txt` - Parse tree table

---

## Requirement 2: Test with Mini-DSL Grammar and PIF

**Input**: Grammar for mini-DSL, PIF file

**Output**: Parse tree representation as table (father-sibling relations)

**Implementation**: `main.c` (already implemented)

**Grammar**: Mini-DSL grammar (from Lab 1) converted to LL(1) form

**Usage**:
```bash
# First generate PIF using scanner
cd ../../lab6/scanner
./scanner test.src

# Then run Requirement 2 parser
cd ../../lab7/ll1_parser
make req2
./ll1_parser ../../lab6/scanner/out/pif.txt
```

**Output**: 
- Console: Parsing progress
- File: `out/parse_tree_table.txt` - Parse tree table with:
  - Node ID
  - Symbol
  - Father ID
  - Sibling ID


---
## Summary

- **Requirement 1**: Arithmetic grammar (E, T, F) with sequence input
- **Requirement 2**: Mini-DSL grammar with PIF input (already in `main.c`)

Both requirements output parse trees as tables with father-sibling relations.

