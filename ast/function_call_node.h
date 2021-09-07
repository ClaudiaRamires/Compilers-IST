#ifndef __FIR_AST_FUNCTION_CALL_NODE_H__
#define __FIR_AST_FUNCTION_CALL_NODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/sequence_node.h>
#include <string>

 

namespace fir {

  class function_call_node: public cdk::expression_node{
    std::string _name;
    cdk::sequence_node *_parameters;

  
  public:
    inline function_call_node(int lineno, std::string name, cdk::sequence_node *parameters) :
        cdk::expression_node(lineno), _name(name), _parameters(parameters) {
    }


  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_call_node(this, level);
    }

    inline std::string name() {
    return _name;
    }
    
    inline cdk::sequence_node *parameters() {
      return _parameters;
    }


  };

} // fir

#endif