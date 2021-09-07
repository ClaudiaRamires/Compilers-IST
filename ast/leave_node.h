#ifndef __FIR_AST_STOP_H__
#define __FIR_AST_STOP_H__

#include <cdk/ast/basic_node.h>

namespace fir {

  class leave_node: public cdk::basic_node {
    int _integer;

  public:
    inline leave_node(int lineno, int integer = 1) :
        cdk::basic_node(lineno), _integer(integer) {
    }

  public:
    int integer() const {
      return _integer;
    }

    void accept(basic_ast_visitor *sp, int integer) {
      sp->do_leave_node(this, integer);
    }

  };

} // fir

#endif
