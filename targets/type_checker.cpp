#include <string>
#include <memory>
#include <string>
#include <vector>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated
#include <cdk/types/basic_type.h>
#include <cdk/types/primitive_type.h>
#include <cdk/types/reference_type.h>
#include <cdk/types/typename_type.h>
#include <cdk/types/types.h>
#include <cdk/ast/expression_node.h>

#include "fir_parser.tab.h"

#define ASSERT_UNSPEC { if (node->type() != nullptr && !node->is_typed(cdk::TYPE_UNSPEC)) return; }

//---------------------------------------------------------------------------

void fir::type_checker::do_sequence_node(cdk::sequence_node *const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  } 
}

//---------------------------------------------------------------------------

void fir::type_checker::do_nil_node(cdk::nil_node *const node, int lvl) {
  // EMPTY
}

void fir::type_checker::do_data_node(cdk::data_node *const node, int lvl) {
  // EMPTY
}

void fir::type_checker::do_double_node(cdk::double_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
}

void fir::type_checker::do_not_node(cdk::not_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->argument()->accept(this, lvl);
  if (node->argument()->is_typed(cdk::TYPE_INT)) {
    node->type(node->argument()->type());
  }
  else {
    throw std::string("no");
  }
}

void fir::type_checker::do_and_node(cdk::and_node *const node, int lvl) {
  processBinaryExpression(node,lvl);
}

void fir::type_checker::do_or_node(cdk::or_node *const node, int lvl) {
  processBinaryExpression(node,lvl);
}

//---------------------------------------------------------------------------

void fir::type_checker::do_integer_node(cdk::integer_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void fir::type_checker::do_string_node(cdk::string_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_STRING));
}

//---------------------------------------------------------------------------

