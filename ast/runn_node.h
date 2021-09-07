#ifndef __FIR_AST_RUNN_NODE_H__
#define __FIR_AST_RUNN_NODE_H__

#include <cdk/ast/expression_node.h>

namespace fir {


  class runn_node: public cdk::basic_node {
    std::string _name;
    cdk::expression_node *_vectorbase, *_low, *_high; 
 

  public:
    inline runn_node(int lineno, std::string name, cdk::expression_node *vectorbase, cdk::expression_node *low, cdk::expression_node *high) :
        basic_node(lineno),_name(name), _vectorbase(vectorbase), _low(low), _high(high) {
    }

  public:
    inline cdk::expression_node *vectorbase() {
      return _vectorbase;
    }
    inline cdk::expression_node *low() {
      return _low;
    }

    inline cdk::expression_node *high() {
      return _high;
    }

    inline std::string name() {
    return _name;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_runn_node(this, level);
    }

  };

} // fir

#endif
