#include <string>
#include <sstream>
#include <memory>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "targets/frame_size_calculator.h"
#include <cdk/types/types.h>
#include <cdk/types/typename_type.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/integer_node.h>
#include <cdk/ast/sequence_node.h>
#include "targets/symbol.h"

#include "ast/all.h"  // all.h is automatically generated

#include "fir_parser.tab.h"

//---------------------------------------------------------------------------
void fir::postfix_writer::do_for_other_wise(fir::for_other_wise * const node, int lvl) {
 ASSERT_SAFE_EXPRESSIONS;

}

void fir::postfix_writer::do_runn_node(fir::runn_node * const node, int lvl) {
 ASSERT_SAFE_EXPRESSIONS;

}







void fir::postfix_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}

void fir::postfix_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}

void fir::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

   if (_inFunctionBody) {
    _pf.DOUBLE(node->value());
  }
  else {
    _pf.SDOUBLE(node->value());
  }
}

void fir::postfix_writer::do_not_node(cdk::not_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->argument()->accept(this,lvl);
  _pf.NOT();
}

void fir::postfix_writer::do_and_node(cdk::and_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int lbl= ++_lbl;
  node->left()->accept(this,lvl);
  _pf.DUP32();
  _pf.JZ(mklbl(lbl));
  node->right()->accept(this,lvl);
  _pf.AND();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}

void fir::postfix_writer::do_or_node(cdk::or_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int lbl= ++_lbl;
  node->left()->accept(this,lvl);
  _pf.DUP32();
  _pf.JNZ(mklbl(lbl));
  node->right()->accept(this,lvl);
  _pf.OR();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (_inFunctionBody) {
    _pf.INT(node->value()); 
  } else {
    _pf.SINT(node->value()); 
  }
}

void fir::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int lbl1;

  _pf.RODATA(); 
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); 
  _pf.SSTRING(node->value()); 

  if (_inFunctionBody) {
    _pf.TEXT(); 
    _pf.ADDR(mklbl(lbl1));
  }
  else {
    _pf.DATA();
    _pf.SADDR(mklbl(lbl1)); 
  }
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->argument()->accept(this, lvl); 
  if (node->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.DNEG();
  }
  else {
    _pf.NEG();
  }
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->left()->accept(this, lvl+2);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->left()->type()->name() == cdk::TYPE_INT) { 
    _pf.I2D();
  }
  else if (node->type()->name() == cdk::TYPE_POINTER && node->left()->type()->name() == cdk::TYPE_INT) {
    _pf.INT(3);
    _pf.SHTL();
  }
  
  node->right()->accept(this, lvl+2);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->right()->type()->name() == cdk::TYPE_INT) {
    _pf.I2D();
  } else if (node->type()->name() == cdk::TYPE_POINTER && node->right()->type()->name() == cdk::TYPE_INT) {
    _pf.INT(3);
    _pf.SHTL();
  }
  
  if (node->type()->name() == cdk::TYPE_DOUBLE) {
    _pf.DADD();
  }
  else {
    _pf.ADD();
  }
}

void fir::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

 node->left()->accept(this, lvl + 2);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->left()->type()->name() == cdk::TYPE_INT) 
    _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->right()->type()->name() == cdk::TYPE_INT) {
    _pf.I2D();

  } else if (node->type()->name() == cdk::TYPE_POINTER && node->right()->type()->name() == cdk::TYPE_INT) {
    _pf.INT(3);
    _pf.SHTL();
  }

  if (node->type()->name() == cdk::TYPE_DOUBLE)
    _pf.DSUB();
  else
    _pf.SUB();


}

void fir::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->left()->type()->name() == cdk::TYPE_INT) 
    _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->right()->type()->name() == cdk::TYPE_INT) 
    _pf.I2D();

  if (node->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.DMUL();
  }
  else {
    _pf.MUL();
  }

}

void fir::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

 node->left()->accept(this, lvl + 2);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->left()->type()->name() == cdk::TYPE_INT) 
    _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->right()->type()->name() == cdk::TYPE_INT) 
    _pf.I2D();

  if (node->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.DDIV();
  }
  else {
    _pf.DIV();
  }

}

void fir::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}

void fir::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
    if (node->left()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE) 
      _pf.I2D();

    node->right()->accept(this, lvl + 2);
    if (node->right()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE) 
      _pf.I2D();

    if (node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE) {
      _pf.DCMP();
      _pf.INT(0);
    }

    _pf.LT();
}

