/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_YY_FIR_PARSER_TAB_H_INCLUDED
# define YY_YY_FIR_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    tINTEGER = 258,
    tFLOAT = 259,
    tIDENTIFIER = 260,
    tSTRING = 261,
    tWHILE = 262,
    tIF = 263,
    tRESTART = 264,
    tFINALLY = 265,
    tWRITE = 266,
    tWRITELN = 267,
    tLEAVE = 268,
    tRETURN = 269,
    tELSE = 270,
    tBIG = 271,
    tARROW = 272,
    tDO = 273,
    tNULL = 274,
    tVOID_TYPE = 275,
    tINT_TYPE = 276,
    tFLOAT_TYPE = 277,
    tSTRING_TYPE = 278,
    tSIZEOF = 279,
    tOR = 280,
    tAND = 281,
    tFOR = 282,
    tOTHERWISE = 283,
    tTHEN = 284,
    tGE = 285,
    tLE = 286,
    tEQ = 287,
    tNE = 288,
    tUNARY = 289
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 17 "fir_parser.y" /* yacc.c:1909  */

  //--- don't change *any* of these: if you do, you'll break the compiler.
  YYSTYPE() : type(cdk::primitive_type::create(0, cdk::TYPE_VOID)) {}
  ~YYSTYPE() {}
  YYSTYPE(const YYSTYPE &other) { *this = other; }
  YYSTYPE& operator=(const YYSTYPE &other) { type = other.type; return *this; }

  std::shared_ptr<cdk::basic_type> type;        /* expression type */
  //-- don't change *any* of these --- END!

  int                   i;	/* integer value */
  double                j;   
  std::string          *s;	/* symbol name or string literal */
  cdk::basic_node      *node;	/* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */


  cdk::lvalue_node     *lvalue;
  fir::block_node       *block;
  fir::return_node *return_value;

#line 112 "fir_parser.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (std::shared_ptr<cdk::compiler> compiler);

#endif /* !YY_YY_FIR_PARSER_TAB_H_INCLUDED  */
