CC = gcc
CFLAGS = -Wall -g -std=c99

# Requirement 2: Mini-DSL parser (main program)
TARGET = ll1_parser
REQ1_TARGET = requirement1

SRCS = main.c ll1_parser.c pif_reader.c
REQ1_SRCS = requirement1.c ll1_parser.c arithmetic_grammar.c

OBJS = $(SRCS:.c=.o)
REQ1_OBJS = $(REQ1_SRCS:.c=.o)

.PHONY: all clean test req1 req2

all: $(TARGET) $(REQ1_TARGET)

# Requirement 2: Mini-DSL grammar with PIF input
$(TARGET): main.o ll1_parser.o pif_reader.o
	$(CC) $(CFLAGS) -o $(TARGET) $^

# Requirement 1: Arithmetic grammar with sequence
$(REQ1_TARGET): requirement1.o ll1_parser.o arithmetic_grammar.o pif_reader.o
	$(CC) $(CFLAGS) -o $(REQ1_TARGET) $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(REQ1_TARGET) *.o out/*.txt

# Test Requirement 1
req1: $(REQ1_TARGET)
	./$(REQ1_TARGET)

# Test Requirement 2
req2: $(TARGET)
	./$(TARGET) ../../lab6/scanner/out/pif.txt

test: req1 req2

