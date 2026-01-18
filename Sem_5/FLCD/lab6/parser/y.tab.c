/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 1 "./parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

extern int yylex(void);
extern int yylineno;
extern char* yytext;
extern FILE* yyin;
extern void init_pif_reader(const char* pif_filename);
extern void cleanup_pif_reader(void);

static FILE* productions_file = NULL;
static int production_count = 0;

static void print_production(const char* rule) {
    if (productions_file) {
        fprintf(productions_file, "%d: %s\n", ++production_count, rule);
        printf("%d: %s\n", production_count, rule);
    }
}

void yyerror(const char* s) {
    const char* token = (yytext && yytext[0]) ? yytext : "EOF";
    const char* kind = (yytext && yytext[0]) ? "unexpected token" : "unexpected end of input";
    fprintf(stderr, "[SYNTAX ERROR] %s '%s' at PIF line %d: %s\n", kind, token, yylineno, s);
}


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 236 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  28
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   240

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  27
/* YYNRULES -- Number of rules.  */
#define YYNRULES  68
/* YYNRULES -- Number of states.  */
#define YYNSTATES  135

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   305

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     7,     9,    12,    14,    16,    18,    20,
      22,    24,    29,    37,    49,    57,    63,    69,    75,    77,
      79,    81,    83,    85,    89,    91,    95,    97,    99,   103,
     105,   108,   110,   112,   114,   116,   120,   124,   128,   130,
     132,   134,   136,   138,   140,   142,   144,   146,   148,   150,
     152,   154,   156,   158,   161,   164,   167,   172,   180,   190,
     200,   205,   207,   209,   211,   213,   217,   219,   221
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      52,     0,    -1,     3,    53,     4,    -1,    54,    -1,    53,
      54,    -1,    55,    -1,    56,    -1,    57,    -1,    58,    -1,
      59,    -1,    71,    -1,    22,    27,    61,    50,    -1,     5,
      42,    65,    43,    46,    53,    47,    -1,     5,    42,    65,
      43,    46,    53,    47,     7,    46,    53,    47,    -1,     8,
      42,    65,    43,    46,    53,    47,    -1,    10,    42,    61,
      43,    50,    -1,     9,    42,    22,    43,    50,    -1,    60,
      22,    27,    61,    50,    -1,    11,    -1,    12,    -1,    14,
      -1,    13,    -1,    62,    -1,    61,    66,    62,    -1,    63,
      -1,    62,    67,    63,    -1,    22,    -1,    64,    -1,    42,
      61,    43,    -1,    74,    -1,    70,    63,    -1,    23,    -1,
      24,    -1,    25,    -1,    26,    -1,    61,    68,    61,    -1,
      65,    69,    65,    -1,    42,    65,    43,    -1,    28,    -1,
      29,    -1,    30,    -1,    31,    -1,    32,    -1,    33,    -1,
      34,    -1,    35,    -1,    36,    -1,    37,    -1,    38,    -1,
      39,    -1,    40,    -1,    28,    -1,    29,    -1,    72,    50,
      -1,    73,    50,    -1,    74,    50,    -1,    22,    41,    77,
      50,    -1,    22,    41,    77,    42,    76,    43,    50,    -1,
      22,    27,    18,    42,    77,    48,    61,    43,    50,    -1,
      22,    27,    18,    42,    22,    48,    61,    43,    50,    -1,
      75,    42,    76,    43,    -1,    15,    -1,    16,    -1,    17,
      -1,    61,    -1,    61,    48,    76,    -1,    19,    -1,    20,
      -1,    21,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    50,    50,    56,    59,    65,    68,    71,    74,    77,
      80,    86,    92,    95,   101,   107,   110,   116,   122,   125,
     128,   131,   137,   140,   146,   149,   155,   158,   161,   164,
     167,   173,   176,   179,   182,   188,   191,   194,   200,   203,
     209,   212,   215,   221,   224,   227,   230,   233,   236,   242,
     245,   251,   254,   260,   263,   266,   272,   275,   281,   284,
     290,   296,   299,   302,   308,   311,   317,   320,   323
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "START", "STOP", "IF", "THEN", "ELSE",
  "WHILE", "READ", "WRITE", "INT", "FLOAT", "STRING", "CHAR", "MEAN",
  "STDEV", "TTEST", "SAMPLE", "NORMAL", "BINOMIAL", "POISSON",
  "IDENTIFIER", "INT_CONST", "FLOAT_CONST", "CHAR_CONST", "STRING_CONST",
  "ASSIGN", "PLUS", "MINUS", "MUL", "DIV", "MOD", "EQ", "NEQ", "LT", "GT",
  "LE", "GE", "AND", "OR", "TILDE", "LPAREN", "RPAREN", "LBRACK", "RBRACK",
  "LBRACE", "RBRACE", "COMMA", "DOT", "SEMICOLON", "$accept", "program",
  "statement_list", "statement", "assignment_statement", "if_statement",
  "while_statement", "io_statement", "declaration_statement", "type",
  "expression", "term", "factor", "constant", "condition",
  "addition_operator", "multiplication_operator", "relational_operator",
  "logical_operator", "sign", "statistical_statement",
  "distribution_statement", "sampling_statement",
  "statistical_function_call", "function_name", "argument_list",
  "distribution_type", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    51,    52,    53,    53,    54,    54,    54,    54,    54,
      54,    55,    56,    56,    57,    58,    58,    59,    60,    60,
      60,    60,    61,    61,    62,    62,    63,    63,    63,    63,
      63,    64,    64,    64,    64,    65,    65,    65,    66,    66,
      67,    67,    67,    68,    68,    68,    68,    68,    68,    69,
      69,    70,    70,    71,    71,    71,    72,    72,    73,    73,
      74,    75,    75,    75,    76,    76,    77,    77,    77
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     1,     2,     1,     1,     1,     1,     1,
       1,     4,     7,    11,     7,     5,     5,     5,     1,     1,
       1,     1,     1,     3,     1,     3,     1,     1,     3,     1,
       2,     1,     1,     1,     1,     3,     3,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     4,     7,     9,     9,
       4,     1,     1,     1,     1,     3,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,    18,    19,    21,
      20,    61,    62,    63,     0,     0,     3,     5,     6,     7,
       8,     9,     0,    10,     0,     0,     0,     0,     1,     0,
       0,     0,     0,     0,     0,     2,     4,     0,    53,    54,
      55,     0,    26,    31,    32,    33,    34,    51,    52,     0,
       0,    22,    24,    27,     0,     0,    29,     0,     0,     0,
       0,     0,     0,    66,    67,    68,     0,     0,    64,     0,
       0,     0,    38,    39,    43,    44,    45,    46,    47,    48,
       0,     0,    40,    41,    42,     0,    49,    50,     0,     0,
      30,     0,     0,     0,     0,     0,    11,     0,    56,     0,
       0,    60,    28,    37,    23,    35,    25,     0,    36,     0,
      16,    15,     0,     0,     0,    17,    65,     0,     0,     0,
       0,     0,    12,    14,     0,     0,    57,     0,     0,     0,
       0,    59,    58,     0,    13
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,    15,    16,    17,    18,    19,    20,    21,    22,
      50,    51,    52,    53,    54,    80,    85,    81,    89,    55,
      23,    24,    25,    56,    27,    69,    66
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -103
static const yytype_int16 yypact[] =
{
      12,   201,    20,   -15,    18,    22,    32,  -103,  -103,  -103,
    -103,  -103,  -103,  -103,   -17,   186,  -103,  -103,  -103,  -103,
    -103,  -103,    50,  -103,    28,    57,    60,    58,  -103,    33,
      33,    90,   151,   128,   105,  -103,  -103,    87,  -103,  -103,
    -103,   151,  -103,  -103,  -103,  -103,  -103,  -103,  -103,    33,
     202,   129,  -103,  -103,   -18,   151,  -103,    26,    72,   151,
     -12,    85,   -20,  -103,  -103,  -103,   -24,   151,    25,    88,
     191,    62,  -103,  -103,  -103,  -103,  -103,  -103,  -103,  -103,
     151,   151,  -103,  -103,  -103,   151,  -103,  -103,    82,    33,
    -103,   102,    99,    66,   108,   101,  -103,   151,  -103,   -16,
     151,  -103,  -103,  -103,   129,     4,  -103,   201,    52,   201,
    -103,  -103,   107,   114,   120,  -103,  -103,    30,    71,   151,
     151,   115,   157,  -103,    68,    70,  -103,   123,   121,   131,
     201,  -103,  -103,   125,  -103
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -103,  -103,  -102,   -14,  -103,  -103,  -103,  -103,  -103,  -103,
     -30,    98,   -49,  -103,   -26,  -103,  -103,  -103,  -103,  -103,
    -103,  -103,  -103,    -1,  -103,   -29,    89
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      26,    36,    60,    62,    57,   117,    90,   118,    72,    73,
      33,    68,    72,    73,    26,     1,    72,    73,    97,    70,
      28,    86,    87,    71,    34,    88,    98,    29,   133,    93,
      96,    94,    72,    73,   115,     3,   106,    99,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    11,    12,
      13,   105,    14,    72,    73,    42,    43,    44,    45,    46,
      30,    47,    48,   108,    31,    86,    87,    68,   114,    91,
      68,   116,    37,   100,    32,    49,     3,   122,    38,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,   124,
     125,    86,    87,    14,    72,    73,    72,    73,    72,    73,
      41,    86,    87,    36,    36,   103,    26,    39,    26,   102,
      40,   128,    58,   129,    67,    92,    26,    26,   123,    36,
      63,    64,    65,   112,    63,    64,    65,    95,   107,    26,
       3,   101,    26,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    11,    12,    13,    61,    14,   109,   110,
      42,    43,    44,    45,    46,   119,    47,    48,   111,    82,
      83,    84,   120,   121,   127,   126,    11,    12,    13,   130,
      59,   131,   134,    42,    43,    44,    45,    46,   104,    47,
      48,   132,     0,     0,   113,     0,     0,     0,     0,     0,
      35,     3,     0,    59,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     3,     0,    14,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    72,
      73,     0,     0,    14,    74,    75,    76,    77,    78,    79,
      72,    73,     0,     0,   102,    74,    75,    76,    77,    78,
      79
};

