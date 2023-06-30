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
     tINTEGER = 258,
     tOBJECT = 259,
     tFLOAT = 260,
     tSTRING = 261,
     tID = 262,
     tERROR = 263,
     tIF = 264,
     tELSE = 265,
     tELSEIF = 266,
     tENDIF = 267,
     tFOR = 268,
     tIN = 269,
     tENDFOR = 270,
     tRETURN = 271,
     tFORK = 272,
     tENDFORK = 273,
     tWHILE = 274,
     tENDWHILE = 275,
     tTRY = 276,
     tENDTRY = 277,
     tEXCEPT = 278,
     tFINALLY = 279,
     tANY = 280,
     tBREAK = 281,
     tCONTINUE = 282,
     tTO = 283,
     tARROW = 284,
     tMAP = 285,
     tAND = 286,
     tOR = 287,
     tGE = 288,
     tLE = 289,
     tNE = 290,
     tEQ = 291,
     tBITXOR = 292,
     tBITAND = 293,
     tBITOR = 294,
     tBITSHR = 295,
     tBITSHL = 296,
     tUNARYMINUS = 297
   };
#endif
/* Tokens.  */
#define tINTEGER 258
#define tOBJECT 259
#define tFLOAT 260
#define tSTRING 261
#define tID 262
#define tERROR 263
#define tIF 264
#define tELSE 265
#define tELSEIF 266
#define tENDIF 267
#define tFOR 268
#define tIN 269
#define tENDFOR 270
#define tRETURN 271
#define tFORK 272
#define tENDFORK 273
#define tWHILE 274
#define tENDWHILE 275
#define tTRY 276
#define tENDTRY 277
#define tEXCEPT 278
#define tFINALLY 279
#define tANY 280
#define tBREAK 281
#define tCONTINUE 282
#define tTO 283
#define tARROW 284
#define tMAP 285
#define tAND 286
#define tOR 287
#define tGE 288
#define tLE 289
#define tNE 290
#define tEQ 291
#define tBITXOR 292
#define tBITAND 293
#define tBITOR 294
#define tBITSHR 295
#define tBITSHL 296
#define tUNARYMINUS 297




/* Copy the first part of user declarations.  */
#line 1 "src/parser.y"

/******************************************************************************
  Copyright (c) 1992, 1995, 1996 Xerox Corporation.  All rights reserved.
  Portions of this code were written by Stephen White, aka ghond.
  Use and copying of this software and preparation of derivative works based
  upon this software are permitted.  Any distribution of this software or
  derivative works must comply with all applicable United States export
  control laws.  This software is made available AS IS, and Xerox Corporation
  makes no warranty about the software, its performance or its conformity to
  any specification.  Any person obtaining a copy of this software is requested
  to send their name and post office or electronic mail address to:
    Pavel Curtis
    Xerox PARC
    3333 Coyote Hill Rd.
    Palo Alto, CA 94304
    Pavel@Xerox.Com
 *****************************************************************************/

/*************************************************************************/
/* NOTE: If you add an #include here, make sure you properly update the  */
/*       parser.o dependency line in the Makefile.                       */
/*************************************************************************/

#include <ctype.h>
#include "my-math.h"
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "code_gen.h"
#include "config.h"
#include "functions.h"
#include "keywords.h"
#include "list.h"
#include "log.h"
#include "numbers.h"
#include "opcode.h"
#include "parser.h"
#include "program.h"
#include "storage.h"
#include "streams.h"
#include "structures.h"
#include "sym_table.h"
#include "utils.h"
#include "version.h"
#include "waif.h"

static Stmt            *prog_start;
static int              dollars_ok;
static DB_Version       language_version;

static void     error(const char *, const char *);
static void     warning(const char *, const char *);
static int      find_id(char *name);
static void     yyerror(const char *s);
static int      yylex(void);
static Scatter *scatter_from_arglist(Arg_List *);
static Scatter *add_scatter_item(Scatter *, Scatter *);
static void     vet_scatter(Scatter *);
static void     push_loop_name(const char *);
static void     pop_loop_name(void);
static void     suspend_loop_scope(void);
static void     resume_loop_scope(void);

enum loop_exit_kind { LOOP_BREAK, LOOP_CONTINUE };