void fir::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
    if (node->left()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE) 
      _pf.I2D();

    node->right()->accept(this, lvl + 2);
    if (node->right()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE) 
      _pf.I2D();

    if (node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE) {
      _pf.DCMP();
      _pf.INT(0);
    }

    _pf.LE();
}

void fir::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

 node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE) 
    _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE) 
    _pf.I2D();

  if (node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }

  _pf.GE();

}

void fir::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE) 
    _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE) 
    _pf.I2D();

  if (node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE) {
      _pf.DCMP();
      _pf.INT(0);
    }
  
  _pf.GT();
}

void fir::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

 node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE) 
    _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE) 
    _pf.I2D();

  if (node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE) {
      _pf.DCMP();
      _pf.INT(0);
    }

  _pf.NE();
}

void fir::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE) 
    _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE) 
    _pf.I2D();

  if (node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE) {
      _pf.DCMP();
      _pf.INT(0);
    }

  _pf.EQ();
}



//---------------------------------------------------------------------------

void fir::postfix_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  std::shared_ptr<fir::symbol> symbol = _symtab.find(node->name());

  if (!(_inFunctionBody) && symbol->offset() == 0) {
    _pf.ADDR(node->name());
  }
  else {
    _pf.LOCAL(symbol->offset());
  }
  reset_new_symbol();
}

void fir::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->lvalue()->accept(this, lvl);
  if (node->type()->name() == cdk::TYPE_DOUBLE) {
    _pf.LDDOUBLE();
  }
  else {
    _pf.LDINT(); 
  }
}

void fir::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->rvalue()->accept(this, lvl + 2);
  if (node->type()->name() == cdk::TYPE_DOUBLE) {
    if (node->rvalue()->type()->name() == cdk::TYPE_INT) {
      _pf.I2D();
    }
    _pf.DUP64();
  } else {
    _pf.DUP32();
  }

  node->lvalue()->accept(this, lvl);
  if (node->type()->name() == cdk::TYPE_DOUBLE) {
    _pf.STDOUBLE();
  } else {
    _pf.STINT();
  }
}

//---------------------------------------------------------------------------


/*     PROGRAM NODE NOT NECESSARY



void fir::postfix_writer::do_program_node(fir::program_node * const node, int lvl) {
  // Note that Simple doesn't have functions. Thus, it doesn't need
  // a function node. However, it must start in the main function.
  // The ProgramNode (representing the whole program) doubles as a
  // main function node.

  // generate the main function (RTS mandates that its name be "_main")
  _pf.TEXT();
  _pf.ALIGN();
  _pf.GLOBAL("_main", _pf.FUNC());
  _pf.LABEL("_main");
  _pf.ENTER(0);  // Simple doesn't implement local variables

  node->statements()->accept(this, lvl);

  // end the main function
  _pf.INT(0);
  _pf.STFVAL32();
  _pf.LEAVE();
  _pf.RET();

  // these are just a few library function imports
  _pf.EXTERN("readi");
  _pf.EXTERN("printi");
  _pf.EXTERN("prints");
  _pf.EXTERN("println");
}

*/

//---------------------------------------------------------------------------

void fir::postfix_writer::do_evaluation_node(fir::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->argument()->accept(this, lvl);
  _pf.TRASH(node->argument()->type()->size());
}


//---------------------------------------------------------------------------

void fir::postfix_writer::do_read_node(fir::read_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _pf.CALL("readi");
  _pf.LDFVAL32();
  _pf.STINT();
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_while_node(fir::while_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2, lbl3;
  _pf.LABEL(mklbl(lbl1 = ++_lbl));
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl2 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl1));
  _pf.JZ(mklbl(lbl3 = ++_lbl));
  _pf.LABEL(mklbl(lbl2));
  node->finally()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl1));
  _pf.LABEL(mklbl(lbl3));
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_if_node(fir::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_if_else_node(fir::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1 = lbl2));
}


//---------------------------------------------------------------------------
//                          NEW ADDRESS
//---------------------------------------------------------------------------

void fir::postfix_writer::do_address_node(fir::address_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->lvalue()->accept(this, lvl + 2);
}

