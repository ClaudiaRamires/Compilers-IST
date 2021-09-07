#ifndef __FIR_AST_INDEXATIONPTR_NODE_H__
#define __FIR_AST_INDEXATIONPTR_NODE_H__


#include <cdk/ast/basic_node.h>
#include <cdk/ast/lvalue_node.h>
#include <cdk/ast/expression_node.h>


namespace fir {

  class indexationptr_node: public cdk::lvalue_node {
    cdk::expression_node *_nodeptr;
    cdk::expression_node *_index;

  public:
   inline indexationptr_node(int lineno, cdk::expression_node *nodeptr, cdk::expression_node *index) :
        cdk::lvalue_node(lineno), _nodeptr(nodeptr), _index(index){
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_indexationptr_node(this, level);
    }

    inline cdk::expression_node* nodeptr(){
      return _nodeptr;
    }
    inline cdk::expression_node* index(){
      return _index;
    }

  };

} // fir

#endif