static const yytype_int16 yycheck[] =
{
       1,    15,    32,    33,    30,   107,    55,   109,    28,    29,
      27,    41,    28,    29,    15,     3,    28,    29,    42,    49,
       0,    39,    40,    49,    41,    43,    50,    42,   130,    59,
      50,    43,    28,    29,    50,     5,    85,    67,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    15,    16,
      17,    81,    22,    28,    29,    22,    23,    24,    25,    26,
      42,    28,    29,    89,    42,    39,    40,    97,    97,    43,
     100,   100,    22,    48,    42,    42,     5,    47,    50,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,   119,
     120,    39,    40,    22,    28,    29,    28,    29,    28,    29,
      42,    39,    40,   117,   118,    43,   107,    50,   109,    43,
      50,    43,    22,    43,    27,    43,   117,   118,    47,   133,
      19,    20,    21,    22,    19,    20,    21,    42,    46,   130,
       5,    43,   133,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    15,    16,    17,    18,    22,    46,    50,
      22,    23,    24,    25,    26,    48,    28,    29,    50,    30,
      31,    32,    48,    43,     7,    50,    15,    16,    17,    46,
      42,    50,    47,    22,    23,    24,    25,    26,    80,    28,
      29,    50,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
       4,     5,    -1,    42,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    -1,    -1,     5,    -1,    22,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    28,
      29,    -1,    -1,    22,    33,    34,    35,    36,    37,    38,
      28,    29,    -1,    -1,    43,    33,    34,    35,    36,    37,
      38
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    52,     5,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    22,    53,    54,    55,    56,    57,
      58,    59,    60,    71,    72,    73,    74,    75,     0,    42,
      42,    42,    42,    27,    41,     4,    54,    22,    50,    50,
      50,    42,    22,    23,    24,    25,    26,    28,    29,    42,
      61,    62,    63,    64,    65,    70,    74,    65,    22,    42,
      61,    18,    61,    19,    20,    21,    77,    27,    61,    76,
      61,    65,    28,    29,    33,    34,    35,    36,    37,    38,
      66,    68,    30,    31,    32,    67,    39,    40,    43,    69,
      63,    43,    43,    61,    43,    42,    50,    42,    50,    61,
      48,    43,    43,    43,    62,    61,    63,    46,    65,    46,
      50,    50,    22,    77,    76,    50,    76,    53,    53,    48,
      48,    43,    47,    47,    61,    61,    50,     7,    43,    43,
      46,    50,    50,    53,    47
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 50 "./parser.y"
    {
        print_production("program -> START statement_list STOP");
    }
    break;

  case 3:
#line 56 "./parser.y"
    {
        print_production("statement_list -> statement");
    }
    break;

  case 4:
#line 59 "./parser.y"
    {
        print_production("statement_list -> statement_list statement");
    }
    break;

  case 5:
#line 65 "./parser.y"
    {
        print_production("statement -> assignment_statement");
    }
    break;

  case 6:
#line 68 "./parser.y"
    {
        print_production("statement -> if_statement");
    }
    break;

  case 7:
#line 71 "./parser.y"
    {
        print_production("statement -> while_statement");
    }
    break;

  case 8:
#line 74 "./parser.y"
    {
        print_production("statement -> io_statement");
    }
    break;

  case 9:
#line 77 "./parser.y"
    {
        print_production("statement -> declaration_statement");
    }
    break;

  case 10:
#line 80 "./parser.y"
    {
        print_production("statement -> statistical_statement");
    }
    break;

  case 11:
#line 86 "./parser.y"
    {
        print_production("assignment_statement -> IDENTIFIER ASSIGN expression SEMICOLON");
    }
    break;

  case 12:
#line 92 "./parser.y"
    {
        print_production("if_statement -> IF LPAREN condition RPAREN LBRACE statement_list RBRACE");
    }
    break;

  case 13:
#line 95 "./parser.y"
    {
        print_production("if_statement -> IF LPAREN condition RPAREN LBRACE statement_list RBRACE ELSE LBRACE statement_list RBRACE");
    }
    break;

  case 14:
#line 101 "./parser.y"
    {
        print_production("while_statement -> WHILE LPAREN condition RPAREN LBRACE statement_list RBRACE");
    }
    break;

  case 15:
#line 107 "./parser.y"
    {
        print_production("io_statement -> WRITE LPAREN expression RPAREN SEMICOLON");
    }
    break;

  case 16:
#line 110 "./parser.y"
    {
        print_production("io_statement -> READ LPAREN IDENTIFIER RPAREN SEMICOLON");
    }
    break;

  case 17:
#line 116 "./parser.y"
    {
        print_production("declaration_statement -> type IDENTIFIER ASSIGN expression SEMICOLON");
    }
    break;

  case 18:
#line 122 "./parser.y"
    {
        print_production("type -> INT");
    }
    break;

  case 19:
#line 125 "./parser.y"
    {
        print_production("type -> FLOAT");
    }
    break;

  case 20:
#line 128 "./parser.y"
    {
        print_production("type -> CHAR");
    }
    break;

  case 21:
#line 131 "./parser.y"
    {
        print_production("type -> STRING");
    }
    break;

  case 22:
#line 137 "./parser.y"
    {
        print_production("expression -> term");
    }
    break;

  case 23:
#line 140 "./parser.y"
    {
        print_production("expression -> expression addition_operator term");
    }
    break;

  case 24:
#line 146 "./parser.y"
    {
        print_production("term -> factor");
    }
    break;

  case 25:
#line 149 "./parser.y"
    {
        print_production("term -> term multiplication_operator factor");
    }
    break;

  case 26:
#line 155 "./parser.y"
    {
        print_production("factor -> IDENTIFIER");
    }
    break;

  case 27:
#line 158 "./parser.y"
    {
        print_production("factor -> constant");
    }
    break;

  case 28:
#line 161 "./parser.y"
    {
        print_production("factor -> LPAREN expression RPAREN");
    }
    break;

  case 29:
#line 164 "./parser.y"
    {
        print_production("factor -> statistical_function_call");
    }
    break;

  case 30:
#line 167 "./parser.y"
    {
        print_production("factor -> sign factor");
    }
    break;

  case 31:
#line 173 "./parser.y"
    {
        print_production("constant -> INT_CONST");
    }
    break;

  case 32:
#line 176 "./parser.y"
    {
        print_production("constant -> FLOAT_CONST");
    }
    break;

  case 33:
#line 179 "./parser.y"
    {
        print_production("constant -> CHAR_CONST");
    }
    break;

  case 34:
#line 182 "./parser.y"
    {
        print_production("constant -> STRING_CONST");
    }
    break;

  case 35:
#line 188 "./parser.y"
    {
        print_production("condition -> expression relational_operator expression");
    }
    break;

  case 36:
#line 191 "./parser.y"
    {
        print_production("condition -> condition logical_operator condition");
    }
    break;

  case 37:
#line 194 "./parser.y"
    {
        print_production("condition -> LPAREN condition RPAREN");
    }
    break;

  case 38:
#line 200 "./parser.y"
    {
        print_production("addition_operator -> PLUS");
    }
    break;

  case 39:
#line 203 "./parser.y"
    {
        print_production("addition_operator -> MINUS");
    }
    break;

  case 40:
#line 209 "./parser.y"
    {
        print_production("multiplication_operator -> MUL");
    }
    break;

  case 41:
#line 212 "./parser.y"
    {
        print_production("multiplication_operator -> DIV");
    }
    break;

  case 42:
#line 215 "./parser.y"
    {
        print_production("multiplication_operator -> MOD");
    }
    break;

  case 43:
#line 221 "./parser.y"
    {
        print_production("relational_operator -> EQ");
    }
    break;

  case 44:
#line 224 "./parser.y"
    {
        print_production("relational_operator -> NEQ");
    }
    break;

  case 45:
#line 227 "./parser.y"
    {
        print_production("relational_operator -> LT");
    }
    break;

  case 46:
#line 230 "./parser.y"
    {
        print_production("relational_operator -> GT");
    }
    break;

  case 47:
#line 233 "./parser.y"
    {
        print_production("relational_operator -> LE");
    }
    break;

  case 48:
#line 236 "./parser.y"
    {
        print_production("relational_operator -> GE");
    }
    break;

  case 49:
#line 242 "./parser.y"
    {
        print_production("logical_operator -> AND");
    }
    break;

  case 50:
#line 245 "./parser.y"
    {
        print_production("logical_operator -> OR");
    }
    break;

  case 51:
#line 251 "./parser.y"
    {
        print_production("sign -> PLUS");
    }
    break;

  case 52:
#line 254 "./parser.y"
    {
        print_production("sign -> MINUS");
    }
    break;

  case 53:
#line 260 "./parser.y"
    {
        print_production("statistical_statement -> distribution_statement SEMICOLON");
    }
    break;

  case 54:
#line 263 "./parser.y"
    {
        print_production("statistical_statement -> sampling_statement SEMICOLON");
    }
    break;

  case 55:
#line 266 "./parser.y"
    {
        print_production("statistical_statement -> statistical_function_call SEMICOLON");
    }
    break;

  case 56:
#line 272 "./parser.y"
    {
        print_production("distribution_statement -> IDENTIFIER TILDE distribution_type SEMICOLON");
    }
    break;

  case 57:
#line 275 "./parser.y"
    {
        print_production("distribution_statement -> IDENTIFIER TILDE distribution_type LPAREN argument_list RPAREN SEMICOLON");
    }
    break;

  case 58:
#line 281 "./parser.y"
    {
        print_production("sampling_statement -> IDENTIFIER ASSIGN SAMPLE LPAREN distribution_type COMMA expression RPAREN SEMICOLON");
    }
    break;

  case 59:
#line 284 "./parser.y"
    {
        print_production("sampling_statement -> IDENTIFIER ASSIGN SAMPLE LPAREN IDENTIFIER COMMA expression RPAREN SEMICOLON");
    }
    break;

  case 60:
#line 290 "./parser.y"
    {
        print_production("statistical_function_call -> function_name LPAREN argument_list RPAREN");
    }
    break;

  case 61:
#line 296 "./parser.y"
    {
        print_production("function_name -> MEAN");
    }
    break;

  case 62:
#line 299 "./parser.y"
    {
        print_production("function_name -> STDEV");
    }
    break;

  case 63:
#line 302 "./parser.y"
    {
        print_production("function_name -> TTEST");
    }
    break;

  case 64:
#line 308 "./parser.y"
    {
        print_production("argument_list -> expression");
    }
    break;

  case 65:
#line 311 "./parser.y"
    {
        print_production("argument_list -> expression COMMA argument_list");
    }
    break;

  case 66:
#line 317 "./parser.y"
    {
        print_production("distribution_type -> NORMAL");
    }
    break;

  case 67:
#line 320 "./parser.y"
    {
        print_production("distribution_type -> BINOMIAL");
    }
    break;

  case 68:
#line 323 "./parser.y"
    {
        print_production("distribution_type -> POISSON");
    }
    break;


/* Line 1267 of yacc.c.  */
#line 2044 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 328 "./parser.y"


int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <pif_file>\n", argv[0]);
        fprintf(stderr, "  PIF file format: TOKEN_NAME: position\n");
        return 1;
    }

    // Initialize PIF reader (reads from PIF file instead of source)
    init_pif_reader(argv[1]);

    productions_file = fopen("out/productions.txt", "w");
    if (!productions_file) {
        fprintf(stderr, "Cannot create productions.txt\n");
        cleanup_pif_reader();
        return 1;
    }

    st_init();

    printf("Starting parsing from PIF file: %s\n", argv[1]);
    printf("Grammar: defined in parser.y\n");
    int result = yyparse();
    
    if (result == 0) {
        printf("\nParsing completed successfully!\n");
        printf("Total productions: %d\n", production_count);
        printf("Productions saved to: out/productions.txt\n");
    } else {
        printf("\nParsing failed!\n");
    }

    fclose(productions_file);
    cleanup_pif_reader();

    return result;
}