void fir::postfix_writer::do_index_node(fir::index_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if (node->base()) {
    node->base()->accept(this, lvl);
  } else {
      if (_function) {
        _pf.LOCV(-_function->type()->size());
      } else {
        std::cerr << "FATAL: " << node->lineno() << ": trying to use return value outside function" << std::endl;
      }
    }

  node->index()->accept(this, lvl);
  _pf.INT(3);
  _pf.SHTL();
  _pf.ADD(); 
}

void fir::postfix_writer::do_size_of_node(fir::size_of_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  _pf.INT(node->expression()->type()->size());
}

void fir::postfix_writer::do_block_node(fir::block_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  _symtab.push();
  if (node->declarations()) node->declarations()->accept(this, lvl + 2);
  if (node->instructions()) node->instructions()->accept(this, lvl + 2);
  _symtab.pop();
}

void fir::postfix_writer::do_nullptr_node(fir::nullptr_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

   if (_inFunctionBody) {
    _pf.INT(0);
  }
  else {
    _pf.SINT(0);
  }
}

void fir::postfix_writer::do_restart_node(fir::restart_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if (_Ini.size() != 0) {
    _pf.JMP(mklbl(_Step.top()));
  }
  else {
    throw std::string("restart - outside of the cycle ");
  }
}

void fir::postfix_writer::do_leave_node(fir::leave_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if (_Ini.size() != 0) {
    _pf.JMP(mklbl(_End.top())); 
  } else
   error(node->lineno(), "leave -  outside of the cycle");
}

void fir::postfix_writer::do_write_node(fir::write_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  for (size_t ix = 0; ix < node->arguments()->size(); ix++) {
    auto child = dynamic_cast<cdk::expression_node*>(node->arguments()->node(ix));

    std::shared_ptr<cdk::basic_type> type = child->type();
    child->accept(this, lvl); 
    if (type->name() == cdk::TYPE_INT) {
      _functions_to_declare.insert("printi");
      _pf.CALL("printi");
      _pf.TRASH(4); 

    } else if (type->name() == cdk::TYPE_DOUBLE) {
      _functions_to_declare.insert("printd");
      _pf.CALL("printd");
      _pf.TRASH(8); 

    } else if (type->name() == cdk::TYPE_STRING) {
      _functions_to_declare.insert("prints");
      _pf.CALL("prints");
      _pf.TRASH(4); 

    } else {
      std::cerr << "FATAL: " << node->lineno() << ": unknown type" << std::endl;
      return;
    }
  }
   if (node->newline()) {
    _functions_to_declare.insert("println");
    _pf.CALL("println");
  }
}

void fir::postfix_writer::do_prologo_node(fir::prologo_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->block()->accept(this, lvl + 2);

}

void fir::postfix_writer::do_function_def_node(fir::function_def_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

   if (_inFunctionBody || _inFunctionArgs) {
      error(node->lineno(), "cannot define function");
    }


  _function = new_symbol();
  reset_new_symbol();

  _pf.TEXT();
  _pf.ALIGN();
  _pf.GLOBAL("_main", _pf.FUNC());
  _pf.LABEL("_main");

  _pf.LEAVE();
  _functions_to_declare.erase(_function->name());
  _symtab.pop();

 if (node->name() == "fir") {
    // declare external functions
    for (std::string s : _functions_to_declare)
      _pf.EXTERN(s);
  }


}

void fir::postfix_writer::do_function_decl_node(fir::function_decl_node * const node, int lvl) {
   ASSERT_SAFE_EXPRESSIONS;

  if (_inFunctionBody || _inFunctionArgs) {
    error(node->lineno(), "cannot declare function in body or in args");
  }

  _function = new_symbol();

  _pf.TEXT();
  _pf.ALIGN();

  _inFunctionArgs = true;
  _currentFunctionName = node->name();
  _offset = 8;
  if (node->parameters()) {
    _symtab.push();
    node->parameters()->accept(this, lvl+2);
    _symtab.pop();
  }
  _offset = 0;
  _currentFunctionName.clear();
  _inFunctionArgs = false;

  _functions_to_declare.insert(_function->name());
  reset_new_symbol();

  if (node->name() == "fir") {
    for (std::string s: _functions_to_declare) {
      _pf.EXTERN(s);
    }
  }

}

