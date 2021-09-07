#ifndef __FIR_AST_FUNC_DECLARATION_NODE_H__
#define __FIR_AST_FUNC_DECLARATION_NODE_H__



#include <cdk/ast/sequence_node.h>
#include <cdk/ast/typed_node.h>
#include <cdk/ast/basic_node.h>
#include <string>

namespace fir {

   class function_decl_node: public cdk::typed_node {
      int _symbol;
      std::string _name;
      cdk::sequence_node *_parameters;


   public:
    inline function_decl_node(int lineno,int symbol,std::string name,cdk::sequence_node *parameters) :
        cdk::typed_node(lineno), _symbol(symbol), _name(name), _parameters(parameters) {
        type(cdk::primitive_type::create(0, cdk::TYPE_VOID));
    }

    inline function_decl_node(int lineno,std::shared_ptr<cdk::basic_type> types, int symbol,std::string name,cdk::sequence_node *parameters) :
        cdk::typed_node(lineno), _symbol(symbol), _name(name), _parameters(parameters) {
        type(types);
    }


  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_decl_node(this, level);
    }

    inline int symbol() {
      return _symbol;
    }

    inline std::string name() {
      return _name;
    }

    inline cdk::sequence_node *parameters() {
      return _parameters;
    }

    inline cdk::typed_node* parameter(size_t ax){
      return dynamic_cast<cdk::typed_node*>(_parameters->node(ax)); 
    }
  };

} // fir

#endif  
