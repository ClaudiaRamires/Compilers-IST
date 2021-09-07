%{
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <algorithm>
#include <memory>
#include <cstring>
#include <cdk/compiler.h>
#include <cdk/types/types.h>
#include "ast/all.h"
#define LINE                         compiler->scanner()->lineno()
#define yylex()                      compiler->scanner()->scan()
#define yyerror(compiler, s)         compiler->scanner()->error(s)
//-- don't change *any* of these --- END!
%}

%parse-param {std::shared_ptr<cdk::compiler> compiler}

%union {
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
};

%token <i> tINTEGER 
%token <j> tFLOAT
%token <s> tIDENTIFIER tSTRING
%token tWHILE tIF tRESTART tFINALLY tWRITE tWRITELN tLEAVE tRETURN tELSE
%token tBIG tARROW tDO tNULL tVOID_TYPE tINT_TYPE tFLOAT_TYPE tSTRING_TYPE tSIZEOF tOR tAND 
%token tFOR tOTHERWISE tRUN tDOT

%nonassoc tIF 
%nonassoc tTHEN tDO
%nonassoc tELSE tFINALLY

%right '='
%left tGE tLE tEQ tNE tAND tOR '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY '?' 
%nonassoc '[' 
%nonassoc '~' '@'

%type <node> declaration instruction block_declaration function prologo var variable else conditional_instruction iteraction_instruction finally
%type <sequence>  file declarations  instructions expression  block_declarations body variables  
%type <expression> expr literals function_call
%type <type> type 
%type<return_value> default_return_value
%type <block> block epilogo 
%type <s> string
%type <lvalue> lval


%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%


file : declarations { compiler->ast($$ = $1); }
     ;


declarations : declaration                    { $$ = new cdk::sequence_node(LINE, $1); }
             | declarations declaration       { $$ = new cdk::sequence_node(LINE, $2, $1); } 
             ;


declaration : var ';'                         { $$ = $1; }
            | function                        { $$ = $1; }
            ;

var         : type  '*'  tIDENTIFIER  '=' expr      { $$ = new fir::declaration_variable_node(LINE,$1, 1,*$3,$5); }
            | type  '?'  tIDENTIFIER  '=' expr      { $$ = new fir::declaration_variable_node(LINE,$1, 2,*$3,$5); }
            | type       tIDENTIFIER  '=' expr      { $$ = new fir::declaration_variable_node(LINE,$1, 0,*$2,$4); }
            | type  '*'  tIDENTIFIER                { $$ = new fir::declaration_variable_node(LINE,$1, 1,*$3,nullptr); }
            | type  '?'  tIDENTIFIER                { $$ = new fir::declaration_variable_node(LINE,$1, 2,*$3,nullptr); }
            | type       tIDENTIFIER                { $$ = new fir::declaration_variable_node(LINE,$1, 0,*$2,nullptr); }
            ;
        

function     : type  '?'  tIDENTIFIER '('  variables  ')'  default_return_value   body   { $$ = new fir::function_def_node(LINE,$1,1,*$3,$5,$7,$8); }
             | type  '*'  tIDENTIFIER '('  variables  ')'  default_return_value   body   { $$ = new fir::function_def_node(LINE,$1,2,*$3,$5,$7,$8); }
             | type       tIDENTIFIER '('  variables  ')'  default_return_value   body   { $$ = new fir::function_def_node(LINE,$1,0,*$2,$4,$6,$7); }
             | type  '?'  tIDENTIFIER '('             ')'  default_return_value   body   { $$ = new fir::function_def_node(LINE,$1,1,*$3,nullptr,$6,$7); }
             | type  '*'  tIDENTIFIER '('             ')'  default_return_value   body   { $$ = new fir::function_def_node(LINE,$1,2,*$3,nullptr,$6,$7); }
             | type       tIDENTIFIER '('             ')'  default_return_value   body   { $$ = new fir::function_def_node(LINE,$1,0,*$2,nullptr,$5,$6); }
             | type  '?'  tIDENTIFIER '('  variables  ')'                         body   { $$ = new fir::function_def_node(LINE,$1,1,*$3,$5,nullptr,$7); }
             | type  '*'  tIDENTIFIER '('  variables  ')'                         body   { $$ = new fir::function_def_node(LINE,$1,2,*$3,$5,nullptr,$7); }
             | type       tIDENTIFIER '('  variables  ')'                         body   { $$ = new fir::function_def_node(LINE,$1,0,*$2,$4,nullptr,$6); }
             | type  '?'  tIDENTIFIER '('             ')'                         body   { $$ = new fir::function_def_node(LINE,$1,1,*$3,nullptr,nullptr,$6); }
             | type  '*'  tIDENTIFIER '('             ')'                         body   { $$ = new fir::function_def_node(LINE,$1,2,*$3,nullptr,nullptr,$6); }
             | type       tIDENTIFIER '('             ')'                         body   { $$ = new fir::function_def_node(LINE,$1,0,*$2,nullptr,nullptr,$5); }
             ;

