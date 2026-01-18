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
     IDENTIFIER = 258,
     INT_CONST = 259,
     FLOAT_CONST = 260,
     CHAR_CONST = 261,
     STRING_CONST = 262,
     START = 263,
     STOP = 264,
     IF = 265,
     THEN = 266,
     ELSE = 267,
     WHILE = 268,
     READ = 269,
     WRITE = 270,
     INT = 271,
     FLOAT = 272,
     STRING = 273,
     CHAR = 274,
     MEAN = 275,
     STDEV = 276,
     TTEST = 277,
     SAMPLE = 278,
     NORMAL = 279,
     BINOMIAL = 280,
     POISSON = 281,
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
#define IDENTIFIER 258
#define INT_CONST 259
#define FLOAT_CONST 260
#define CHAR_CONST 261
#define STRING_CONST 262
#define START 263
#define STOP 264
#define IF 265
#define THEN 266
#define ELSE 267
#define WHILE 268
#define READ 269
#define WRITE 270
#define INT 271
#define FLOAT 272
#define STRING 273
#define CHAR 274
#define MEAN 275
#define STDEV 276
#define TTEST 277
#define SAMPLE 278
#define NORMAL 279
#define BINOMIAL 280
#define POISSON 281
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
typedef union YYSTYPE
#line 66 "./parser.y"
{
    char* code;
}
/* Line 1529 of yacc.c.  */
#line 153 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

