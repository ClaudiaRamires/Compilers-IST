#ifndef __FIR_AST_PROLOGO_NODE_H__
#define __FIR_AST_PROLOGO_NODE_H__

#include <cdk/ast/basic_node.h>
#include "block_node.h"


namespace fir {

  class prologo_node: public cdk::basic_node {
    fir::block_node *_block;

  public:
    inline prologo_node(int lineno, fir::block_node *block) :
      cdk::basic_node(lineno), _block(block) {
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_prologo_node(this, level);
    }

    inline fir::block_node *block() {
        return _block;
      }

  };

} // fir

#endif