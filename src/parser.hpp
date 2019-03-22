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
     TDATA = 258,
     TMAIN = 259,
     TLBRACE = 260,
     TRBRACE = 261,
     TSEMI = 262,
     TLPAREN = 263,
     TRPAREN = 264,
     TLBRACKET = 265,
     TRBRACKET = 266,
     TINC = 267,
     TDEC = 268,
     TEQUALS = 269,
     TADD = 270,
     TSUB = 271,
     TMUL = 272,
     TDIV = 273,
     TLESS = 274,
     TISEQUAL = 275,
     TVAR = 276,
     TARRAY = 277,
     TIF = 278,
     TELSE = 279,
     TWHILE = 280,
     TCOMMA = 281,
     TPENUP = 282,
     TPENDOWN = 283,
     TSETPOS = 284,
     TSETCOLOR = 285,
     TFWD = 286,
     TBACK = 287,
     TROT = 288,
     TINTEGER = 289,
     TIDENTIFIER = 290
   };
#endif
/* Tokens.  */
#define TDATA 258
#define TMAIN 259
#define TLBRACE 260
#define TRBRACE 261
#define TSEMI 262
#define TLPAREN 263
#define TRPAREN 264
#define TLBRACKET 265
#define TRBRACKET 266
#define TINC 267
#define TDEC 268
#define TEQUALS 269
#define TADD 270
#define TSUB 271
#define TMUL 272
#define TDIV 273
#define TLESS 274
#define TISEQUAL 275
#define TVAR 276
#define TARRAY 277
#define TIF 278
#define TELSE 279
#define TWHILE 280
#define TCOMMA 281
#define TPENUP 282
#define TPENDOWN 283
#define TSETPOS 284
#define TSETCOLOR 285
#define TFWD 286
#define TBACK 287
#define TROT 288
#define TINTEGER 289
#define TIDENTIFIER 290




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 16 "Procc.y"
{
    Node* node;
	NProgram* program;
	NData* data;
	NDecl* decl;
	NBlock* block;
	NStatement* statement;
	NNumeric* numeric;
	NExpr* expr;
	NComparison* comparison;
	std::string* string;
	int token;
}
/* Line 1529 of yacc.c.  */
#line 133 "/Users/ebrahimalhaddad/pa6-efalhadd/src/parser.hpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE procclval;

