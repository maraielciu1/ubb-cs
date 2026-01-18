# Parser Comparison Results

**Quick Summary**: Both parsers work! Yacc is 5% faster, LL(1) provides more detail.

## How to Run

```bash
./compare_parsers.sh
```

## Results (100 iterations each)

| Parser | Time | Speed | Productions | Output |
|--------|------|-------|-------------|---------|
| **LL(1)** | 0.189s | 528 parses/sec | 36 | Parse tree (61 nodes) |
| **Yacc** | 0.180s | 557 parses/sec | 24 | C code (28 lines) |

**Winner**: Yacc (1.05x faster)

## Test Program

```
START
    INT !x = 5;
    INT !y = 10;
    !x = !x + !y;
STOP
```

## Key Differences

### LL(1) Parser (Lab 7)
- ✅ Explicit parse tree with 61 nodes
- ✅ Complete derivation sequence (36 steps)
- ✅ Educational/analysis tool
- ✅ Better error messages
- ❌ No code generation
- ❌ More complex (1,400 lines)

### Yacc Parser (Lab 8)
- ✅ Generates executable C code
- ✅ Production-ready compiler
- ✅ Simple (365 lines of grammar)
- ✅ 5% faster
- ❌ No explicit parse tree
- ❌ Generic error messages

## Files

- `compare_parsers.sh` - Comparison script
- `valid_test.src` - Test program
- `valid_pif.txt` - Program Internal Form
- `out/ll1_productions.txt` - LL(1) productions
- `out/ll1_parse_tree.txt` - LL(1) parse tree
- `out/yacc_generated.c` - Generated C code
- `out/yacc_productions.txt` - Yacc productions

## Recommendation

- **Production use**: Yacc Parser ✅
- **Learning/Research**: LL(1) Parser ✅

See `../PARSER_COMPARISON_FINAL.md` for detailed analysis.
