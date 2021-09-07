%option c++ prefix="fir_scanner_" outfile="fir_scanner.cpp"
%option stack noyywrap yylineno 8bit debug
%{ 
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include <sstream>
static std::ostringstream strlit;

#include "fir_parser.tab.h"



// don't change this
#define yyerror LexerError
%}
%x X_STRING X_COMMENT X_GO X_STOP 
%%

   //  {yydebug=1; set_debug(true);}

"!!".*                 ;


"(*"                   yy_push_state(X_COMMENT);
<X_COMMENT>"*)"        yy_pop_state();
<X_COMMENT>.|"\n"      ;                        // no nested comments



">="                   return tGE; 
"<="                   return tLE; 
"=="                   return tEQ; 
"!="                   return tNE; 
"&&"                   return tAND;
"||"                   return tOR; 

"while"                return tWHILE; 
"if"                   return tIF; 
"else"                 return tELSE;  
"then"                 return tTHEN;
"restart"              return tRESTART;
"finally"              return tFINALLY;
"write"                return tWRITE;
"writeln"              return tWRITELN;
"leave"                return tLEAVE;
"return"               return tRETURN;
"do"                   return tDO;
"null"                 return tNULL;
"sizeof"               return tSIZEOF;
"void"                 return tVOID_TYPE;
"int"                  return tINT_TYPE;
"float"                return tFLOAT_TYPE;
"string"               return tSTRING_TYPE;
">>"                   return tBIG;
"->"                   return tARROW;
"for"                  return tFOR;
"otherwise"            return "tOTHERWISE";
"run"                  return "tRUN";
".."                   return "tDOT";


[A-Za-z][A-Za-z0-9_]*  yylval.s = new std::string(yytext); return tIDENTIFIER;


\'                  yy_push_state(X_STRING);
<X_STRING>\'    {
                    yylval.s = new std::string(strlit.str());
                    strlit.str("");
                    yy_pop_state(); 
                    return tSTRING;
                }
<X_STRING>~     yy_push_state(X_GO);
<X_STRING>\\n   strlit << '\n';
<X_STRING>\\t   strlit << '\t';
<X_STRING>\\r   strlit << '\r';
<X_STRING>.     strlit << *yytext;

<X_GO>0                            yy_push_state(X_STOP);
<X_GO>n                            strlit << '\n'; yy_pop_state();
<X_GO>r                            strlit << '\r'; yy_pop_state();
<X_GO>t                            strlit << '\t'; yy_pop_state();
<X_GO>\'                           strlit << '\''; yy_pop_state();
<X_GO>~                            strlit << '~'; yy_pop_state();
<X_GO>[0-9A-Fa-f][0-9A-Fa-f]?      strlit << std::string(1, std::strtol(yytext, nullptr, 16)) ; yy_pop_state(); 


<X_STOP>\'                             {
                                            yy_pop_state(); 
                                            yy_pop_state();
                                            yylval.s = new std::string(strlit.str());
                                            strlit.str("");
                                            yy_pop_state();  
                                            return tSTRING;
                                        }
<X_STOP>\\\'                            ;                   
<X_STOP>.|\n                           ;




[1-9][0-9]*|0                           {
                                            try{
                                                yylval.i = strtol(yytext, nullptr, 10); 
                                            } catch(const std::out_of_range& oor){
                                                yyerror("Lexical error: overflow");
                                            }
                                            return tINTEGER;
                                        }


0[0-7]+                                 {
                                            try {
                                                yylval.i = std::stoi(yytext, nullptr, 8); 
                                            } catch (const std::out_of_range& oor) {
                                                yyerror("Lexical error: overflow");
                                            }
                                            return tINTEGER;
                                        }



[0-9]+([Ee][-+]?[0-9]+)                 {
                                            try {
                                                std::string::size_type sz;
                                                yylval.i = std::stod(yytext, &sz); 
                                            } catch (const std::out_of_range& oor) {
                                                 yyerror("Lexical error: overflow");
                                            }
                                            return tFLOAT;
                                        }


[0-9]*\.[0-9]+([Ee]([-+])?[0-9]+)?      {
                                            try {
                                                std::string::size_type sz;
                                                yylval.i = std::stod(yytext, &sz); 
                                            } catch (const std::out_of_range& oor) {
                                                yyerror("Lexical error: overflow");
                                            }
                                            return tFLOAT;
                                        }


[0-9]+\.[0-9]*([Ee]([-+])?[0-9]+)?      {
                                            try {
                                                std::string::size_type sz;
                                                yylval.i = std::stod(yytext, &sz); 
                                            } catch (const std::out_of_range& oor) {
                                                yyerror("Lexical error: overflow");
                                            }
                                            return tFLOAT;
                                        }


[-()<>=+*/%;{}.?@,~[\]]       return *yytext;

[ \t\n\r]+               ; /* ignore whitespace */

.                      yyerror("Unknown character");


%%
