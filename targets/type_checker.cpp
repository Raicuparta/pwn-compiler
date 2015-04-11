// $Id: type_checker.cpp,v 1.3 2015/03/24 14:09:47 ist173639 Exp $ -*- c++ -*-
#include <string>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

#define ASSERT_UNSPEC \
    { if (node->type() != nullptr && \
          node->type()->name() != basic_type::TYPE_UNSPEC) return; }

//---------------------------------------------------------------------------

void pwn::type_checker::do_integer_node(cdk::integer_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void pwn::type_checker::do_string_node(cdk::string_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_STRING));
}

//---------------------------------------------------------------------------

inline void pwn::type_checker::processUnaryExpression(cdk::unary_expression_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in argument of unary expression");

  // in Simple, expressions are always int
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void pwn::type_checker::do_neg_node(cdk::neg_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

inline void pwn::type_checker::processBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in left argument of binary expression");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in right argument of binary expression");

  // in Simple, expressions are always int
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void pwn::type_checker::do_add_node(cdk::add_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::type_checker::do_sub_node(cdk::sub_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::type_checker::do_mul_node(cdk::mul_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::type_checker::do_div_node(cdk::div_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::type_checker::do_mod_node(cdk::mod_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::type_checker::do_lt_node(cdk::lt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::type_checker::do_le_node(cdk::le_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::type_checker::do_ge_node(cdk::ge_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::type_checker::do_gt_node(cdk::gt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::type_checker::do_ne_node(cdk::ne_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::type_checker::do_eq_node(cdk::eq_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void pwn::type_checker::do_rvalue_node(pwn::rvalue_node * const node, int lvl) {
  node->lvalue()->accept(this, lvl);
  node->type(node->lvalue()->type());
}

//---------------------------------------------------------------------------

/*void pwn::type_checker::do_lvalue_node(pwn::lvalue_node * const node, int lvl) {
  const std::string &id = node->value();
  std::shared_ptr<pwn::symbol> symbol = _symtab.find(id);
  if (symbol == nullptr) throw id + " undeclared";
  // hackish stuff...
  node->type(new basic_type(4, basic_type::TYPE_INT));
}*/

//---------------------------------------------------------------------------

void pwn::type_checker::do_assignment_node(pwn::assignment_node * const node, int lvl) {
  ASSERT_UNSPEC;

  // DAVID: horrible hack!
  // (this is caused by Simple not having explicit variable declarations)
  /*const std::string &id = node->lvalue()->value();
  if (!_symtab.find(id)) {
    _symtab.insert(id, std::make_shared<pwn::symbol>(new basic_type(4, basic_type::TYPE_INT), id, -1)); // put in the symbol table
  }*/

  node->lvalue()->accept(this, lvl + 2);
  if (node->lvalue()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in left argument of assignment expression");

  node->rvalue()->accept(this, lvl + 2);
  if (node->rvalue()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in right argument of assignment expression");

  // in Simple, expressions are always int
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

//---------------------------------------------------------------------------

void pwn::type_checker::do_evaluation_node(pwn::evaluation_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

void pwn::type_checker::do_print_node(pwn::print_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void pwn::type_checker::do_read_node(pwn::read_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void pwn::type_checker::do_while_node(cdk::while_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

//---------------------------------------------------------------------------

void pwn::type_checker::do_if_node(cdk::if_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

void pwn::type_checker::do_if_else_node(cdk::if_else_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

	/*Novos nodes*/
	
void pwn::type_checker::do_repeat_node(pwn::repeat_node * const node, int lvl) {
	//TODO
}

void pwn::type_checker::do_stop_node(pwn::stop_node * const node, int lvl) {
	//TODO
}

void pwn::type_checker::do_and_node(pwn::and_node * const node, int lvl) {
	//TODO
}

void pwn::type_checker::do_or_node(pwn::or_node * const node, int lvl) {
	//TODO
}

void pwn::type_checker::do_return_node(pwn::return_node * const node, int lvl) {
	//TODO
}

void pwn::type_checker::do_identity_node(pwn::identity_node * const node, int lvl) {
	//TODO
}

void pwn::type_checker::do_next_node(pwn::next_node * const node, int lvl) {
	//TODO
}
void pwn::type_checker::do_noob_node(pwn::noob_node * const node, int lvl) {
	//TODO
}
void pwn::type_checker::do_index_node(pwn::index_node * const node, int lvl) {
	//TODO
}
void pwn::type_checker::do_block_node(pwn::block_node * const node, int lvl) {
	//TODO
}
void pwn::type_checker::do_func_decl_node(pwn::func_decl_node * const node, int lvl) {
	//TODO
}
void pwn::type_checker::do_func_def_node(pwn::func_def_node * const node, int lvl) {
	//TODO
}
void pwn::type_checker::do_func_call_node(pwn::func_call_node * const node, int lvl) {
	//TODO
}
void pwn::type_checker::do_var_node(pwn::var_node * const node, int lvl) {
	//TODO
}
void pwn::type_checker::do_var_decl_node(pwn::var_decl_node * const node, int lvl) {
	//TODO
}
void pwn::type_checker::do_println_node(pwn::println_node * const node, int lvl) {
	//TODO
}
void pwn::type_checker::do_maloc_node(pwn::maloc_node * const node, int lvl) {
	//TODO
}
void pwn::type_checker::do_mem_address_node(pwn::mem_address_node * const node, int lvl) {
	//TODO
}
void pwn::type_checker::do_not_node(pwn::not_node * const node, int lvl) {
	//TODO
}