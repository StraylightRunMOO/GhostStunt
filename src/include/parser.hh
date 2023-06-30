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
/* Line 1529 of yacc.c.  */
#line 148 "/Users/kevin/Desktop/MOO-rescue/straylight-moo/build3/parser.hh"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

