#include <string>
#include "targets/xml_writer.h"
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

#include "fir_parser.tab.h"

//---------------------------------------------------------------------------

void fir::xml_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  openTag(node,lvl);
  closeTag(node,lvl);
}
void fir::xml_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}
void fir::xml_writer::do_double_node(cdk::double_node * const node, int lvl) {
  process_literal(node, lvl);
}
void fir::xml_writer::do_not_node(cdk::not_node * const node, int lvl) {
  do_unary_operation(node, lvl);
}
void fir::xml_writer::do_and_node(cdk::and_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void fir::xml_writer::do_or_node(cdk::or_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}

//---------------------------------------------------------------------------

void fir::xml_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<sequence_node size='" << node->size() << "'>" << std::endl;
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void fir::xml_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  process_literal(node, lvl);
}

void fir::xml_writer::do_string_node(cdk::string_node * const node, int lvl) {
  process_literal(node, lvl);
}

//---------------------------------------------------------------------------

void fir::xml_writer::do_unary_operation(cdk::unary_operation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void fir::xml_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  do_unary_operation(node, lvl);
}

//---------------------------------------------------------------------------

void fir::xml_writer::do_binary_operation(cdk::binary_operation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void fir::xml_writer::do_add_node(cdk::add_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void fir::xml_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void fir::xml_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void fir::xml_writer::do_div_node(cdk::div_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void fir::xml_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void fir::xml_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void fir::xml_writer::do_le_node(cdk::le_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void fir::xml_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void fir::xml_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void fir::xml_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void fir::xml_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}

//---------------------------------------------------------------------------

void fir::xml_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label() << ">" << node->name() << "</" << node->label() << ">" << std::endl;
}

void fir::xml_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

void fir::xml_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);

  node->lvalue()->accept(this, lvl);
  reset_new_symbol();

  node->rvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}


//---------------------------------------------------------------------------

