#ifndef __FIR_AST_DECLARATION_NODE_H__
#define __FIR_AST_DECLARATION_NODE_H__

#include <cdk/types/basic_type.h>
#include <cdk/ast/typed_node.h>
#include <cdk/ast/expression_node.h>
#include <string>

namespace fir {

   class declaration_variable_node: public cdk::typed_node {
      int _symbol;
      std::string _name;
      cdk::expression_node *_express;
      

   public:
    inline declaration_variable_node(int lineno,  std::shared_ptr<cdk::basic_type> vartypes,int symbol,std::string name,cdk::expression_node *express) :
        cdk::typed_node(lineno), _symbol(symbol), _name(name), _express(express) {
          type(vartypes);
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_declaration_variable_node(this, level);
    }


    inline int symbol() {
      return _symbol;
    }

    inline std::string name() {
      return _name;
    }

    inline cdk::expression_node *express() {
      return _express;
    }

  };

} // fir

#endif