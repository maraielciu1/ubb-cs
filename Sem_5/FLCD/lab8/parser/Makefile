CC = gcc
# Use bison if available, otherwise yacc
YACC = $(shell command -v bison >/dev/null 2>&1 && echo bison || echo yacc)
CFLAGS = -Wall -g

# Directories
PARSER_DIR = .
OUT_DIR = out

# Source files
PARSER_Y = $(PARSER_DIR)/parser.y
PIF_LEXER_C = $(PARSER_DIR)/pif_lexer.c
SYMBOL_TABLE_C = $(PARSER_DIR)/symbol_table.c

# Target executable
TARGET = parser

.PHONY: all clean test

all: $(TARGET)

# Detect if using bison or yacc and set appropriate file names
BISON_CHECK := $(shell command -v bison >/dev/null 2>&1 && echo yes || echo no)

ifeq ($(BISON_CHECK),yes)
# Use -y flag to make bison generate y.tab.c/h like yacc
PARSER_C = y.tab.c
PARSER_H = y.tab.h
PARSER_O = y.tab.o
YACC_FLAGS = -d -y
else
PARSER_C = y.tab.c
PARSER_H = y.tab.h
PARSER_O = y.tab.o
YACC_FLAGS = -d
endif

OBJS = $(PARSER_O) pif_lexer.o symbol_table.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Generate parser from yacc/bison file
$(PARSER_C) $(PARSER_H): $(PARSER_Y)
	$(YACC) $(YACC_FLAGS) $(PARSER_Y)

# Compile object files
$(PARSER_O): $(PARSER_C)
	$(CC) $(CFLAGS) -c $(PARSER_C) -I$(PARSER_DIR)

pif_lexer.o: $(PIF_LEXER_C) $(PARSER_H)
	$(CC) $(CFLAGS) -c $(PIF_LEXER_C) -I$(PARSER_DIR)

symbol_table.o: $(SYMBOL_TABLE_C)
	$(CC) $(CFLAGS) -c $(SYMBOL_TABLE_C) -I$(PARSER_DIR)

clean:
	rm -f $(TARGET) $(OBJS) $(PARSER_C) $(PARSER_H) parser.tab.c parser.tab.h y.tab.c y.tab.h $(OUT_DIR)/productions.txt $(OUT_DIR)/generated_program.c 2>/dev/null || true

test: $(TARGET)
	./$(TARGET) ../scanner/out/pif.txt