static void     check_loop_name(const char *, enum loop_exit_kind);


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
typedef union YYSTYPE
#line 69 "src/parser.y"
{
  Stmt         *stmt;
  Expr         *expr;
  Num           integer;
  Objid         object;
  double       real;
  char         *string;
  enum error    error;
  Arg_List     *args;
  Map_List     *map;
  Cond_Arm     *arm;
  Except_Arm   *except;
  Scatter      *scatter;
}
/* Line 193 of yacc.c.  */
#line 263 "/Users/kevin/Desktop/MOO-rescue/toaststunt-release/build/parser.cc"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 276 "/Users/kevin/Desktop/MOO-rescue/toaststunt-release/build/parser.cc"

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
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1898

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  70
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  26
/* YYNRULES -- Number of rules.  */
#define YYNRULES  108
/* YYNRULES -- Number of states.  */
#define YYNSTATES  251

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   297

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    53,     2,     2,    59,    51,     2,    68,
      60,    61,    49,    47,    62,    48,    56,    50,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    57,    64,
      36,    31,    37,    32,    69,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    58,     2,    63,    52,     2,    67,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    65,    33,    66,    54,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    34,    35,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    55
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     9,    18,    19,    29,    30,
      42,    43,    55,    56,    64,    65,    74,    75,    83,    84,
      93,    96,    99,   103,   106,   110,   114,   117,   119,   124,
     130,   131,   138,   139,   142,   145,   146,   151,   157,   158,
     160,   162,   164,   166,   168,   170,   172,   175,   179,   184,
     190,   197,   203,   212,   218,   226,   228,   230,   234,   240,
     245,   249,   253,   257,   261,   265,   269,   273,   277,   281,
     285,   289,   293,   297,   301,   305,   309,   313,   317,   321,
     325,   328,   331,   334,   338,   342,   346,   349,   355,   362,
     363,   365,   367,   368,   371,   375,   381,   382,   384,   386,
     389,   393,   398,   402,   406,   410,   415,   417,   420
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      71,     0,    -1,    72,    -1,    -1,    72,    73,    -1,     9,
      60,    87,    61,    72,    81,    82,    12,    -1,    -1,    13,
       7,    14,    60,    87,    61,    74,    72,    15,    -1,    -1,
      13,     7,    62,     7,    14,    60,    87,    61,    75,    72,
      15,    -1,    -1,    13,     7,    14,    58,    87,    28,    87,
      63,    76,    72,    15,    -1,    -1,    19,    60,    87,    61,
      77,    72,    20,    -1,    -1,    19,     7,    60,    87,    61,
      78,    72,    20,    -1,    -1,    17,    60,    87,    61,    79,
      72,    18,    -1,    -1,    17,     7,    60,    87,    61,    80,
      72,    18,    -1,    87,    64,    -1,    26,    64,    -1,    26,
       7,    64,    -1,    27,    64,    -1,    27,     7,    64,    -1,
      16,    87,    64,    -1,    16,    64,    -1,    64,    -1,    21,
      72,    83,    22,    -1,    21,    72,    24,    72,    22,    -1,
      -1,    81,    11,    60,    87,    61,    72,    -1,    -1,    10,
      72,    -1,    23,    85,    -1,    -1,    83,    23,    84,    85,
      -1,    86,    60,    89,    61,    72,    -1,    -1,     7,    -1,
       3,    -1,     5,    -1,     6,    -1,     4,    -1,     8,    -1,
       7,    -1,    59,     7,    -1,    87,    56,     7,    -1,    87,
      56,    57,     7,    -1,    87,    56,    60,    87,    61,    -1,
      87,    57,     7,    60,    92,    61,    -1,    59,     7,    60,
      92,    61,    -1,    87,    57,    60,    87,    61,    60,    92,
      61,    -1,    87,    58,    88,    87,    63,    -1,    87,    58,
      88,    87,    28,    87,    63,    -1,    52,    -1,    59,    -1,
      87,    31,    87,    -1,    65,    94,    66,    31,    87,    -1,
       7,    60,    92,    61,    -1,    87,    47,    87,    -1,    87,
      48,    87,    -1,    87,    49,    87,    -1,    87,    50,    87,
      -1,    87,    51,    87,    -1,    87,    52,    87,    -1,    87,
      34,    87,    -1,    87,    35,    87,    -1,    87,    44,    87,
      -1,    87,    43,    87,    -1,    87,    42,    87,    -1,    87,
      41,    87,    -1,    87,    40,    87,    -1,    87,    36,    87,
      -1,    87,    39,    87,    -1,    87,    37,    87,    -1,    87,
      38,    87,    -1,    87,    14,    87,    -1,    87,    46,    87,
      -1,    87,    45,    87,    -1,    48,    87,    -1,    53,    87,
      -1,    54,    87,    -1,    60,    87,    61,    -1,    65,    92,
      66,    -1,    58,    91,    63,    -1,    58,    63,    -1,    87,
      32,    87,    33,    87,    -1,    67,    87,    53,    89,    90,
      68,    -1,    -1,    25,    -1,    93,    -1,    -1,    29,    87,
      -1,    87,    30,    87,    -1,    91,    62,    87,    30,    87,
      -1,    -1,    93,    -1,    87,    -1,    69,    87,    -1,    93,
      62,    87,    -1,    93,    62,    69,    87,    -1,    93,    62,
      95,    -1,    94,    62,    95,    -1,    94,    62,     7,    -1,
      94,    62,    69,     7,    -1,    95,    -1,    32,     7,    -1,
      32,     7,    31,    87,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   117,   117,   123,   124,   139,   148,   147,   161,   160,
     176,   175,   189,   188,   201,   200,   213,   212,   225,   224,
     236,   241,   247,   253,   259,   265,   270,   275,   277,   283,
     293,   294,   312,   313,   318,   321,   320,   347,   353,   354,
     359,   364,   369,   374,   379,   384,   389,   398,   405,   417,
     421,   428,   437,   441,   446,   454,   460,   466,   488,   496,
     517,   521,   525,   529,   533,   537,   541,   545,   549,   553,
     557,   561,   565,   569,   573,   577,   581,   585,   589,   593,
     597,   618,   623,   628,   630,   635,   640,   646,   653,   664,
     668,   670,   676,   677,   682,   684,   702,   703,   708,   710,
     712,   726,   743,   752,   756,   761,   766,   771,   775
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "tINTEGER", "tOBJECT", "tFLOAT",
  "tSTRING", "tID", "tERROR", "tIF", "tELSE", "tELSEIF", "tENDIF", "tFOR",
  "tIN", "tENDFOR", "tRETURN", "tFORK", "tENDFORK", "tWHILE", "tENDWHILE",
  "tTRY", "tENDTRY", "tEXCEPT", "tFINALLY", "tANY", "tBREAK", "tCONTINUE",
  "tTO", "tARROW", "tMAP", "'='", "'?'", "'|'", "tAND", "tOR", "'<'",
  "'>'", "tGE", "tLE", "tNE", "tEQ", "tBITXOR", "tBITAND", "tBITOR",
  "tBITSHR", "tBITSHL", "'+'", "'-'", "'*'", "'/'", "'%'", "'^'", "'!'",
  "'~'", "tUNARYMINUS", "'.'", "':'", "'['", "'$'", "'('", "')'", "','",
  "']'", "';'", "'{'", "'}'", "'`'", "'''", "'@'", "$accept", "program",
  "statements", "statement", "@1", "@2", "@3", "@4", "@5", "@6", "@7",
  "elseifs", "elsepart", "excepts", "@8", "except", "opt_id", "expr",
  "dollars_up", "codes", "default", "maplist", "arglist", "ne_arglist",
  "scatter", "scatter_item", 0
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
     285,    61,    63,   124,   286,   287,    60,    62,   288,   289,
     290,   291,   292,   293,   294,   295,   296,    43,    45,    42,
      47,    37,    94,    33,   126,   297,    46,    58,    91,    36,
      40,    41,    44,    93,    59,   123,   125,    96,    39,    64
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    70,    71,    72,    72,    73,    74,    73,    75,    73,
      76,    73,    77,    73,    78,    73,    79,    73,    80,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      81,    81,    82,    82,    83,    84,    83,    85,    86,    86,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    88,
      89,    89,    90,    90,    91,    91,    92,    92,    93,    93,
      93,    93,    94,    94,    94,    94,    94,    95,    95
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     8,     0,     9,     0,    11,
       0,    11,     0,     7,     0,     8,     0,     7,     0,     8,
       2,     2,     3,     2,     3,     3,     2,     1,     4,     5,
       0,     6,     0,     2,     2,     0,     4,     5,     0,     1,
       1,     1,     1,     1,     1,     1,     2,     3,     4,     5,
       6,     5,     8,     5,     7,     1,     1,     3,     5,     4,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     2,     3,     3,     3,     2,     5,     6,     0,
       1,     1,     0,     2,     3,     5,     0,     1,     1,     2,
       3,     4,     3,     3,     3,     4,     1,     2,     4
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     2,     1,    40,    43,    41,    42,    45,    44,
       0,     0,     0,     0,     0,     3,     0,     0,     0,    55,
       0,     0,     0,    56,     0,    27,    96,     0,     4,     0,
      96,     0,     0,    26,     0,     0,     0,     0,     0,     0,
       0,    21,     0,    23,    80,    81,    82,    86,     0,     0,
      46,     0,     0,     0,    98,     0,    97,     0,   106,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    89,    20,     0,    97,     0,     0,
       0,    25,     0,     0,     0,     0,    38,     3,     0,    22,
      24,     0,     0,    85,    96,    83,   107,    99,    84,     0,
       0,     0,     0,    77,    57,     0,    66,    67,    73,    75,
      76,    74,    72,    71,    70,    69,    68,    79,    78,    60,
      61,    62,    63,    64,    65,    47,     0,     0,     0,     0,
       0,    59,     0,     3,     0,     0,     0,     0,    16,     0,
      12,    39,    34,     0,     0,    28,    35,    94,     0,     0,
       0,     0,   100,   102,   104,     0,   103,     0,    90,    92,
      91,     0,    48,     0,    96,     0,     0,    30,     0,     0,
       0,    18,     3,    14,     3,     0,    29,    38,     0,    51,
     108,   101,   105,    58,     0,     0,    87,    49,     0,     0,
       0,    53,    32,     0,     6,     0,     3,     0,     3,     0,
       0,    36,    95,    93,    88,    50,    96,     0,     3,     0,
       0,     0,     3,     0,     0,    17,     0,    13,     3,     0,
      54,    33,     0,     5,    10,     0,     8,    19,    15,    37,
      52,     0,     3,     7,     3,     3,     0,     0,    31,    11,
       9
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    28,   222,   244,   242,   184,   208,   182,
     206,   202,   220,    98,   187,   152,   153,    29,   140,   169,
     195,    49,    55,    87,    57,    58
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -58
static const yytype_int16 yypact[] =
{
     -58,    11,   844,   -58,   -58,   -58,   -58,   -58,   -40,   -58,
     -31,    32,   879,    20,    25,   -58,    10,    14,   944,   -58,
     944,   944,   909,    34,   944,   -58,   211,   944,   -58,   981,
     314,   944,   -11,   -58,  1020,   -10,   944,    21,   944,   399,
      28,   -58,    31,   -58,   -50,   -50,   -50,   -58,  1605,   -38,
      37,  1176,    84,   944,  1801,    33,    36,   -57,   -58,  1644,
     944,   944,   944,   944,   944,   944,   944,   944,   944,   944,
     944,   944,   944,   944,   944,   944,   944,   944,   944,   944,
     944,   944,    19,    27,   -58,   -58,    39,    40,  1215,   -27,
      96,   -58,   944,  1254,   944,  1293,   115,   -58,    24,   -58,
     -58,   944,   944,   -58,   314,   -58,    92,  1801,   -58,   229,
       8,    93,   247,   299,  1801,  1684,    69,    69,   299,   299,
     299,   299,   299,   299,   382,   382,   382,    91,    91,   -14,
     -14,    98,    98,    98,    98,   -58,   123,   944,    73,   944,
     944,   -58,   332,   -58,   944,   944,   121,  1332,   -58,  1371,
     -58,   -58,   -58,    86,   464,   -58,   -58,  1801,  1723,    83,
     944,   944,  1801,   -58,   -58,   144,   -58,   944,   -58,   128,
      40,   944,   -58,  1410,   314,  1449,  1059,   844,  1762,  1488,
     100,   -58,   -58,   -58,   -58,   247,   -58,   115,   944,   -58,
    1801,  1801,   -58,  1801,   944,    90,  1840,   -58,   101,   104,
     944,   -58,    38,   944,   -58,   944,   -58,   529,   -58,   559,
     107,   -58,  1801,  1801,   -58,   -58,   314,  1098,   -58,   105,
     158,  1137,   -58,  1527,   624,   -58,   654,   -58,   -58,   110,
     -58,   844,   944,   -58,   -58,   719,   -58,   -58,   -58,   844,
     -58,  1566,   -58,   -58,   -58,   -58,   749,   814,   844,   -58,
     -58
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -58,   -58,   -15,   -58,   -58,   -58,   -58,   -58,   -58,   -58,
     -58,   -58,   -58,   -58,   -58,   -13,   -58,    -8,   -58,   -12,
     -58,   -58,   -29,   -24,   -58,   -20
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_int16 yytable[] =
{
      39,    86,    56,    89,    34,   110,    82,    83,    84,   111,
      44,     3,    45,    46,    48,   164,    51,    40,    54,    59,
      30,    42,    54,    88,   102,   103,   135,    35,    93,    31,
      95,   144,    37,   145,   138,    78,    79,    80,    81,    32,
      52,    50,    82,    83,    84,   107,   155,   156,   218,   219,
      92,    90,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,    41,   159,   136,   165,    43,   137,
      36,    94,   154,    60,   147,    38,   149,   139,   170,   163,
     166,   106,    99,   157,   158,   100,    54,   104,   109,   108,
     141,   162,   142,   146,    54,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,   151,   160,   167,    82,    83,    84,   177,   173,
     172,   175,   176,   174,   162,   180,   178,   179,    76,    77,
      78,    79,    80,    81,   189,   198,   185,    82,    83,    84,
      81,   192,   190,   191,    82,    83,    84,   194,   214,   193,
     205,   170,   215,   196,   216,   232,    54,   207,   228,   209,
     233,   240,     0,   210,   211,     0,     0,    54,     0,     0,
     212,     0,     0,     0,     0,     0,   213,   229,     0,     0,
       0,   224,   217,   226,     0,   221,     0,   223,     0,     0,
       0,     0,     0,   231,     0,     0,     0,   235,    54,     0,
       0,     0,     0,   239,     4,     5,     6,     7,     8,     9,
       0,     0,     0,     0,   241,     0,     0,   246,     0,   247,
     248,     0,     4,     5,     6,     7,     8,     9,     0,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     9,     0,     0,     0,    18,
       0,    52,     0,    19,    20,    21,     0,     0,     0,    22,
      23,    24,   168,     0,     0,     0,    26,    18,    27,     0,
      53,    19,    20,    21,     0,     0,     0,    22,    23,    24,
       0,     0,     0,     0,    26,    18,    27,     0,   161,    19,
      20,    21,     0,     0,     0,    22,    23,    24,     0,     0,
       0,     0,    26,     0,    27,     0,    53,     4,     5,     6,
       7,     8,     9,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
       9,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,     0,     0,     0,    82,    83,    84,     0,     0,
       0,     0,    18,     0,     0,     0,    19,    20,    21,     0,
       0,     0,    22,    23,    24,     0,     0,     0,     0,    26,
      18,    27,     0,    53,    19,    20,    21,     0,     0,     0,
      22,    23,    24,     0,     0,     0,     0,    26,     0,    27,
       0,   161,     4,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,     0,     0,    12,    13,     0,    14,     0,
      15,     0,    96,    97,     0,    16,    17,    74,    75,    76,
      77,    78,    79,    80,    81,     0,     0,     0,    82,    83,
      84,     0,     0,     0,     0,     0,     0,    18,     0,     0,
       0,    19,    20,    21,     0,     0,     0,    22,    23,    24,
       0,     0,     0,    25,    26,     0,    27,     4,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,     0,     0,
      12,    13,     0,    14,     0,    15,   186,     0,     0,     0,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    18,     0,     0,     0,    19,    20,    21,     0,
       0,     0,    22,    23,    24,     0,     0,     0,    25,    26,
       0,    27,     4,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,     0,     0,    12,    13,   225,    14,     0,
      15,     0,     0,     0,     0,    16,    17,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,     0,     0,    12,    13,    18,    14,   227,
      15,    19,    20,    21,     0,    16,    17,    22,    23,    24,
       0,     0,     0,    25,    26,     0,    27,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    18,     0,     0,
       0,    19,    20,    21,     0,     0,     0,    22,    23,    24,
       0,     0,     0,    25,    26,     0,    27,     4,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,     0,     0,
      12,    13,   237,    14,     0,    15,     0,     0,     0,     0,
      16,    17,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,     0,     0,
      12,    13,    18,    14,   238,    15,    19,    20,    21,     0,
      16,    17,    22,    23,    24,     0,     0,     0,    25,    26,
       0,    27,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    18,     0,     0,     0,    19,    20,    21,     0,
       0,     0,    22,    23,    24,     0,     0,     0,    25,    26,
       0,    27,     4,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,     0,   243,    12,    13,     0,    14,     0,
      15,     0,     0,     0,     0,    16,    17,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,     0,   249,    12,    13,    18,    14,     0,
      15,    19,    20,    21,     0,    16,    17,    22,    23,    24,
       0,     0,     0,    25,    26,     0,    27,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    18,     0,     0,
       0,    19,    20,    21,     0,     0,     0,    22,    23,    24,
       0,     0,     0,    25,    26,     0,    27,     4,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,     0,   250,
      12,    13,     0,    14,     0,    15,     0,     0,     0,     0,
      16,    17,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,     0,     0,
      12,    13,    18,    14,     0,    15,    19,    20,    21,     0,
      16,    17,    22,    23,    24,     0,     0,     0,    25,    26,
       0,    27,     4,     5,     6,     7,     8,     9,     0,     0,
       0,     0,    18,     0,     0,     0,    19,    20,    21,     0,
       0,     0,    22,    23,    24,     0,     0,     0,    25,    26,
       0,    27,     4,     5,     6,     7,     8,     9,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    18,     0,     0,
       0,    19,    20,    21,     0,     0,     0,    22,    23,    24,
       0,     0,     0,    33,    26,     0,    27,     4,     5,     6,
       7,     8,     9,     0,     0,     0,     0,    18,     0,     0,
       0,    19,    20,    21,     0,     0,     0,    22,    23,    24,
       0,     0,    47,     0,    26,     0,    27,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    18,     0,     0,    60,    19,    20,    21,     0,
       0,     0,    22,    23,    24,     0,     0,     0,     0,    26,
       0,    27,    61,    62,     0,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    60,     0,     0,    82,    83,    84,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,    61,    62,     0,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    60,     0,     0,    82,    83,    84,     0,
       0,     0,     0,     0,    91,     0,     0,   200,     0,     0,
      61,    62,     0,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    60,     0,     0,    82,    83,    84,     0,     0,
       0,     0,   201,     0,     0,     0,     0,     0,     0,    61,
      62,     0,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    60,     0,     0,    82,    83,    84,     0,     0,     0,
       0,   230,     0,     0,     0,     0,     0,     0,    61,    62,
       0,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      60,     0,     0,    82,    83,    84,     0,     0,     0,     0,
     234,     0,     0,     0,     0,     0,     0,    61,    62,     0,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    60,
       0,     0,    82,    83,    84,     0,     0,   105,     0,     0,
       0,     0,     0,     0,     0,     0,    61,    62,     0,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    60,     0,
       0,    82,    83,    84,     0,     0,   143,     0,     0,     0,
       0,     0,     0,     0,     0,    61,    62,     0,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    60,     0,     0,
      82,    83,    84,     0,     0,   148,     0,     0,     0,     0,
       0,     0,     0,     0,    61,    62,     0,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    60,     0,     0,    82,
      83,    84,     0,     0,   150,     0,     0,     0,     0,     0,
       0,     0,     0,    61,    62,     0,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    60,     0,     0,    82,    83,
      84,     0,     0,   181,     0,     0,     0,     0,     0,     0,
       0,     0,    61,    62,     0,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    60,     0,     0,    82,    83,    84,
       0,     0,   183,     0,     0,     0,     0,     0,     0,     0,
       0,    61,    62,     0,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    60,     0,     0,    82,    83,    84,     0,
       0,   197,     0,     0,     0,     0,     0,     0,     0,     0,
      61,    62,     0,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    60,     0,     0,    82,    83,    84,     0,     0,
     199,     0,     0,     0,     0,     0,     0,     0,     0,    61,
      62,     0,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    60,     0,     0,    82,    83,    84,     0,     0,   204,
       0,     0,     0,     0,     0,     0,     0,     0,    61,    62,
       0,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      60,     0,     0,    82,    83,    84,     0,     0,   236,     0,
       0,     0,     0,     0,     0,     0,     0,    61,    62,     0,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    60,
       0,     0,    82,    83,    84,     0,     0,   245,     0,     0,
       0,     0,     0,     0,     0,   101,    61,    62,     0,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    60,     0,
       0,    82,    83,    84,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,    62,     0,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,   112,    60,     0,
      82,    83,    84,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,    62,   171,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    60,     0,     0,
      82,    83,    84,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   188,    61,    62,     0,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    60,     0,     0,    82,
      83,    84,     0,     0,     0,     0,     0,     0,     0,     0,
     203,     0,     0,    61,    62,     0,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    60,     0,     0,    82,    83,
      84,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,    62,     0,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    60,     0,     0,    82,    83,    84,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    -1,     0,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,     0,     0,     0,    82,    83,    84
};

static const yytype_int16 yycheck[] =
{
      15,    30,    26,    14,    12,    62,    56,    57,    58,    66,
      18,     0,    20,    21,    22,     7,    24,     7,    26,    27,
      60,     7,    30,    31,    62,    63,     7,     7,    36,    60,
      38,    58,     7,    60,     7,    49,    50,    51,    52,     7,
      32,     7,    56,    57,    58,    53,    22,    23,    10,    11,
      60,    62,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    64,   104,    57,    69,    64,    60,
      60,    60,    97,    14,    92,    60,    94,    60,   112,   109,
     110,     7,    64,   101,   102,    64,   104,    60,    62,    66,
      61,   109,    62,     7,   112,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,     7,    31,    31,    56,    57,    58,   143,   137,
       7,   139,   140,    60,   142,    14,   144,   145,    47,    48,
      49,    50,    51,    52,    61,   174,    60,    56,    57,    58,
      52,     7,   160,   161,    56,    57,    58,    29,    68,   167,
      60,   185,    61,   171,    60,    60,   174,   182,    61,   184,
      12,    61,    -1,   185,   187,    -1,    -1,   185,    -1,    -1,
     188,    -1,    -1,    -1,    -1,    -1,   194,   216,    -1,    -1,
      -1,   206,   200,   208,    -1,   203,    -1,   205,    -1,    -1,
      -1,    -1,    -1,   218,    -1,    -1,    -1,   222,   216,    -1,
      -1,    -1,    -1,   228,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    -1,   232,    -1,    -1,   242,    -1,   244,
     245,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    48,
      -1,    32,    -1,    52,    53,    54,    -1,    -1,    -1,    58,
      59,    60,    25,    -1,    -1,    -1,    65,    48,    67,    -1,
      69,    52,    53,    54,    -1,    -1,    -1,    58,    59,    60,
      -1,    -1,    -1,    -1,    65,    48,    67,    -1,    69,    52,
      53,    54,    -1,    -1,    -1,    58,    59,    60,    -1,    -1,
      -1,    -1,    65,    -1,    67,    -1,    69,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    -1,    -1,    -1,    56,    57,    58,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    52,    53,    54,    -1,
      -1,    -1,    58,    59,    60,    -1,    -1,    -1,    -1,    65,
      48,    67,    -1,    69,    52,    53,    54,    -1,    -1,    -1,
      58,    59,    60,    -1,    -1,    -1,    -1,    65,    -1,    67,
      -1,    69,     3,     4,     5,     6,     7,     8,     9,    -1,
      -1,    -1,    13,    -1,    -1,    16,    17,    -1,    19,    -1,
      21,    -1,    23,    24,    -1,    26,    27,    45,    46,    47,
      48,    49,    50,    51,    52,    -1,    -1,    -1,    56,    57,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    52,    53,    54,    -1,    -1,    -1,    58,    59,    60,
      -1,    -1,    -1,    64,    65,    -1,    67,     3,     4,     5,
       6,     7,     8,     9,    -1,    -1,    -1,    13,    -1,    -1,
      16,    17,    -1,    19,    -1,    21,    22,    -1,    -1,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    52,    53,    54,    -1,
      -1,    -1,    58,    59,    60,    -1,    -1,    -1,    64,    65,
      -1,    67,     3,     4,     5,     6,     7,     8,     9,    -1,
      -1,    -1,    13,    -1,    -1,    16,    17,    18,    19,    -1,
      21,    -1,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,     9,    -1,
      -1,    -1,    13,    -1,    -1,    16,    17,    48,    19,    20,
      21,    52,    53,    54,    -1,    26,    27,    58,    59,    60,
      -1,    -1,    -1,    64,    65,    -1,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    52,    53,    54,    -1,    -1,    -1,    58,    59,    60,
      -1,    -1,    -1,    64,    65,    -1,    67,     3,     4,     5,
       6,     7,     8,     9,    -1,    -1,    -1,    13,    -1,    -1,
      16,    17,    18,    19,    -1,    21,    -1,    -1,    -1,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,     9,    -1,    -1,    -1,    13,    -1,    -1,
      16,    17,    48,    19,    20,    21,    52,    53,    54,    -1,
      26,    27,    58,    59,    60,    -1,    -1,    -1,    64,    65,
      -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    52,    53,    54,    -1,
      -1,    -1,    58,    59,    60,    -1,    -1,    -1,    64,    65,
      -1,    67,     3,     4,     5,     6,     7,     8,     9,    -1,
      -1,    -1,    13,    -1,    15,    16,    17,    -1,    19,    -1,
      21,    -1,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,     9,    -1,
      -1,    -1,    13,    -1,    15,    16,    17,    48,    19,    -1,
      21,    52,    53,    54,    -1,    26,    27,    58,    59,    60,
      -1,    -1,    -1,    64,    65,    -1,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    52,    53,    54,    -1,    -1,    -1,    58,    59,    60,
      -1,    -1,    -1,    64,    65,    -1,    67,     3,     4,     5,
       6,     7,     8,     9,    -1,    -1,    -1,    13,    -1,    15,
      16,    17,    -1,    19,    -1,    21,    -1,    -1,    -1,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,     9,    -1,    -1,    -1,    13,    -1,    -1,
      16,    17,    48,    19,    -1,    21,    52,    53,    54,    -1,
      26,    27,    58,    59,    60,    -1,    -1,    -1,    64,    65,
      -1,    67,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    52,    53,    54,    -1,
      -1,    -1,    58,    59,    60,    -1,    -1,    -1,    64,    65,
      -1,    67,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    52,    53,    54,    -1,    -1,    -1,    58,    59,    60,
      -1,    -1,    -1,    64,    65,    -1,    67,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    52,    53,    54,    -1,    -1,    -1,    58,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    14,    52,    53,    54,    -1,
      -1,    -1,    58,    59,    60,    -1,    -1,    -1,    -1,    65,
      -1,    67,    31,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    14,    -1,    -1,    56,    57,    58,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    14,    -1,    -1,    56,    57,    58,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    28,    -1,    -1,
      31,    32,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    14,    -1,    -1,    56,    57,    58,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    14,    -1,    -1,    56,    57,    58,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      14,    -1,    -1,    56,    57,    58,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    14,
      -1,    -1,    56,    57,    58,    -1,    -1,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    14,    -1,
      -1,    56,    57,    58,    -1,    -1,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    14,    -1,    -1,
      56,    57,    58,    -1,    -1,    61,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    14,    -1,    -1,    56,
      57,    58,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    14,    -1,    -1,    56,    57,
      58,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    14,    -1,    -1,    56,    57,    58,
      -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    14,    -1,    -1,    56,    57,    58,    -1,
      -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    14,    -1,    -1,    56,    57,    58,    -1,    -1,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    14,    -1,    -1,    56,    57,    58,    -1,    -1,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      14,    -1,    -1,    56,    57,    58,    -1,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    14,
      -1,    -1,    56,    57,    58,    -1,    -1,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    31,    32,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    14,    -1,
      -1,    56,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    14,    -1,
      56,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    14,    -1,    -1,
      56,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    31,    32,    -1,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    14,    -1,    -1,    56,
      57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    31,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    14,    -1,    -1,    56,    57,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    14,    -1,    -1,    56,    57,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    -1,    -1,    -1,    56,    57,    58
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    71,    72,     0,     3,     4,     5,     6,     7,     8,
       9,    13,    16,    17,    19,    21,    26,    27,    48,    52,
      53,    54,    58,    59,    60,    64,    65,    67,    73,    87,
      60,    60,     7,    64,    87,     7,    60,     7,    60,    72,
       7,    64,     7,    64,    87,    87,    87,    63,    87,    91,
       7,    87,    32,    69,    87,    92,    93,    94,    95,    87,
      14,    31,    32,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    56,    57,    58,    64,    92,    93,    87,    14,
      62,    64,    60,    87,    60,    87,    23,    24,    83,    64,
      64,    30,    62,    63,    60,    61,     7,    87,    66,    62,
      62,    66,    53,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,     7,    57,    60,     7,    60,
      88,    61,    62,    61,    58,    60,     7,    87,    61,    87,
      61,     7,    85,    86,    72,    22,    23,    87,    87,    92,
      31,    69,    87,    95,     7,    69,    95,    31,    25,    89,
      93,    33,     7,    87,    60,    87,    87,    72,    87,    87,
      14,    61,    79,    61,    77,    60,    22,    84,    30,    61,
      87,    87,     7,    87,    29,    90,    87,    61,    92,    61,
      28,    63,    81,    28,    61,    60,    80,    72,    78,    72,
      89,    85,    87,    87,    68,    61,    60,    87,    10,    11,
      82,    87,    74,    87,    72,    18,    72,    20,    61,    92,
      63,    72,    60,    12,    63,    72,    61,    18,    20,    72,
      61,    87,    76,    15,    75,    61,    72,    72,    72,    15,
      15
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
#line 118 "src/parser.y"
    { prog_start = (yyvsp[(1) - (1)].stmt); }
    break;

  case 3:
#line 123 "src/parser.y"
    { (yyval.stmt) = 0; }
    break;

  case 4:
#line 125 "src/parser.y"
    {
		    if ((yyvsp[(1) - (2)].stmt)) {
			Stmt *tmp = (yyvsp[(1) - (2)].stmt);
			
			while (tmp->next)
			    tmp = tmp->next;
			tmp->next = (yyvsp[(2) - (2)].stmt);
			(yyval.stmt) = (yyvsp[(1) - (2)].stmt);
		    } else
			(yyval.stmt) = (yyvsp[(2) - (2)].stmt);
		}
    break;

  case 5:
#line 140 "src/parser.y"
    {

		    (yyval.stmt) = alloc_stmt(STMT_COND);
		    (yyval.stmt)->s.cond.arms = alloc_cond_arm((yyvsp[(3) - (8)].expr), (yyvsp[(5) - (8)].stmt));
		    (yyval.stmt)->s.cond.arms->next = (yyvsp[(6) - (8)].arm);
		    (yyval.stmt)->s.cond.otherwise = (yyvsp[(7) - (8)].stmt);
		}
    break;

  case 6:
#line 148 "src/parser.y"
    {
		    push_loop_name((yyvsp[(2) - (6)].string));
		}
    break;

  case 7:
#line 152 "src/parser.y"
    {
		    (yyval.stmt) = alloc_stmt(STMT_LIST);
		    (yyval.stmt)->s.list.id = find_id((yyvsp[(2) - (9)].string));
		    (yyval.stmt)->s.list.index = -1;
		    (yyval.stmt)->s.list.expr = (yyvsp[(5) - (9)].expr);
		    (yyval.stmt)->s.list.body = (yyvsp[(8) - (9)].stmt);
		    pop_loop_name();
		}
    break;

  case 8:
#line 161 "src/parser.y"
    {
		    push_loop_name((yyvsp[(2) - (8)].string));
		    push_loop_name((yyvsp[(4) - (8)].string));
		}
    break;

  case 9:
#line 166 "src/parser.y"
    {
		    (yyval.stmt) = alloc_stmt(STMT_LIST);
		    (yyval.stmt)->s.list.id = find_id((yyvsp[(2) - (11)].string));
		    (yyval.stmt)->s.list.index = find_id((yyvsp[(4) - (11)].string));
		    (yyval.stmt)->s.list.expr = (yyvsp[(7) - (11)].expr);
		    (yyval.stmt)->s.list.body = (yyvsp[(10) - (11)].stmt);
		    pop_loop_name();
		    pop_loop_name();
		}
    break;

  case 10:
#line 176 "src/parser.y"
    {
		    push_loop_name((yyvsp[(2) - (8)].string));
		}
    break;

  case 11:
#line 180 "src/parser.y"
    {
		    (yyval.stmt) = alloc_stmt(STMT_RANGE);
		    (yyval.stmt)->s.range.id = find_id((yyvsp[(2) - (11)].string));
		    (yyval.stmt)->s.range.from = (yyvsp[(5) - (11)].expr);
		    (yyval.stmt)->s.range.to = (yyvsp[(7) - (11)].expr);
		    (yyval.stmt)->s.range.body = (yyvsp[(10) - (11)].stmt);
		    pop_loop_name();
		}
    break;

  case 12:
#line 189 "src/parser.y"
    {
		    push_loop_name(0);
		}
    break;

  case 13:
#line 193 "src/parser.y"
    {
		    (yyval.stmt) = alloc_stmt(STMT_WHILE);
		    (yyval.stmt)->s.loop.id = -1;
		    (yyval.stmt)->s.loop.condition = (yyvsp[(3) - (7)].expr);
		    (yyval.stmt)->s.loop.body = (yyvsp[(6) - (7)].stmt);
		    pop_loop_name();
		}
    break;

  case 14:
#line 201 "src/parser.y"
    {
		    push_loop_name((yyvsp[(2) - (5)].string));
		}
    break;

  case 15:
#line 205 "src/parser.y"
    {
		    (yyval.stmt) = alloc_stmt(STMT_WHILE);
		    (yyval.stmt)->s.loop.id = find_id((yyvsp[(2) - (8)].string));
		    (yyval.stmt)->s.loop.condition = (yyvsp[(4) - (8)].expr);
		    (yyval.stmt)->s.loop.body = (yyvsp[(7) - (8)].stmt);
		    pop_loop_name();
		}
    break;

  case 16:
#line 213 "src/parser.y"
    {
		    suspend_loop_scope();
		}
    break;

  case 17:
#line 217 "src/parser.y"
    {
		    (yyval.stmt) = alloc_stmt(STMT_FORK);
		    (yyval.stmt)->s.fork.id = -1;
		    (yyval.stmt)->s.fork.time = (yyvsp[(3) - (7)].expr);
		    (yyval.stmt)->s.fork.body = (yyvsp[(6) - (7)].stmt);
		    resume_loop_scope();
		}
    break;

  case 18:
#line 225 "src/parser.y"
    {
		    suspend_loop_scope();
		}
    break;

  case 19:
#line 229 "src/parser.y"
    {
		    (yyval.stmt) = alloc_stmt(STMT_FORK);
		    (yyval.stmt)->s.fork.id = find_id((yyvsp[(2) - (8)].string));
		    (yyval.stmt)->s.fork.time = (yyvsp[(4) - (8)].expr);
		    (yyval.stmt)->s.fork.body = (yyvsp[(7) - (8)].stmt);
		    resume_loop_scope();
		}
    break;

  case 20:
#line 237 "src/parser.y"
    {
		    (yyval.stmt) = alloc_stmt(STMT_EXPR);
		    (yyval.stmt)->s.expr = (yyvsp[(1) - (2)].expr);
		}
    break;

  case 21:
#line 242 "src/parser.y"
    {
		    check_loop_name(0, LOOP_BREAK);
		    (yyval.stmt) = alloc_stmt(STMT_BREAK);
		    (yyval.stmt)->s.exit = -1;
		}
    break;

  case 22:
#line 248 "src/parser.y"
    {
		    check_loop_name((yyvsp[(2) - (3)].string), LOOP_BREAK);
		    (yyval.stmt) = alloc_stmt(STMT_BREAK);
		    (yyval.stmt)->s.exit = find_id((yyvsp[(2) - (3)].string));
		}
    break;

  case 23:
#line 254 "src/parser.y"
    {
		    check_loop_name(0, LOOP_CONTINUE);
		    (yyval.stmt) = alloc_stmt(STMT_CONTINUE);
		    (yyval.stmt)->s.exit = -1;
		}
    break;

  case 24:
#line 260 "src/parser.y"
    {
		    check_loop_name((yyvsp[(2) - (3)].string), LOOP_CONTINUE);
		    (yyval.stmt) = alloc_stmt(STMT_CONTINUE);
		    (yyval.stmt)->s.exit = find_id((yyvsp[(2) - (3)].string));
		}
    break;

  case 25:
#line 266 "src/parser.y"
    {
		    (yyval.stmt) = alloc_stmt(STMT_RETURN);
		    (yyval.stmt)->s.expr = (yyvsp[(2) - (3)].expr);
		}
    break;

  case 26:
#line 271 "src/parser.y"
    {
		    (yyval.stmt) = alloc_stmt(STMT_RETURN);
		    (yyval.stmt)->s.expr = 0;
		}
    break;

  case 27:
#line 276 "src/parser.y"
    { (yyval.stmt) = 0; }
    break;

  case 28:
#line 278 "src/parser.y"
    {
		    (yyval.stmt) = alloc_stmt(STMT_TRY_EXCEPT);
		    (yyval.stmt)->s._catch.body = (yyvsp[(2) - (4)].stmt);
		    (yyval.stmt)->s._catch.excepts = (yyvsp[(3) - (4)].except);
		}
    break;

  case 29:
#line 284 "src/parser.y"
    {
		    (yyval.stmt) = alloc_stmt(STMT_TRY_FINALLY);
		    (yyval.stmt)->s.finally.body = (yyvsp[(2) - (5)].stmt);
		    (yyval.stmt)->s.finally.handler = (yyvsp[(4) - (5)].stmt);
		}
    break;

  case 30:
#line 293 "src/parser.y"
    { (yyval.arm) = 0; }
    break;

  case 31:
#line 295 "src/parser.y"
    {
		    Cond_Arm *this_arm = alloc_cond_arm((yyvsp[(4) - (6)].expr), (yyvsp[(6) - (6)].stmt));
		    
		    if ((yyvsp[(1) - (6)].arm)) {
		        Cond_Arm *tmp = (yyvsp[(1) - (6)].arm);

			while (tmp->next)
			    tmp = tmp->next;
			tmp->next = this_arm;
			(yyval.arm) = (yyvsp[(1) - (6)].arm);
		    } else
			(yyval.arm) = this_arm;
		}
    break;

  case 32:
#line 312 "src/parser.y"
    { (yyval.stmt) = 0; }
    break;

  case 33:
#line 314 "src/parser.y"
    { (yyval.stmt) = (yyvsp[(2) - (2)].stmt); }
    break;

  case 34:
#line 319 "src/parser.y"
    { (yyval.except) = (yyvsp[(2) - (2)].except); }
    break;

  case 35:
#line 321 "src/parser.y"
    {
		    Except_Arm *tmp = (yyvsp[(1) - (2)].except);
		    int        count = 1;
		    
		    while (tmp->next) {
			tmp = tmp->next;
			count++;
		    }
		    if (!tmp->codes)
			yyerror("Unreachable EXCEPT clause");
		    else if (count > 255)
			yyerror("Too many EXCEPT clauses (max. 255)");
		}
    break;

  case 36:
#line 335 "src/parser.y"
    {
		    Except_Arm *tmp = (yyvsp[(1) - (4)].except);
		    
		    while (tmp->next)
			tmp = tmp->next;

		    tmp->next = (yyvsp[(4) - (4)].except);
		    (yyval.except) = (yyvsp[(1) - (4)].except);
		}
    break;

  case 37:
#line 348 "src/parser.y"
    { (yyval.except) = alloc_except((yyvsp[(1) - (5)].string) ? find_id((yyvsp[(1) - (5)].string)) : -1, (yyvsp[(3) - (5)].args), (yyvsp[(5) - (5)].stmt)); }
    break;

  case 38:
#line 353 "src/parser.y"
    { (yyval.string) = 0; }
    break;

  case 39:
#line 355 "src/parser.y"
    { (yyval.string) = (yyvsp[(1) - (1)].string); }
    break;

  case 40:
#line 360 "src/parser.y"
    {
		    (yyval.expr) = alloc_var(TYPE_INT);
		    (yyval.expr)->e.var.v.num = (yyvsp[(1) - (1)].integer);
		}
    break;

  case 41:
#line 365 "src/parser.y"
    {
		    (yyval.expr) = alloc_var(TYPE_FLOAT);
		    (yyval.expr)->e.var.v.fnum = (yyvsp[(1) - (1)].real);
		}
    break;

  case 42:
#line 370 "src/parser.y"
    {
		    (yyval.expr) = alloc_var(TYPE_STR);
		    (yyval.expr)->e.var.v.str = (yyvsp[(1) - (1)].string);
		}
    break;

  case 43:
#line 375 "src/parser.y"
    {
		    (yyval.expr) = alloc_var(TYPE_OBJ);
		    (yyval.expr)->e.var.v.obj = (yyvsp[(1) - (1)].object);
		}
    break;

  case 44:
#line 380 "src/parser.y"
    {
		    (yyval.expr) = alloc_var(TYPE_ERR);
		    (yyval.expr)->e.var.v.err = (yyvsp[(1) - (1)].error);
		}
    break;

  case 45:
#line 385 "src/parser.y"
    {
		    (yyval.expr) = alloc_expr(EXPR_ID);
		    (yyval.expr)->e.id = find_id((yyvsp[(1) - (1)].string));
		}
    break;

  case 46:
#line 390 "src/parser.y"
    {
		    /* Treat $foo like #0.("foo") */
		    Expr *obj = alloc_var(TYPE_OBJ);
		    Expr *prop = alloc_var(TYPE_STR);
		    obj->e.var.v.obj = 0;
		    prop->e.var.v.str = (yyvsp[(2) - (2)].string);
		    (yyval.expr) = alloc_binary(EXPR_PROP, obj, prop);
		}
    break;

  case 47:
#line 399 "src/parser.y"
    {
		    /* Treat foo.bar like foo.("bar") for simplicity */
		    Expr *prop = alloc_var(TYPE_STR);
		    prop->e.var.v.str = (yyvsp[(3) - (3)].string);
		    (yyval.expr) = alloc_binary(EXPR_PROP, (yyvsp[(1) - (3)].expr), prop);
		}
    break;

  case 48:
#line 406 "src/parser.y"
    {
            /* Treat foo.:bar (waif properties) like foo.(":bar") 
               (we should be using  WAIF_PROP_PREFIX here...) */
		    Expr *prop = alloc_var(TYPE_STR);
			char *newstr;
            asprintf(&newstr, "%c%s", WAIF_PROP_PREFIX, (yyvsp[(4) - (4)].string));
			dealloc_string((yyvsp[(4) - (4)].string));
		    prop->e.var.v.str = alloc_string(newstr);
			free(newstr);
		    (yyval.expr) = alloc_binary(EXPR_PROP, (yyvsp[(1) - (4)].expr), prop);
		}
    break;

  case 49:
#line 418 "src/parser.y"
    {
		    (yyval.expr) = alloc_binary(EXPR_PROP, (yyvsp[(1) - (5)].expr), (yyvsp[(4) - (5)].expr));
		}
    break;

  case 50:
#line 422 "src/parser.y"
    {
		    /* treat foo:bar(args) like foo:("bar")(args) */
		    Expr *verb = alloc_var(TYPE_STR);
		    verb->e.var.v.str = (yyvsp[(3) - (6)].string);
		    (yyval.expr) = alloc_verb((yyvsp[(1) - (6)].expr), verb, (yyvsp[(5) - (6)].args));
		}
    break;

  case 51:
#line 429 "src/parser.y"
    {
		    /* treat $bar(args) like #0:("bar")(args) */
		    Expr *obj = alloc_var(TYPE_OBJ);
		    Expr *verb = alloc_var(TYPE_STR);
		    obj->e.var.v.obj = 0;
		    verb->e.var.v.str = (yyvsp[(2) - (5)].string);
		    (yyval.expr) = alloc_verb(obj, verb, (yyvsp[(4) - (5)].args));
		}
    break;

  case 52:
#line 438 "src/parser.y"
    {
		    (yyval.expr) = alloc_verb((yyvsp[(1) - (8)].expr), (yyvsp[(4) - (8)].expr), (yyvsp[(7) - (8)].args));
		}
    break;

  case 53:
#line 442 "src/parser.y"
    {
		    dollars_ok--;
		    (yyval.expr) = alloc_binary(EXPR_INDEX, (yyvsp[(1) - (5)].expr), (yyvsp[(4) - (5)].expr));
		}
    break;

  case 54:
#line 447 "src/parser.y"
    {
		    dollars_ok--;
		    (yyval.expr) = alloc_expr(EXPR_RANGE);
		    (yyval.expr)->e.range.base = (yyvsp[(1) - (7)].expr);
		    (yyval.expr)->e.range.from = (yyvsp[(4) - (7)].expr);
		    (yyval.expr)->e.range.to = (yyvsp[(6) - (7)].expr);
		}
    break;

  case 55:
#line 455 "src/parser.y"
    {
		    if (!dollars_ok)
			yyerror("Illegal context for `^' expression.");
		    (yyval.expr) = alloc_expr(EXPR_FIRST);
		}
    break;

  case 56:
#line 461 "src/parser.y"
    {
		    if (!dollars_ok)
			yyerror("Illegal context for `$' expression.");
		    (yyval.expr) = alloc_expr(EXPR_LAST);
		}
    break;

  case 57:
#line 467 "src/parser.y"
    {
		    Expr *e = (yyvsp[(1) - (3)].expr);

		    if (e->kind == EXPR_LIST) {
			e->kind = EXPR_SCATTER;
			if (e->e.list) {
			    e->e.scatter = scatter_from_arglist(e->e.list);
			    vet_scatter(e->e.scatter);
			} else
			    yyerror("Empty list in scattering assignment.");
		    } else {
			if (e->kind == EXPR_RANGE)
			    e = e->e.range.base;
			while (e->kind == EXPR_INDEX)
			    e = e->e.bin.lhs;
			if (e->kind != EXPR_ID  &&  e->kind != EXPR_PROP)
			    yyerror("Illegal expression on left side of"
				    " assignment.");
		    }
		    (yyval.expr) = alloc_binary(EXPR_ASGN, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
	        }
    break;

  case 58:
#line 489 "src/parser.y"
    {
		    Expr       *e = alloc_expr(EXPR_SCATTER);

		    e->e.scatter = (yyvsp[(2) - (5)].scatter);
		    vet_scatter((yyvsp[(2) - (5)].scatter));
		    (yyval.expr) = alloc_binary(EXPR_ASGN, e, (yyvsp[(5) - (5)].expr));
		}
    break;

  case 59:
#line 497 "src/parser.y"
    {
		    unsigned f_no;

		    (yyval.expr) = alloc_expr(EXPR_CALL);
		    if ((f_no = number_func_by_name((yyvsp[(1) - (4)].string))) == FUNC_NOT_FOUND) {
			/* Replace with call_function("$1", @args) */
			Expr           *fname = alloc_var(TYPE_STR);
			Arg_List       *a = alloc_arg_list(ARG_NORMAL, fname);

			fname->e.var.v.str = (yyvsp[(1) - (4)].string);
			a->next = (yyvsp[(3) - (4)].args);
			warning("Unknown built-in function: ", (yyvsp[(1) - (4)].string));
			(yyval.expr)->e.call.func = number_func_by_name("call_function");
			(yyval.expr)->e.call.args = a;
		    } else {
			(yyval.expr)->e.call.func = f_no;
			(yyval.expr)->e.call.args = (yyvsp[(3) - (4)].args);
			dealloc_string((yyvsp[(1) - (4)].string));
		    }
		}
    break;

  case 60:
#line 518 "src/parser.y"
    {
		    (yyval.expr) = alloc_binary(EXPR_PLUS, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		}
    break;

  case 61:
#line 522 "src/parser.y"
    {
		    (yyval.expr) = alloc_binary(EXPR_MINUS, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		}
    break;

  case 62:
#line 526 "src/parser.y"
    {
		    (yyval.expr) = alloc_binary(EXPR_TIMES, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		}
    break;

  case 63:
#line 530 "src/parser.y"
    {
		    (yyval.expr) = alloc_binary(EXPR_DIVIDE, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		}
    break;

  case 64:
#line 534 "src/parser.y"
    {
		    (yyval.expr) = alloc_binary(EXPR_MOD, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		}
    break;

  case 65:
#line 538 "src/parser.y"
    {
		    (yyval.expr) = alloc_binary(EXPR_EXP, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		}
    break;

  case 66:
#line 542 "src/parser.y"
    {
		    (yyval.expr) = alloc_binary(EXPR_AND, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		}
    break;

  case 67:
#line 546 "src/parser.y"
    {
		    (yyval.expr) = alloc_binary(EXPR_OR, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		}
    break;

  case 68:
#line 550 "src/parser.y"
    {
		    (yyval.expr) = alloc_binary(EXPR_BITOR, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		}
    break;

  case 69:
#line 554 "src/parser.y"
    {
		    (yyval.expr) = alloc_binary(EXPR_BITAND, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		}
    break;

  case 70:
#line 558 "src/parser.y"
    {
		    (yyval.expr) = alloc_binary(EXPR_BITXOR, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		}
    break;

  case 71:
#line 562 "src/parser.y"
    {
		    (yyval.expr) = alloc_binary(EXPR_EQ, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		}
    break;

  case 72:
#line 566 "src/parser.y"
    {
		    (yyval.expr) = alloc_binary(EXPR_NE, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		}
    break;

  case 73:
#line 570 "src/parser.y"
    {
		    (yyval.expr) = alloc_binary(EXPR_LT, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		}
    break;

  case 74:
#line 574 "src/parser.y"
    {
		    (yyval.expr) = alloc_binary(EXPR_LE, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		}
    break;

  case 75:
#line 578 "src/parser.y"
    {
		    (yyval.expr) = alloc_binary(EXPR_GT, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		}
    break;

  case 76:
#line 582 "src/parser.y"
    {
		    (yyval.expr) = alloc_binary(EXPR_GE, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		}
    break;

  case 77:
#line 586 "src/parser.y"
    {
		    (yyval.expr) = alloc_binary(EXPR_IN, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		}
    break;

  case 78:
#line 590 "src/parser.y"
    {
		    (yyval.expr) = alloc_binary(EXPR_BITSHL, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		}
    break;

  case 79:
#line 594 "src/parser.y"
    {
		    (yyval.expr) = alloc_binary(EXPR_BITSHR, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		}
    break;

  case 80:
#line 598 "src/parser.y"
    {
		    if ((yyvsp[(2) - (2)].expr)->kind == EXPR_VAR
			&& ((yyvsp[(2) - (2)].expr)->e.var.type == TYPE_INT
			    || (yyvsp[(2) - (2)].expr)->e.var.type == TYPE_FLOAT)) {
			switch ((yyvsp[(2) - (2)].expr)->e.var.type) {
			  case TYPE_INT:
			    (yyvsp[(2) - (2)].expr)->e.var.v.num = -(yyvsp[(2) - (2)].expr)->e.var.v.num;
			    break;
			  case TYPE_FLOAT:
			    (yyvsp[(2) - (2)].expr)->e.var.v.fnum = -(yyvsp[(2) - (2)].expr)->e.var.v.fnum;
			    break;
			  default:
			    break;
			}
		        (yyval.expr) = (yyvsp[(2) - (2)].expr);
		    } else {
			(yyval.expr) = alloc_expr(EXPR_NEGATE);
			(yyval.expr)->e.expr = (yyvsp[(2) - (2)].expr);
		    }
		}
    break;

  case 81:
#line 619 "src/parser.y"
    {
		    (yyval.expr) = alloc_expr(EXPR_NOT);
		    (yyval.expr)->e.expr = (yyvsp[(2) - (2)].expr);
		}
    break;

  case 82:
#line 624 "src/parser.y"
    {
		    (yyval.expr) = alloc_expr(EXPR_COMPLEMENT);
		    (yyval.expr)->e.expr = (yyvsp[(2) - (2)].expr);
		}
    break;

  case 83:
#line 629 "src/parser.y"
    { (yyval.expr) = (yyvsp[(2) - (3)].expr); }
    break;

  case 84:
#line 631 "src/parser.y"
    {
		    (yyval.expr) = alloc_expr(EXPR_LIST);
		    (yyval.expr)->e.list = (yyvsp[(2) - (3)].args);
		}
    break;

  case 85:
#line 636 "src/parser.y"
    {
		    (yyval.expr) = alloc_expr(EXPR_MAP);
		    (yyval.expr)->e.map = (yyvsp[(2) - (3)].map);
		}
    break;

  case 86:
#line 641 "src/parser.y"
    {
		    /* [] is the expression for an empty map */
		    (yyval.expr) = alloc_expr(EXPR_MAP);
		    (yyval.expr)->e.map = 0;
		}
    break;

  case 87:
#line 647 "src/parser.y"
    {
		    (yyval.expr) = alloc_expr(EXPR_COND);
		    (yyval.expr)->e.cond.condition = (yyvsp[(1) - (5)].expr);
		    (yyval.expr)->e.cond.consequent = (yyvsp[(3) - (5)].expr);
		    (yyval.expr)->e.cond.alternate = (yyvsp[(5) - (5)].expr);
		}
    break;

  case 88:
#line 654 "src/parser.y"
    {
		    (yyval.expr) = alloc_expr(EXPR_CATCH);
		    (yyval.expr)->e._catch._try = (yyvsp[(2) - (6)].expr);
		    (yyval.expr)->e._catch.codes = (yyvsp[(4) - (6)].args);
		    (yyval.expr)->e._catch.except = (yyvsp[(5) - (6)].expr);
		}
    break;

  case 89:
#line 664 "src/parser.y"
    { dollars_ok++; }
    break;

  case 90:
#line 669 "src/parser.y"
    { (yyval.args) = 0; }
    break;

  case 91:
#line 671 "src/parser.y"
    { (yyval.args) = (yyvsp[(1) - (1)].args); }
    break;

  case 92:
#line 676 "src/parser.y"
    { (yyval.expr) = 0; }
    break;

  case 93:
#line 678 "src/parser.y"
    { (yyval.expr) = (yyvsp[(2) - (2)].expr); }
    break;

  case 94:
#line 683 "src/parser.y"
    { (yyval.map) = alloc_map_list((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 95:
#line 685 "src/parser.y"
    {
		    Map_List *this_map = alloc_map_list((yyvsp[(3) - (5)].expr), (yyvsp[(5) - (5)].expr));

		    if ((yyvsp[(1) - (5)].map)) {
			Map_List *tmp = (yyvsp[(1) - (5)].map);

			while (tmp->next)
			    tmp = tmp->next;
			tmp->next = this_map;
			(yyval.map) = (yyvsp[(1) - (5)].map);
		    } else
			(yyval.map) = this_map;
		}
    break;

  case 96:
#line 702 "src/parser.y"
    { (yyval.args) = 0; }
    break;

  case 97:
#line 704 "src/parser.y"
    { (yyval.args) = (yyvsp[(1) - (1)].args); }
    break;

  case 98:
#line 709 "src/parser.y"
    { (yyval.args) = alloc_arg_list(ARG_NORMAL, (yyvsp[(1) - (1)].expr)); }
    break;

  case 99:
#line 711 "src/parser.y"
    { (yyval.args) = alloc_arg_list(ARG_SPLICE, (yyvsp[(2) - (2)].expr)); }
    break;

  case 100:
#line 713 "src/parser.y"
    {
		    Arg_List *this_arg = alloc_arg_list(ARG_NORMAL, (yyvsp[(3) - (3)].expr));

		    if ((yyvsp[(1) - (3)].args)) {
			Arg_List *tmp = (yyvsp[(1) - (3)].args);

			while (tmp->next)
			    tmp = tmp->next;
			tmp->next = this_arg;
			(yyval.args) = (yyvsp[(1) - (3)].args);
		    } else
			(yyval.args) = this_arg;
		}
    break;

  case 101:
#line 727 "src/parser.y"
    {
		    Arg_List *this_arg = alloc_arg_list(ARG_SPLICE, (yyvsp[(4) - (4)].expr));

		    if ((yyvsp[(1) - (4)].args)) {
			Arg_List *tmp = (yyvsp[(1) - (4)].args);

			while (tmp->next)
			    tmp = tmp->next;
			tmp->next = this_arg;
			(yyval.args) = (yyvsp[(1) - (4)].args);
		    } else
			(yyval.args) = this_arg;
		}
    break;

  case 102:
#line 744 "src/parser.y"
    {
		    Scatter    *sc = scatter_from_arglist((yyvsp[(1) - (3)].args));

		    if (sc)
			(yyval.scatter) = add_scatter_item(sc, (yyvsp[(3) - (3)].scatter));
		    else
			(yyval.scatter) = (yyvsp[(3) - (3)].scatter);
		}
    break;

  case 103:
#line 753 "src/parser.y"
    {
		    (yyval.scatter) = add_scatter_item((yyvsp[(1) - (3)].scatter), (yyvsp[(3) - (3)].scatter));
		}
    break;

  case 104:
#line 757 "src/parser.y"
    {
		    (yyval.scatter) = add_scatter_item((yyvsp[(1) - (3)].scatter), alloc_scatter(SCAT_REQUIRED,
							    find_id((yyvsp[(3) - (3)].string)), 0));
		}
    break;

  case 105:
#line 762 "src/parser.y"
    {
		    (yyval.scatter) = add_scatter_item((yyvsp[(1) - (4)].scatter), alloc_scatter(SCAT_REST,
							    find_id((yyvsp[(4) - (4)].string)), 0));
		}
    break;

  case 106:
#line 767 "src/parser.y"
    {   (yyval.scatter) = (yyvsp[(1) - (1)].scatter);  }
    break;

  case 107:
#line 772 "src/parser.y"
    {
		    (yyval.scatter) = alloc_scatter(SCAT_OPTIONAL, find_id((yyvsp[(2) - (2)].string)), 0);
		}
    break;

  case 108:
#line 776 "src/parser.y"
    {
		    (yyval.scatter) = alloc_scatter(SCAT_OPTIONAL, find_id((yyvsp[(2) - (4)].string)), (yyvsp[(4) - (4)].expr));
		}
    break;


/* Line 1267 of yacc.c.  */
#line 2950 "/Users/kevin/Desktop/MOO-rescue/toaststunt-release/build/parser.cc"
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


#line 781 "src/parser.y"


static int              lineno, nerrors, must_rename_keywords;
static Parser_Client    client;
static void            *client_data;
static Names           *local_names;

static int
find_id(char *name)
{
    int slot = find_or_add_name(&local_names, name);

    dealloc_string(name);
    return slot;
}

static void
yyerror(const char *s)
{
    error(s, 0);
}

static const char *
fmt_error(const char *s, const char *t)
{
    static Stream      *str = 0;

    if (str == 0)
	str = new_stream(100);
    if (t)
	stream_printf(str, "Line %d:  %s%s", lineno, s, t);
    else
	stream_printf(str, "Line %d:  %s", lineno, s);
    return reset_stream(str);
}

static void
error(const char *s, const char *t)
{
    nerrors++;
    (*(client.error))(client_data, fmt_error(s, t));
}

static void
warning(const char *s, const char *t)
{
    if (client.warning)
	(*(client.warning))(client_data, fmt_error(s, t));
    else
	error(s, t);
}

static int unget_buffer[5], unget_count;

static int
lex_getc(void)
{
    if (unget_count > 0)
	return unget_buffer[--unget_count];
    else
	return (*(client.getch))(client_data);
}

static void
lex_ungetc(int c)
{
    unget_buffer[unget_count++] = c;
}

static int
follow(int expect, int ifyes, int ifno)     /* look ahead for >=, etc. */
{
    int c = lex_getc();

    if (c == expect)
	return ifyes;
    lex_ungetc(c);
    return ifno;
}

static int
check_two_dots(void)     /* look ahead for .. but don't consume */
{
    int c1 = lex_getc();
    int c2 = lex_getc();

    lex_ungetc(c2);
    lex_ungetc(c1);

    return c1 == '.' && c2 == '.';
}

static Stream  *token_stream = 0;

static int
yylex(void)
{
    int c;

    reset_stream(token_stream);

start_over:

    do {
	c = lex_getc();
	if (c == '\n') lineno++;
    } while (isspace(c));

    if (c == '/') {
	c = lex_getc();
	if (c == '*') {
	    for (;;) {
		c = lex_getc();
		if (c == '*') {
		    c = lex_getc();
		    if (c == '/')
			goto start_over;
		}
		if (c == EOF) {
		    yyerror("End of program while in a comment");
		    return c;
		}
	    }
	} else {
	    lex_ungetc(c);
	    return '/';
	}
    }

    if (c == '#') {
	int            negative = 0;
	Objid          oid = 0;

	c = lex_getc();
	if (c == '-') {
	    negative = 1;
	    c = lex_getc();
	}
	if (!isdigit(c)) {
	    yyerror("Malformed object number");
	    lex_ungetc(c);
	    return 0;
	}
	do {
	    oid = oid * 10 + (c - '0');
	    c = lex_getc();
	} while (isdigit(c));
	lex_ungetc(c);

	yylval.object = negative ? -oid : oid;
	return tOBJECT;
    }

    if (isdigit(c) || (c == '.'  &&  language_version >= DBV_Float)) {
	Num	n = 0;
	int	type = tINTEGER;

	while (isdigit(c)) {
	    n = n * 10 + (c - '0');
	    stream_add_char(token_stream, c);
	    c = lex_getc();
	}

	if (language_version >= DBV_Float && c == '.') {
	    /* maybe floating-point (but maybe `..') */
	    int cc;

	    lex_ungetc(cc = lex_getc()); /* peek ahead */
	    if (isdigit(cc)) {  /* definitely floating-point */
		type = tFLOAT;
		do {
		    stream_add_char(token_stream, c);
		    c = lex_getc();
		} while (isdigit(c));
	    } else if (stream_length(token_stream) == 0) {
		/* no digits before or after `.'; not a number at all */
		goto normal_dot;
	    } else if (cc != '.') {
		/* some digits before dot, not `..' */
		type = tFLOAT;
		stream_add_char(token_stream, c);
		c = lex_getc();
	    }
	}

	if (language_version >= DBV_Float && (c == 'e' || c == 'E')) {
	    /* better be an exponent */
	    type = tFLOAT;
	    stream_add_char(token_stream, c);
	    c = lex_getc();
	    if (c == '+' || c == '-') {
		stream_add_char(token_stream, c);
		c = lex_getc();
	    }
	    if (!isdigit(c)) {
		yyerror("Malformed floating-point literal");
		lex_ungetc(c);
		return 0;
	    }
	    do {
		stream_add_char(token_stream, c);
		c = lex_getc();
	    } while (isdigit(c));
	}
	
	lex_ungetc(c);

	if (type == tINTEGER)
	    yylval.integer = n;
	else {
	    double	d;
	    
	    d = strtod(reset_stream(token_stream), 0);
	    if (!IS_REAL(d)) {
		yyerror("Floating-point literal out of range");
		d = 0.0;
	    }
	    yylval.real = d; 
	}
	return type;
    }
    
    if (isalpha(c) || c == '_') {
	char	       *buf;
	Keyword	       *k;

	stream_add_char(token_stream, c);
	while (isalnum(c = lex_getc()) || c == '_')
	    stream_add_char(token_stream, c);
	lex_ungetc(c);
	buf = reset_stream(token_stream);

	k = find_keyword(buf);
	if (k) {
	    if (k->version <= language_version) {
		int	t = k->token;

		if (t == tERROR)
		    yylval.error = k->error;
		return t;
	    } else {  /* New keyword being used as an identifier */
		if (!must_rename_keywords)
		    warning("Renaming old use of new keyword: ", buf);
		must_rename_keywords = 1;
	    }
	}
	
	yylval.string = alloc_string(buf);
	return tID;
    }

    if (c == '"') {
	while(1) {
	    c = lex_getc();
	    if (c == '"')
		break;
	    if (c == '\\')
		c = lex_getc();
	    if (c == '\n' || c == EOF) {
		yyerror("Missing quote");
		break;
	    }
	    stream_add_char(token_stream, c);
	}
	yylval.string = alloc_string(reset_stream(token_stream));
	return tSTRING;
    }

    switch(c) {
      case '^':         return check_two_dots() ? '^'
			     : follow('.', tBITXOR, '^');
      case '>':         return follow('>', 1, 0) ? tBITSHR
			     : follow('=', tGE, '>');
      case '<':         return follow('<', 1, 0) ? tBITSHL
			     : follow('=', tLE, '<');
      case '=':         return follow('=', 1, 0) ? tEQ
			     : follow('>', tARROW, '=');
      case '|':         return follow('.', 1, 0) ? tBITOR
			     : follow('|', tOR, '|');
      case '&':         return follow('.', 1, 0) ? tBITAND
			     : follow('&', tAND, '&');
      case '-':         return follow('>', tMAP, '-');
      case '!':         return follow('=', tNE, '!');
      normal_dot:
      case '.':         return follow('.', tTO, '.');
      default:          return c;
    }
}

static Scatter *
add_scatter_item(Scatter *first, Scatter *last)
{
    Scatter    *tmp = first;

    while (tmp->next)
	tmp = tmp->next;
    tmp->next = last;

    return first;
}

static Scatter *
scatter_from_arglist(Arg_List *a)
{
    Scatter    *sc = 0, **scp;
    Arg_List   *anext;

    for (scp = &sc; a; a = anext, scp = &((*scp)->next)) {
	if (a->expr->kind == EXPR_ID) {
	    *scp = alloc_scatter(a->kind == ARG_NORMAL ? SCAT_REQUIRED
						       : SCAT_REST,
				 a->expr->e.id, 0);
	    anext = a->next;
	    dealloc_node(a->expr);
	    dealloc_node(a);
	} else {
	    yyerror("Scattering assignment targets must be simple variables.");
	    return 0;
	}
    }

    return sc;
}

static void
vet_scatter(Scatter *sc)
{
    int seen_rest = 0, count = 0;

    for (; sc; sc = sc->next) {
	if (sc->kind == SCAT_REST) {
	    if (seen_rest)
		yyerror("More than one `@' target in scattering assignment.");
	    else
		seen_rest = 1;
	}
	count++;
    }

    if (count > 255)
	yyerror("Too many targets in scattering assignment.");
}

struct loop_entry {
    struct loop_entry  *next;
    const char         *name;
    int                 is_barrier;
};

static struct loop_entry *loop_stack;

static void
push_loop_name(const char *name)
{
    struct loop_entry *entry = (struct loop_entry *)mymalloc(sizeof(struct loop_entry), M_AST);

    entry->next = loop_stack;
    entry->name = (name ? str_dup(name) : 0);
    entry->is_barrier = 0;
    loop_stack = entry;
}

static void
pop_loop_name(void)
{
    if (!loop_stack)
	errlog("PARSER: Empty loop stack in POP_LOOP_NAME!\n");
    else if (loop_stack->is_barrier)
	errlog("PARSER: Tried to pop loop-scope barrier!\n");
    else {
	struct loop_entry      *entry = loop_stack;

	loop_stack = loop_stack->next;
	if (entry->name)
	    free_str(entry->name);
	myfree(entry, M_AST);
    }
}

static void
suspend_loop_scope(void)
{
    struct loop_entry *entry = (struct loop_entry *)mymalloc(sizeof(struct loop_entry), M_AST);

    entry->next = loop_stack;
    entry->name = 0;
    entry->is_barrier = 1;
    loop_stack = entry;
}

static void
resume_loop_scope(void)
{
    if (!loop_stack)
	errlog("PARSER: Empty loop stack in RESUME_LOOP_SCOPE!\n");
    else if (!loop_stack->is_barrier)
	errlog("PARSER: Tried to resume non-loop-scope barrier!\n");
    else {
	struct loop_entry      *entry = loop_stack;

	loop_stack = loop_stack->next;
	myfree(entry, M_AST);
    }
}

static void
check_loop_name(const char *name, enum loop_exit_kind kind)
{
    struct loop_entry  *entry;

    if (!name) {
	if (!loop_stack  ||  loop_stack->is_barrier) {
	    if (kind == LOOP_BREAK)
		yyerror("No enclosing loop for `break' statement");
	    else
		yyerror("No enclosing loop for `continue' statement");
	}
	return;
    }

    for (entry = loop_stack; entry && !entry->is_barrier; entry = entry->next)
	if (entry->name  &&  strcasecmp(entry->name, name) == 0)
	    return;

    if (kind == LOOP_BREAK)
	error("Invalid loop name in `break' statement: ", name);
    else
	error("Invalid loop name in `continue' statement: ", name);
}

Program *
parse_program(DB_Version version, Parser_Client c, void *data)
{
    extern int  yyparse();
    Program    *prog;

    if (token_stream == 0)
	token_stream = new_stream(1024);
    unget_count = 0;
    nerrors = 0;
    must_rename_keywords = 0;
    lineno = 1;
    client = c;
    client_data = data;
    local_names = new_builtin_names(version);
    dollars_ok = 0; /* true when the special symbols `^' and `$' are valid */
    loop_stack = 0;
    language_version = version;

    begin_code_allocation();
    yyparse();
    end_code_allocation(nerrors > 0);
    if (loop_stack) {
	if (nerrors == 0)
	    errlog("PARSER: Non-empty loop-scope stack!\n");
	while (loop_stack) {
	    struct loop_entry *entry = loop_stack;

	    loop_stack = loop_stack->next;
	    if (entry->name)
		free_str(entry->name);
	    myfree(entry, M_AST);
	}
    }

    if (nerrors == 0) {
	if (must_rename_keywords) {
	    /* One or more new keywords were used as identifiers in this code,
	     * possibly as local variable names (but maybe only as property or
	     * verb names).  Such local variables must be renamed to avoid a
	     * conflict in the new world.  We just add underscores to the end
	     * until it stops conflicting with any other local variable.
	     */
	    unsigned i;

	    for (i = first_user_slot(version); i < local_names->size; i++) {
		const char	*name = local_names->names[i];
	    
		if (find_keyword(name)) { /* Got one... */
		    stream_add_string(token_stream, name);
		    do {
			stream_add_char(token_stream, '_');
		    } while (find_name(local_names,
				       stream_contents(token_stream)) >= 0);
		    free_str(name);
		    local_names->names[i] =
			str_dup(reset_stream(token_stream));
		}
	    }
	}

	prog = generate_code(prog_start, version);
	prog->num_var_names = local_names->size;
	prog->var_names = local_names->names;

	myfree(local_names, M_NAMES);
	free_stmt(prog_start);

	return prog;
    } else {
	free_names(local_names);
	return 0;
    }
}

struct parser_state {
    Var         code;           /* a list of strings */
    int         cur_string;     /* which string? */
    int         cur_char;       /* which character in that string? */
    Var         errors;         /* a list of strings */
};

static void
my_error(void *data, const char *msg)
{
    struct parser_state *state = (struct parser_state *) data;
    Var                 v;

    v.type = TYPE_STR;
    v.v.str = str_dup(msg);
    state->errors = listappend(state->errors, v);
}

static int
my_getc(void *data)
{
    struct parser_state *state = (struct parser_state *) data;
    Var                 code;
    char                c;

    code = state->code;
    if (task_timed_out  ||  state->cur_string > code.v.list[0].v.num)
	return EOF;
    else if (!(c = code.v.list[state->cur_string].v.str[state->cur_char])) {
	state->cur_string++;
	state->cur_char = 0;
	return '\n';
    } else {
	state->cur_char++;
	return c;
    }
}

static Parser_Client list_parser_client = { my_error, 0, my_getc };

Program *
parse_list_as_program(Var code, Var *errors)
{
    struct parser_state state;
    Program            *program;

    state.code = code;
    state.cur_string = 1;
    state.cur_char = 0;
    state.errors = new_list(0);
    program = parse_program(current_db_version, list_parser_client, &state);
    *errors = state.errors;

    return program;
}

