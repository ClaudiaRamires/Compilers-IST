#ifndef __FIR_AST_RUN_NODE_H__
#define __FIR_AST_RUN_NODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/basic_node.h>

namespace fir {

  /**
   * Class for describing if-then-else nodes.
   */
  class run_node: public cdk::basic_node {
    std::string _name;
    cdk::expression_node *_vectorbase;
    cdk::expression_node *_low;
    cdk::expression_node *_high;


  public:
    inline run_node(int lineno,std::string name, cdk::expression_node *vectorbase,cdk::expression_node *low, cdk::expression_node *high) :
        cdk::basic_node(lineno),_name(name), _vectorbase(vectorbase),_low(low),_high(high) {
    }

  public:
    inline std::string name() {
      return _name;
    }

    inline cdk::expression_node *vectorbase() {
      return _vectorbase;
    }
    inline cdk::expression_node *low() {
      return _low;
    }

    inline cdk::expression_node *high() {
    return _high;
    }


    void accept(basic_ast_visitor *sp, int level) {
      sp->do_run_node(this, level);
    }

  };

} // fir

#endif