default_return_value : tARROW literals { $$ = new fir::return_node(LINE); }
                     ;  

literals             :                 { $$ = nullptr; }
                     | tINTEGER        { $$ = new cdk::integer_node (LINE, $1); }
                     ;


variables            : variable                 { $$ = new cdk::sequence_node(LINE,$1); }
                     | variables ',' variable   { $$ = new cdk::sequence_node(LINE, $3, $1); } 
                     ;

variable :type  tIDENTIFIER               {$$ = new fir::declaration_variable_node(LINE,$1, 0,*$2,nullptr);}
         ;

body  : prologo                      {$$ = new cdk::sequence_node(LINE,nullptr,new cdk::sequence_node(LINE,nullptr,new cdk::sequence_node(LINE,$1))); }
      |         block                {$$ = new cdk::sequence_node(LINE,nullptr,new cdk::sequence_node(LINE,$1,new cdk::sequence_node(LINE,nullptr))); }
      |               epilogo        {$$ = new cdk::sequence_node(LINE,$1,new cdk::sequence_node(LINE,nullptr,new cdk::sequence_node(LINE,nullptr))); }
      | prologo block epilogo        {$$ = new cdk::sequence_node(LINE,$3,new cdk::sequence_node(LINE,$2,new cdk::sequence_node(LINE,$1))); }
      |                              {$$ = new cdk::sequence_node(LINE,nullptr,new cdk::sequence_node(LINE,nullptr,new cdk::sequence_node(LINE,nullptr))); }
      ;


prologo : '@' block                   {$$ = new fir::prologo_node(LINE,$2); }
        ;

epilogo : tBIG block                  {$$ = $2; }
        ;


block : '{' block_declarations instructions   '}'  { $$ = new fir::block_node(LINE, $2, $3); } 
      | '{' block_declarations                '}'  { $$ = new fir::block_node(LINE, $2, nullptr); } 
      | '{'                    instructions   '}'  { $$ = new fir::block_node(LINE, nullptr, $2); } 
      | '{'                                   '}'  { $$ = new fir::block_node(LINE, nullptr, nullptr); } 
      ;         

block_declarations : block_declaration                                               { $$ = new cdk::sequence_node(LINE, $1); } 
                   | block_declarations block_declaration                            { $$ = new cdk::sequence_node(LINE, $2, $1); }  
                   ;
     
block_declaration : type     tIDENTIFIER '=' expr            ';'                { $$ = new fir::declaration_variable_node(LINE,$1, 0,*$2,$4); }
                  | type     tIDENTIFIER                     ';'                { $$ = new fir::declaration_variable_node(LINE,$1, 0,*$2,nullptr); }
                  |          tIDENTIFIER     function_call                      { $$ = new fir::declaration_variable_node(LINE,nullptr, 0,*$1,$2); }
                  ; 

instructions : instruction                          { $$ = new cdk::sequence_node(LINE, $1); }
             | instructions instruction             { $$ = new cdk::sequence_node(LINE, $2, $1); }
             ;
 

instruction : expr ';'                                                     { $$ = new fir::evaluation_node(LINE, $1); }
            | tWRITE   expression  ';'                                     { $$ = new fir::write_node(LINE, $2, false); }
            | tWRITELN expression  ';'                                     { $$ = new fir::write_node(LINE, $2, true); }
            | tLEAVE tINTEGER     ';'                                      { $$ = new fir::leave_node(LINE, $2); }
            | tLEAVE              ';'                                      { $$ = new fir::leave_node(LINE, 0); }
            | tRESTART tINTEGER   ';'                                      { $$ = new fir::restart_node(LINE, $2); }
            | tRESTART            ';'                                      { $$ = new fir::restart_node(LINE, 0); }
            | tRETURN                                                      { $$ = new fir::return_node(LINE); }
            | conditional_instruction                                      { $$ = $1; }
            | iteraction_instruction                                       { $$ = $1; }
            | block                                                        { $$ = $1; } 
            | tFOR '('expr ',' expr ',' expr ')' instruction tOTHERWISE instruction { $$ = new fir::for_other_wise_node(LINE,$3,$5,$7, $9, $11);}
            | tRUN tIDENTIFIER tARROW expr ':' expr tDOT expr                       { $$ = new fir::run_node(LINE,*$2,$4,$6,$8); }
            ;


