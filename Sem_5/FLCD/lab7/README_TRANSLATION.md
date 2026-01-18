# LL(1) Parser - C Translation of Yacc Parser

## Purpose

This is a **hand-written C translation** of the Yacc parser from Lab 8.

**Original Request**: Translate the `parser.y` (Yacc-based parser) into pure C code without external dependencies.

## What This Is

A complete LL(1) parser implementation in C that:
- Parses the same mini-DSL statistical language as the Yacc parser
- Uses top-down predictive parsing instead of bottom-up
- Generates explicit parse tree structures
- Requires no external tools (Yacc/Bison)

## Key Differences from Yacc

| Aspect | Yacc (Lab 8) | This C Translation |
|--------|--------------|-------------------|
| **Lines of Code** | 366 | 1,100 |
| **Grammar** | Left-recursive | Right-recursive (LL(1)) |
| **Parse Tree** | Implicit | Explicit (father-sibling) |
| **Code Generation** | Yes (C code) | No (analysis only) |
| **Dependencies** | Yacc/Bison | None |
| **Development** | Auto-generated | Hand-written |

## Grammar Transformation

The Yacc grammar was transformed for LL(1) compatibility:

**Example - Expression Rules**:
```
Yacc: expression -> expression + term | term

LL(1): expression -> term expression_prime
       expression_prime -> + term expression_prime | ε
```

All left-recursive rules were converted to right-recursive equivalents.

## Files

- `ll1_parser.c` (1,100 lines) - Main parser implementation
- `ll1_parser.h` - Data structures and declarations
- `pif_reader.c` - PIF file reader
- `main.c` - Entry point

## Building

```bash
make
```

## Usage

```bash
./ll1_parser <pif_file>
```

## Output

1. **Productions** (`out/productions.txt`) - List of productions used
2. **Parse Tree** (`out/parse_tree_table.txt`) - Father-sibling representation
3. **Derivations** (`out/derivations.txt`) - Leftmost derivation steps

## Comparison with Yacc

See `../YACC_TO_C_TRANSLATION.md` for complete translation details and comparison.

Run performance comparison:
```bash
cd ../parser_comparison
./compare_parsers.sh
```

## Status

✅ **Translation Complete** - Successfully parses all valid programs
✅ **Tested** - Comprehensive test suite passes
✅ **Documented** - Full comparison available

---

*This C implementation demonstrates that Yacc parsers can be translated to pure C, though with increased complexity.*

