/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     START = 258,
     STOP = 259,
     IF = 260,
     THEN = 261,
     ELSE = 262,
     WHILE = 263,
     READ = 264,
     WRITE = 265,
     INT = 266,
     FLOAT = 267,
     STRING = 268,
     CHAR = 269,
     MEAN = 270,
     STDEV = 271,
     TTEST = 272,
     SAMPLE = 273,
     NORMAL = 274,
     BINOMIAL = 275,
     POISSON = 276,
     IDENTIFIER = 277,
     INT_CONST = 278,
     FLOAT_CONST = 279,
     CHAR_CONST = 280,
     STRING_CONST = 281,
     ASSIGN = 282,
     PLUS = 283,
     MINUS = 284,
     MUL = 285,
     DIV = 286,
     MOD = 287,
     EQ = 288,
     NEQ = 289,
     LT = 290,
     GT = 291,
     LE = 292,
     GE = 293,
     AND = 294,
     OR = 295,
     TILDE = 296,
     LPAREN = 297,
     RPAREN = 298,
     LBRACK = 299,
     RBRACK = 300,
     LBRACE = 301,
     RBRACE = 302,
     COMMA = 303,
     DOT = 304,
     SEMICOLON = 305
   };
#endif
/* Tokens.  */
#define START 258
#define STOP 259
#define IF 260
#define THEN 261
#define ELSE 262
#define WHILE 263
#define READ 264
#define WRITE 265
#define INT 266
#define FLOAT 267
#define STRING 268
#define CHAR 269
#define MEAN 270
#define STDEV 271
#define TTEST 272
#define SAMPLE 273
#define NORMAL 274
#define BINOMIAL 275
#define POISSON 276
#define IDENTIFIER 277
#define INT_CONST 278
#define FLOAT_CONST 279
#define CHAR_CONST 280
#define STRING_CONST 281
#define ASSIGN 282
#define PLUS 283
#define MINUS 284
#define MUL 285
#define DIV 286
#define MOD 287
#define EQ 288
#define NEQ 289
#define LT 290
#define GT 291
#define LE 292
#define GE 293
#define AND 294
#define OR 295
#define TILDE 296
#define LPAREN 297
#define RPAREN 298
#define LBRACK 299
#define RBRACK 300
#define LBRACE 301
#define RBRACE 302
#define COMMA 303
#define DOT 304
#define SEMICOLON 305




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