conditional_instruction:  tIF expr tTHEN instruction else                              { $$ = new fir::if_else_node(LINE, $2, $4, $5); }
                       |  tIF expr tTHEN instruction                                   { $$ = new fir::if_node(LINE, $2, $4); }
                       ;

else : tELSE instruction                                    { $$ = $2; }
     ; 


iteraction_instruction: tWHILE expr tDO instruction finally   { $$ = new fir::while_node(LINE,$2,$4,$5); } 
                      | tWHILE expr tDO instruction           { $$ = new fir::while_node(LINE,$2,$4,nullptr); }
                      ;


finally: tFINALLY instruction             { $$ = $2; }                                   
       ;


expression : expr                     { $$ = new cdk::sequence_node(LINE, $1); }
           | expression ',' expr      { $$ = new cdk::sequence_node(LINE, $3, $1); }
           ;


type : tINT_TYPE                     { $$ = cdk::primitive_type::create(4, cdk::TYPE_INT); }                                                                                             
     | tFLOAT_TYPE                   { $$ = cdk::primitive_type::create(8, cdk::TYPE_DOUBLE); }                                                                                                  
     | tSTRING_TYPE                  { $$ = cdk::primitive_type::create(4, cdk::TYPE_STRING); }
     | tVOID_TYPE                    { $$ = cdk::primitive_type::create(1, cdk::TYPE_VOID); }
     | '<' type '>'                  { $$ = cdk::reference_type::create(4,$2); }
     ;  

    


expr : tINTEGER                   { $$ = new cdk::integer_node (LINE, $1); }
     | tFLOAT                     { $$ = new cdk::double_node (LINE, $1); }
	| string                     { $$ = new cdk::string_node(LINE, $1); }
     | tNULL                      { $$ = new fir::nullptr_node(LINE); }  
     | '-' expr %prec tUNARY      { $$ = new cdk::neg_node(LINE, $2); }
     | '+' expr %prec tUNARY      { $$ = new fir::identity_node(LINE, $2); }
     | '~' expr                   { $$ = new cdk::not_node(LINE, $2); }
     | expr '+' expr	         { $$ = new cdk::add_node(LINE, $1, $3); } 
     | expr '-' expr	         { $$ = new cdk::sub_node(LINE, $1, $3); }
     | expr '*' expr	         { $$ = new cdk::mul_node(LINE, $1, $3); }
     | expr '/' expr	         { $$ = new cdk::div_node(LINE, $1, $3); }
     | expr '%' expr	         { $$ = new cdk::mod_node(LINE, $1, $3); }
     | expr '<' expr	         { $$ = new cdk::lt_node(LINE, $1, $3); }
     | expr '>' expr	         { $$ = new cdk::gt_node(LINE, $1, $3); }
     | expr tGE expr	         { $$ = new cdk::ge_node(LINE, $1, $3); }
     | expr tLE expr              { $$ = new cdk::le_node(LINE, $1, $3); }
     | expr tNE expr	         { $$ = new cdk::ne_node(LINE, $1, $3); }
     | expr tEQ expr	         { $$ = new cdk::eq_node(LINE, $1, $3); }
     | expr tAND expr             { $$ = new cdk::and_node(LINE, $1, $3); }
     | expr tOR  expr             { $$ = new cdk::or_node(LINE, $1, $3);  }
     | tSIZEOF '(' expr ')'       { $$ = new fir::size_of_node(LINE, $3); }
     | '(' expr ')'               { $$ = $2; }
     | '[' expr ']'               { $$ = $2; } 
     | lval                       { $$ = new cdk::rvalue_node(LINE, $1); }  
     | lval '=' expr              { $$ = new cdk::assignment_node(LINE, $1, $3); }
     | lval '?'                   { $$ = new fir::address_node(LINE, $1); }
     | function_call              { $$ = $1; }
     ;


lval : tIDENTIFIER             { $$ = new cdk::variable_node(LINE, $1); }
     | expr '[' expr ']'       { $$ = new fir::index_node(LINE, $1, $3); }
     ;  

string : tSTRING                                                                     { $$ = $1; }
       | string tSTRING                                                              { $$ = new std::string(*$1 + *$2); delete $1; delete $2; }
       ; 

function_call : tIDENTIFIER '(' expression ')'                                       { $$ = new fir::function_call_node(LINE, *$1, $3); }
              | tIDENTIFIER '(' ')'                                                  { $$ = new fir::function_call_node(LINE, *$1, nullptr); }
              ; 
%%

