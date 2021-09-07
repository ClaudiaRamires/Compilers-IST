#ifndef __FIR_AST_FUNCTION_DEF_NODE_H__
#define __FIR_AST_FUNCTION_DEF_NODE_H__

#include <cdk/types/basic_type.h>
#include <cdk/ast/typed_node.h>
#include <cdk/ast/sequence_node.h>
#include "return_node.h"
#include <string>

namespace fir {

  class function_def_node: public cdk::typed_node {
      int _symbol;
      std::string _name;
      cdk::sequence_node *_parameters;
      fir::return_node *_return_node;
      cdk::sequence_node *_body;
      


   public:
    inline function_def_node(int lineno,int symbol,std::string name,cdk::sequence_node *parameters, fir::return_node *return_node, cdk::sequence_node *body) :
        cdk::typed_node(lineno), _symbol(symbol), _name(name), _parameters(parameters), _return_node(return_node),_body(body) {
          type(cdk::primitive_type::create(1, cdk::TYPE_VOID));
    }

    inline function_def_node(int lineno, std::shared_ptr<cdk::basic_type> types,int symbol,std::string name,cdk::sequence_node *parameters, fir::return_node *return_node, cdk::sequence_node *body) :
        cdk::typed_node(lineno), _symbol(symbol), _name(name), _parameters(parameters), _return_node(return_node),_body(body) {
          type(types);
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_def_node(this, level);
    }

    inline int symbol() {
      return _symbol;
    }

    inline std::string name() {
      return _name;
    }

    inline cdk::typed_node* parameter(size_t ax){
      return dynamic_cast<cdk::typed_node*>(_parameters->node(ax)); 
    }

    inline cdk::sequence_node *parameters() {
      return _parameters;
    }

     inline cdk::typed_node* bodys(size_t ax){
      return dynamic_cast<cdk::typed_node*>(_body->node(ax)); 
    }

    inline cdk::sequence_node *body() {
      return _body;
    }

    inline fir::return_node *return_node(){
      return _return_node;
    }

  };

} // fir

#endif