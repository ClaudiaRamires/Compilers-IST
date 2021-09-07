#ifndef __FIR_AST_FOR_OTHER_WISE_NODE_H__
#define __FIR_AST_FOR_OTHER_WISE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/basic_node.h>

namespace fir {

  /**
   * Class for describing if-then-else nodes.
   */
  class for_other_wise: public cdk::basic_node {
    cdk::expression_node *_condition_1;
    cdk::expression_node *_condition_2;
    cdk::expression_node *_condition_3;
    cdk::basic_node *_instruction, *_otherwise;

  public:
    inline for_other_wise(int lineno, cdk::expression_node *condition_1,cdk::expression_node *condition_2,cdk::expression_node *condition_3, cdk::basic_node *instruction, cdk::basic_node *otherwise) :
        cdk::basic_node(lineno), _condition_1(condition_1),_condition_2(condition_2),_condition_3(condition_3), _instruction(instruction), _otherwise(otherwise) {
    }

  public:
    inline cdk::expression_node *condition_1() {
      return _condition_1;
    }
    inline cdk::expression_node *condition_2() {
      return _condition_2;
    }
    inline cdk::expression_node *condition_3() {
      return _condition_3;
    }
    inline cdk::basic_node *instruction() {
      return _instruction;
    }
    inline cdk::basic_node *otherwise() {
      return _otherwise;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_for_other_wise(this, level);
    }

  };

} // fir

#endif