void fir::xml_writer::do_evaluation_node(fir::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void fir::xml_writer::do_read_node(fir::read_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void fir::xml_writer::do_while_node(fir::while_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
  openTag("finally", lvl + 2);
  node->finally()->accept(this, lvl + 4);
  closeTag("finally", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void fir::xml_writer::do_if_node(fir::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  closeTag(node, lvl);
}

void fir::xml_writer::do_if_else_node(fir::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->thenblock()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  openTag("else", lvl + 2);
  node->elseblock()->accept(this, lvl + 4);
  closeTag("else", lvl + 2);
  closeTag(node, lvl);
}


//---------------------------------------------------------------------------
//                              NEW ADDRESS
//---------------------------------------------------------------------------


void fir::xml_writer::do_address_node(fir::address_node * const node, int lvl) {
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void fir::xml_writer::do_index_node(fir::index_node * const node, int lvl) {
  openTag(node, lvl);
  openTag("base", lvl+2);
  node->base()->accept(this, lvl + 4);
  closeTag("base", lvl+2);
  openTag("index", lvl+2);
  node->index()->accept(this, lvl + 4);
  closeTag("index", lvl+2);
  closeTag(node, lvl);
}

void fir::xml_writer::do_size_of_node(fir::size_of_node * const node, int lvl) {
  openTag(node,lvl);
  node->expression()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void fir::xml_writer::do_nullptr_node(fir::nullptr_node * const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}

void fir::xml_writer::do_block_node(fir::block_node * const node, int lvl) {
  openTag(node, lvl);
  openTag("declarations", lvl+2);
  node->declarations()->accept(this, lvl + 4);
  closeTag("declarations", lvl+2);
  openTag("instructions", lvl+2);
  node->instructions()->accept(this, lvl + 4);
  closeTag("instructions", lvl+2);
  closeTag(node, lvl);
}

void fir::xml_writer::do_restart_node(fir::restart_node * const node, int lvl) {
  openTag(node, lvl);
  os() << node->integer() << std::endl;
  closeTag(node, lvl);
}

void fir::xml_writer::do_leave_node(fir::leave_node * const node, int lvl) {
  openTag(node, lvl);
  os() << node->integer() << std::endl;
  closeTag(node, lvl);
}

void fir::xml_writer::do_write_node(fir::write_node * const node, int lvl) {
  openTag(node, lvl);
  openTag("arguments", lvl+2);
  node->arguments()->accept(this, lvl + 4);
  closeTag("arguments", lvl+2);
  openTag("newline", lvl+2);
  os() << node->newline() << std::endl;
  closeTag("newline", lvl+2);
  closeTag(node, lvl);

}

void fir::xml_writer::do_prologo_node(fir::prologo_node * const node, int lvl) {
  openTag(node, lvl);
  node->block()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void fir::xml_writer::do_function_def_node(fir::function_def_node * const node, int lvl) {
  _function = new_symbol();
  reset_new_symbol();

  _inFunctionBody = true;
  _symtab.push(); // scope of args

  os() << std::string(lvl, ' ') << "<" << node->label() << " symbol= '" << (node->symbol()) << "' type= '" 
  << cdk::to_string(std::shared_ptr<cdk::basic_type>(node->type())) << "' name= '" << node->name() << "'>" << std::endl;


  openTag("parameters", lvl+2);
  if (node->parameters()) {
    _inFunctionArgs = true; 
    node->parameters()->accept(this, lvl + 4);
    _inFunctionArgs = false; 
  }
  closeTag("parameters", lvl+2);
  node->return_node()->accept(this, lvl + 2);

  openTag("body", lvl+2);
  if (node->body()) {
    _inFunctionArgs = true; 
    node->body()->accept(this, lvl + 4);
    _inFunctionArgs = false; 
  }
  closeTag("body", lvl+2);


  closeTag(node, lvl);
  _symtab.pop(); // scope of args
  _inFunctionBody = false;

}

void fir::xml_writer::do_function_call_node(fir::function_call_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<" << node->label() << " name='" << node->name() << "'>" << std::endl;
  openTag("parameters", lvl+2);
  if (node->parameters()) {
     node->parameters()->accept(this, lvl + 4);
  }
  closeTag("parameters", lvl+2); 
  closeTag(node, lvl);
}

void fir::xml_writer::do_memory_reserve_node(fir::memory_reserve_node * const node, int lvl) {
  do_unary_operation(node,lvl);
}

void fir::xml_writer::do_identity_node(fir::identity_node * const node, int lvl) {
  do_unary_operation(node,lvl);
}

void fir::xml_writer::do_indexationptr_node(fir::indexationptr_node * const node, int lvl) {
  openTag(node, lvl);
  openTag("nodeptr", lvl+2);
  node->nodeptr()->accept(this, lvl + 4);
  closeTag("nodeptr", lvl+2);
  openTag("index", lvl+2);
  node->index()->accept(this, lvl + 4);
  closeTag("index", lvl+2);
  closeTag(node, lvl);
}

void fir::xml_writer::do_return_node(fir::return_node * const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}

void fir::xml_writer::do_declaration_variable_node(fir::declaration_variable_node * const node, int lvl) {
  reset_new_symbol();

  os() << std::string(lvl, ' ') << "<" << node->label() << " name='" << node->name() << "' symbol='"
      << (node->symbol()) << "' type='" << cdk::to_string(std::shared_ptr<cdk::basic_type>(node->type())) << "'>" << std::endl;

  if (node->express()) {
  openTag("express",lvl+2);
  node->express()->accept(this, lvl + 4);
  closeTag("express", lvl+2);
  }
  closeTag(node, lvl);
}

void fir::xml_writer::do_function_decl_node(fir::function_decl_node * const node, int lvl) {
  reset_new_symbol();
  os() << std::string(lvl, ' ') << "<" << node->label() << " name='" << node->name() << "' symbol='"
      << (node->symbol()) << "' type='" << cdk::to_string(std::shared_ptr<cdk::basic_type>(node->type())) << "'>" << std::endl;

  openTag("parameters", lvl+2);
   if (node->parameters()) {
    _symtab.push();
    node->parameters()->accept(this, lvl + 4);
    _symtab.pop();
  }

  closeTag("parameters", lvl+2);  
  closeTag(node, lvl);

}

void fir::xml_writer::do_for_other_wise(fir::for_other_wise * const node, int lvl) {
  //Empty
}