void fir::type_checker::processUnaryExpression(cdk::unary_operation_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if (!node->argument()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in argument of unary expression");

  // in Simple, expressions are always int
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void fir::type_checker::do_neg_node(cdk::neg_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl);
  if (node->argument()->is_typed(cdk::TYPE_INT)) {
    node->type(node->argument()->type());
  } else {
    throw std::string("integer or vector expressions expected");
  }
}

//---------------------------------------------------------------------------

void fir::type_checker::processBinaryExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (!node->left()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in left argument of binary expression");

  node->right()->accept(this, lvl + 2);
  if (!node->right()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in right argument of binary expression");

  // in Simple, expressions are always int
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void fir::type_checker::do_add_node(cdk::add_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_sub_node(cdk::sub_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_mul_node(cdk::mul_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_div_node(cdk::div_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_mod_node(cdk::mod_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_lt_node(cdk::lt_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_le_node(cdk::le_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_ge_node(cdk::ge_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_gt_node(cdk::gt_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_ne_node(cdk::ne_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_eq_node(cdk::eq_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void fir::type_checker::do_variable_node(cdk::variable_node *const node, int lvl) {
  ASSERT_UNSPEC;

  const std::string &id = node->name();
  std::shared_ptr<fir::symbol> symbol = _symtab.find(id);

  if (symbol != nullptr) {
    node->type(symbol->type());
  } else {
    throw id;
  }
}

void fir::type_checker::do_rvalue_node(cdk::rvalue_node *const node, int lvl) {
  ASSERT_UNSPEC;

  try {
    node->lvalue()->accept(this, lvl);
    node->type(node->lvalue()->type());
  } catch (const std::string &id) {
    throw "undeclared variable '" + id + "'";
  }
}

void fir::type_checker::do_assignment_node(cdk::assignment_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->lvalue()->accept(this, lvl + 4);
  node->rvalue()->accept(this, lvl + 4);

  if (node->lvalue()->is_typed(cdk::TYPE_INT)) {
    if (node->rvalue()->is_typed(cdk::TYPE_INT)) {
      node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    } else if (node->rvalue()->is_typed(cdk::TYPE_UNSPEC)) {
      node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
      node->rvalue()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    } else {
      throw std::string("wrong assignment to integer");
    }
  } else if (node->lvalue()->is_typed(cdk::TYPE_POINTER)) {


    if (node->rvalue()->is_typed(cdk::TYPE_POINTER)) {
      node->type(node->rvalue()->type());
    } else if (node->rvalue()->is_typed(cdk::TYPE_INT)) {
      node->type(cdk::primitive_type::create(4, cdk::TYPE_POINTER));
    } else if (node->rvalue()->is_typed(cdk::TYPE_UNSPEC)) {
      node->type(cdk::primitive_type::create(4, cdk::TYPE_ERROR));
      node->rvalue()->type(cdk::primitive_type::create(4, cdk::TYPE_ERROR));
    } else {
      throw std::string("wrong assignment to pointer");
    }

  } else if (node->lvalue()->is_typed(cdk::TYPE_DOUBLE)) {

    if (node->rvalue()->is_typed(cdk::TYPE_DOUBLE) || node->rvalue()->is_typed(cdk::TYPE_INT)) {
      node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    } else if (node->rvalue()->is_typed(cdk::TYPE_UNSPEC)) {
      node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
      node->rvalue()->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    } else {
      throw std::string("wrong assignment to real");
    }

  } else if (node->lvalue()->is_typed(cdk::TYPE_STRING)) {

    if (node->rvalue()->is_typed(cdk::TYPE_STRING)) {
      node->type(cdk::primitive_type::create(4, cdk::TYPE_STRING));
    } else if (node->rvalue()->is_typed(cdk::TYPE_UNSPEC)) {
      node->type(cdk::primitive_type::create(4, cdk::TYPE_STRING));
      node->rvalue()->type(cdk::primitive_type::create(4, cdk::TYPE_STRING));
    } else {
      throw std::string("wrong assignment to string");
    }

  } else {
    throw std::string("wrong types in assignment");
  }

}

//---------------------------------------------------------------------------


void fir::type_checker::do_evaluation_node(fir::evaluation_node *const node, int lvl) {

  node->argument()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void fir::type_checker::do_read_node(fir::read_node *const node, int lvl) {
 // if (node->arguments()->accept(this, lvl)) throw "unable to read";

}

//---------------------------------------------------------------------------

void fir::type_checker::do_while_node(fir::while_node *const node, int lvl) {

  node->condition()->accept(this, lvl + 4);
}

//---------------------------------------------------------------------------

void fir::type_checker::do_if_node(fir::if_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  if (!node->condition()->is_typed(cdk::TYPE_INT)) throw std::string("expected integer condition");
}

void fir::type_checker::do_if_else_node(fir::if_else_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  if (!node->condition()->is_typed(cdk::TYPE_INT)) throw std::string("expected integer condition");
}

//---------------------------------------------------------------------------
//                          NEW ADDRESS
//---------------------------------------------------------------------------

void fir::type_checker::do_address_node(fir::address_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->lvalue()->accept(this, lvl + 2);
  node->type(cdk::reference_type::create(4, node->lvalue()->type()));
}

void fir::type_checker::do_index_node(fir::index_node *const node, int lvl) {
  ASSERT_UNSPEC;
  
  std::shared_ptr < cdk::reference_type > reference;

  if (node->base()) {
    node->base()->accept(this, lvl + 2);
    reference = cdk::reference_type::cast(node->base()->type());
    if (!node->base()->is_typed(cdk::TYPE_POINTER)) throw std::string("pointer expression expected");
  } else {
    reference = cdk::reference_type::cast(_function->type());
    if (!_function->is_typed(cdk::TYPE_POINTER)) throw std::string("return pointer expression expected");
  }

  node->index()->accept(this, lvl + 2);
  if (!node->index()->is_typed(cdk::TYPE_INT)) throw std::string("integer expression expected");

  node->type(reference->referenced());
}

void fir::type_checker::do_size_of_node(fir::size_of_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->expression()->accept(this, lvl + 2);
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT)); 
}

void fir::type_checker::do_nullptr_node(fir::nullptr_node *const node, int lvl) {
  ASSERT_UNSPEC;
  
  node->type(cdk::reference_type::create(4, nullptr)); 
}

void fir::type_checker::do_block_node(fir::block_node *const node, int lvl) {
  //Empty
}

void fir::type_checker::do_restart_node(fir::restart_node *const node, int lvl) {
  //Empty 
}

void fir::type_checker::do_leave_node(fir::leave_node *const node, int lvl) {
  //Empty 
}

void fir::type_checker::do_write_node(fir::write_node *const node, int lvl) {

  node->arguments()->accept(this, lvl + 2);
}

void fir::type_checker::do_prologo_node(fir::prologo_node *const node, int lvl) {
  //Empty 
}

void fir::type_checker::do_function_call_node(fir::function_call_node *const node, int lvl) {
  ASSERT_UNSPEC;

  const std::string &id = node->name();
  std::shared_ptr<fir::symbol> symbol = _symtab.find(id);

  _parent->set_new_symbol(symbol);
  node->type(symbol->type());

  if (node->parameters()) {
    node->parameters()->accept(this, lvl + 2);
  }
}

void fir::type_checker::do_function_def_node(fir::function_def_node *const node, int lvl) {
  ASSERT_UNSPEC;

  std::string id;

  if (node->name() == "fir")
    id = "_main";
  else if (node->name() == "_main")
    id = "._main";
  else
    id = node->name();


  _inBlockReturnType = nullptr;

  auto function = fir::make_symbol(false, node->type(), id, false, true);

  std::vector < std::shared_ptr < cdk::basic_type >> argtypesparameters;
   std::vector < std::shared_ptr < cdk::basic_type >> argtypesbody;

  for (size_t ax = 0; ax < node->parameters()->size(); ax++)
    argtypesparameters.push_back(node->parameter(ax)->type());
    for(size_t ax = 0; ax < node->body()->size(); ax++)
      argtypesbody.push_back(node->bodys(ax)->type());
    function->set_argument_types(argtypesbody);
  function->set_argument_types(argtypesparameters);

  std::shared_ptr<fir::symbol> previous = _symtab.find(function->name());
  if (previous) {
    if (previous->forward()) {
      _symtab.replace(function->name(), function);
      _parent->set_new_symbol(function);
    } else {
      throw std::string("conflicting definition for '" + function->name() + "'");
    }
  } else {
    _symtab.insert(function->name(), function);
    _parent->set_new_symbol(function);
  }

}

void fir::type_checker::do_memory_reserve_node(fir::memory_reserve_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->argument()->accept(this, lvl);

  if (!node->argument()->is_typed(cdk::TYPE_INT)) {
    throw std::string("integer expected in memory reserve node");
  }

  node->type(cdk::reference_type::create(4, cdk::primitive_type::create(8, cdk::TYPE_UNSPEC)));
}

void fir::type_checker::do_identity_node(fir::identity_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->argument()->accept(this, lvl);

  if (node->argument()->type()->name() == cdk::TYPE_INT) {
    node->type(node->argument()->type());

  } else if (node->argument()->type()->name() == cdk::TYPE_DOUBLE) {
    node->type(node->argument()->type());

  } else {
    throw std::string("Cannot be identity");
  }
}

void fir::type_checker::do_indexationptr_node(fir::indexationptr_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->nodeptr()->accept(this, lvl + 2);
  node->index()->accept(this, lvl + 2);
  if (!node->nodeptr()->is_typed(cdk::TYPE_INT) || !node->index()->is_typed(cdk::TYPE_POINTER)) {
    throw std::string("cannot index ptr");
  }
}

void fir::type_checker::do_return_node(fir::return_node *const node, int lvl) {
  //Empty 
}

void fir::type_checker::do_declaration_variable_node(fir::declaration_variable_node *const node, int lvl) {
  ASSERT_UNSPEC;
 
  if (node->express() != nullptr) {

    node->express()->accept(this, lvl + 2);
    if (node->is_typed(cdk::TYPE_INT)) {
      if (!node->express()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type for express (integer expected).");
    } else if (node->is_typed(cdk::TYPE_DOUBLE)) {
        if (!node->express()->is_typed(cdk::TYPE_INT) && !node->express()->is_typed(cdk::TYPE_DOUBLE)) {
        throw std::string("wrong type for express (integer or double expected).");
      }
    } else if (node->is_typed(cdk::TYPE_STRING)) {
      if (!node->express()->is_typed(cdk::TYPE_STRING)) {
        throw std::string("wrong type for initializer (string expected).");
      }
    } else if (node->is_typed(cdk::TYPE_POINTER)) {
      //DAVID: FIXME: trouble!!!
      if (!node->express()->is_typed(cdk::TYPE_POINTER)) {
        auto in = (cdk::literal_node<int>*)node->express();
        if (in == nullptr || in->value() != 0) throw std::string("wrong type for express (pointer expected).");
      }
    } else {
      throw std::string("unknown type for express.");
    }
  }
}


void fir::type_checker::do_function_decl_node(fir::function_decl_node *const node, int lvl) {
  std::string id;

  if (node->name() == "fir")
    id = "_main";
  else if (node->name() == "_main")
    id = "._main";
  else
    id = node->name();

  std::shared_ptr<fir::symbol> function;

  if (node->type()->name() == cdk::TYPE_INT) {
    node->type(cdk::reference_type::create(4, cdk::primitive_type::create(8, cdk::TYPE_INT)));
  }
  else if (node->type()->name() == cdk::TYPE_VOID) {
     node->type(cdk::reference_type::create(4, cdk::primitive_type::create(8, cdk::TYPE_VOID)));
  }
  else if (node->type()->name() == cdk::TYPE_STRING) {
     node->type(cdk::reference_type::create(4, cdk::primitive_type::create(8, cdk::TYPE_STRING)));
  }
  else if (node->type()->name() == cdk::TYPE_DOUBLE) {
     node->type(cdk::reference_type::create(4, cdk::primitive_type::create(8, cdk::TYPE_DOUBLE)));
  }
  else {
    throw std::string("Undefined function type");      
  }

  std::shared_ptr<fir::symbol> declared = _symtab.find(id);
  if (declared) {
      throw std::string("Function already declared"); 
    } 
  else {
    _symtab.insert(node->name(), function);
    _parent->set_new_symbol(function);
  }


void fir::type_checker::do_for_other_wise(fir::for_other_wise *const node, int lvl) {
  ASSERT_UNSPEC;

  node->condition_1()->accept(this, lvl + 4);
  node->condition_2()->accept(this, lvl + 4);
  node->condition_3()->accept(this, lvl + 4);

  if(!node->condition_2()->type()->name() == cdk::TYPE_INT) throw std::string("Bad type");   

}

void fir::type_checker::do_run_node(fir::run_node *const node, int lvl) {
  ASSERT_UNSPEC;


  std::shared_ptr<fir::symbol> declared = _symtab.find(id);
  if (!declared) {
      throw std::string("Function NOT DECLARED"); 
    } 

  node->vectorbase()->accept(this, lvl + 4);
  node->low()->accept(this, lvl + 4);
  node->high()->accept(this, lvl + 4);  

}
