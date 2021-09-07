#ifndef __FIR_AST_NEXT_H__
#define __FIR_AST_NEXT_H__

#include <cdk/ast/basic_node.h>

namespace fir {

  class restart_node: public cdk::basic_node {
    int _integer;


  public:
    inline restart_node(int lineno, int integer = 1) :
        cdk::basic_node(lineno), _integer(integer) {
    }

  public:
    int integer() const {
      return _integer;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_restart_node(this, level);
    }

  };

} // fir

#endif