void fir::postfix_writer::do_function_call_node(fir::function_call_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

   if (_inFunctionBody || _inFunctionArgs) {
      error(node->lineno(), "cannot call function");
    }

  std::shared_ptr<fir::symbol> symbol = _symtab.find(node->name());

  size_t argsSize = 0;
  if (node->parameters()->size() > 0) {
    for (int ax = node->parameters()->size() - 1; ax >= 0; ax--) {
      cdk::expression_node *arg = dynamic_cast<cdk::expression_node*>(node->parameters()->node(ax));
      arg->accept(this, lvl + 2);
      if (symbol->argument_is_typed(ax, cdk::TYPE_DOUBLE) && arg->is_typed(cdk::TYPE_INT)) {
        _pf.I2D();
      }
      argsSize += symbol->argument_size(ax);
    }
  }
  _pf.CALL(node->name());
  if (argsSize != 0) {
    _pf.TRASH(argsSize);
  }

  if (symbol->type()->name() == cdk::TYPE_INT || symbol->type()->name() == cdk::TYPE_POINTER || symbol->type()->name() == cdk::TYPE_STRING) {
    _pf.LDFVAL32();
  } else if (symbol->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.LDFVAL64();
  } else {
    error(node->lineno(), "cannot call function");
  }

}

void fir::postfix_writer::do_memory_reserve_node(fir::memory_reserve_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->argument()->accept(this, lvl);
  _pf.INT(3);
  _pf.SHTL();
  _pf.ALLOC(); 
  _pf.SP();

}

void fir::postfix_writer::do_identity_node(fir::identity_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->argument()->accept(this, lvl + 2);
}

void fir::postfix_writer::do_indexationptr_node(fir::indexationptr_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->nodeptr()->accept(this, lvl + 2);
  node->index()->accept(this, lvl + 2);

  _pf.INT(node->type()->size());
  _pf.MUL();
  _pf.ADD();
}

void fir::postfix_writer::do_declaration_variable_node(fir::declaration_variable_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  
  auto id = node->express();

  std::cout << "INITIAL OFFSET: " << _offset << std::endl;

  int offset = 0, typesize = node->type()->size(); 
  std::cout << "ARG: " << id << ", " << typesize << std::endl;
  if (_inFunctionBody) {
    std::cout << "IN BODY" << std::endl;
    _offset -= typesize;
    offset = _offset;
  } else if (_inFunctionArgs) {
    std::cout << "IN ARGS" << std::endl;
    offset = _offset;
    _offset += typesize;
  } else {
    std::cout << "GLOBAL!" << std::endl;
    offset = 0; 
  }
  std::cout << "OFFSET: " << id << ", " << offset << std::endl;

  auto symbol = new_symbol();
  if (symbol) {
    symbol->set_offset(offset);
    reset_new_symbol();
  }

  if (_inFunctionBody) {
    if (node->express()) {
      node->express()->accept(this, lvl);
      if (node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_STRING) || node->is_typed(cdk::TYPE_POINTER)) {
        _pf.LOCAL(symbol->offset());
        _pf.STINT();
      } else if (node->is_typed(cdk::TYPE_DOUBLE)) {
        if (node->express()->is_typed(cdk::TYPE_INT))
          _pf.I2D();
          _pf.LOCAL(symbol->offset());
          _pf.STDOUBLE();
      } else {
        std::cerr << "cannot initialize" << std::endl;
      }
    }
  } /*else {
    if (!_function) {
      if (node->express() == nullptr) {
        _pf.BSS();
        _pf.ALIGN();
        _pf.LABEL(_currentFunctionName);
        _pf.SALLOC(typesize);
      } else {

        if (node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_DOUBLE) || node->is_typed(cdk::TYPE_POINTER)) {
          _pf.RODATA();
        } else {
          _pf.DATA();
        }
        _pf.ALIGN();
        _pf.LABEL(_currentFunctionName);
      }
      } if (node->is_typed(cdk::TYPE_STRING)) {
          int litlbl;

          _pf.RODATA();
          _pf.ALIGN();
          _pf.LABEL(mklbl(litlbl = ++_lbl));
          _pf.SSTRING(dynamic_cast<cdk::string_node*>(node->express())->value());
          _pf.ALIGN();
          _pf.LABEL(_currentFunctionName);
          _pf.SADDR(mklbl(litlbl));
        } else {
          _pf.DATA();
          _pf.ALIGN();
          _pf.LABEL(_currentFunctionName);
          node->express()->accept(this, lvl);
        } */
} 
    

void fir::postfix_writer::do_return_node(fir::return_node * const node, int lvl) {
 ASSERT_SAFE_EXPRESSIONS;
 
  _pf.RET();
}


