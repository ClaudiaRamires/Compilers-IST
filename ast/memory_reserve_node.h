#ifndef __FIR_AST_MEMORY_RESERVE_NODE_H__
#define __FIR_AST_MEMORY_RESERVE_NODE_H__

#include <cdk/ast/unary_operation_node.h>
#include <cdk/ast/expression_node.h>


namespace fir {

  class memory_reserve_node: public cdk::unary_operation_node {


  public:
    inline memory_reserve_node(int lineno, cdk::expression_node *expression) :
        cdk::unary_operation_node(lineno,expression){
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_memory_reserve_node(this, level);
    }

  };

} // fir

#endif