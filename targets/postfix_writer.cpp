// $Id: postfix_writer.cpp,v 1.7 2015/04/14 10:00:27 ist173639 Exp $ -*- c++ -*-
#include <string>
#include <string.h>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "ast/all.h"  // all.h is automatically generated

//---------------------------------------------------------------------------
//     THIS IS THE VISITOR'S DEFINITION
//---------------------------------------------------------------------------

void pwn::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  _pf.INT(node->value()); // push an integer
}

void pwn::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  int lbl1;

  /* generate the string */
  _pf.RODATA(); // strings are DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  _pf.STR(node->value()); // output string characters

  /* leave the address on the stack */
  _pf.TEXT(); // return to the TEXT segment
  _pf.ADDR(mklbl(lbl1)); // the string to be printed
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value
  _pf.NEG(); // 2-complement
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.ADD();
}
void pwn::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.SUB();
}
void pwn::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MUL();
}
void pwn::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.DIV();
}
void pwn::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}
void pwn::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LT();
}
void pwn::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LE();
}
void pwn::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GE();
}
void pwn::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GT();
}
void pwn::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.NE();
}
void pwn::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.EQ();
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_rvalue_node(pwn::rvalue_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->lvalue()->accept(this, lvl);
  _pf.LOAD(); //FIXME: depends on type size
}

//---------------------------------------------------------------------------

/* void pwn::postfix_writer::do_lvalue_node(pwn::lvalue_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  // simplified generation: all variables are global
  _pf.ADDR(node->value());
} */

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_assignment_node(pwn::assignment_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  // DAVID: horrible hack!
  // (this is caused by Simple not having explicit variable declarations)
  /* const std::string &id = node->lvalue()->value();
  std::shared_ptr<pwn::symbol> symbol = _symtab.find(id);
  if (symbol->value() == -1) {
    _pf.DATA(); // variables are all global and live in DATA
    _pf.ALIGN(); // make sure we are aligned
    _pf.LABEL(id); // name variable location
    _pf.CONST(0); // initialize it to 0 (zero)
    _pf.TEXT(); // return to the TEXT segment
    symbol->value(0);
  }*/

  node->rvalue()->accept(this, lvl); // determine the new value
  _pf.DUP();
  node->lvalue()->accept(this, lvl); // where to store the value
  _pf.STORE(); // store the value at address
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_func_def_node(pwn::func_def_node * const node, int lvl) {
  // Note that Simple doesn't have functions. Thus, it doesn't need
  // a function node. However, it must start in the main function.
  // The ProgramNode (representing the whole program) doubles as a
  // main function node.

  // generate the main function (RTS mandates that its name be "_main")
  // stackcounter sc(node, _compiler);
  
  std::cout << "------ DEF -------" << std::endl;
  CHECK_TYPES(_compiler, _symtab, node);

  _pf.TEXT();
 
  
  std::string * name = node->name()->name(); //nome da funcao
  if(strcmp(name->c_str(), "pwn")==0){
    name = new std::string("_main");
  }
  else if(strcmp(name->c_str(), "_main")==0){
    name = new std::string("._main");
  }
  
  const std::string &sName = *name;
  _pf.GLOBAL(sName, _pf.FUNC());
  _pf.ALIGN();
	_pf.LABEL(sName);
  _pf.ENTER(0);  // Simple doesn't implement local variables

  if(node->name()->arguments() != NULL)
    node->name()->arguments()->accept(this, lvl); //argumentos da funcao
  if(node->instructions() != NULL)
    node->instructions()->accept(this, lvl); //corpo da funcao
  if(node->value() != NULL)
    node->value()->accept(this, lvl); //retorno por omissao da funcao

  // end the main function
  _pf.INT(0);
  _pf.POP();
  _pf.LEAVE();
  _pf.RET();

  // these are just a few library function imports
  _pf.EXTERN("readi");
  _pf.EXTERN("printi");
  _pf.EXTERN("prints");
  _pf.EXTERN("println");
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_evaluation_node(pwn::evaluation_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.TRASH(4); // delete the evaluated value
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.TRASH(4); // delete the evaluated value's address
  }
  else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

void pwn::postfix_writer::do_print_node(pwn::print_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value to print
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  }
  else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_read_node(pwn::read_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  _pf.CALL("readi");
  _pf.PUSH();
 // node->argument()->accept(this, lvl);
  _pf.STORE();
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_while_node(cdk::while_node * const node, int lvl) {
  int lbl1, lbl2;
  _pf.LABEL(mklbl(lbl1 = ++_lbl));
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl2 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl1));
  _pf.LABEL(mklbl(lbl2));
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_if_node(cdk::if_node * const node, int lvl) {
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_if_else_node(cdk::if_else_node * const node, int lvl) {
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1 = lbl2));
}

//------------Novos nodes-----------------------------------------------


void pwn::postfix_writer::do_repeat_node(pwn::repeat_node * const node, int lvl) {
  //TODO
}
void pwn::postfix_writer::do_stop_node(pwn::stop_node * const node, int lvl) {
  //TODO
}
void pwn::postfix_writer::do_and_node(pwn::and_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.AND();
}
void pwn::postfix_writer::do_or_node(pwn::or_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.OR();
}
void pwn::postfix_writer::do_return_node(pwn::return_node * const node, int lvl) {
  //TODO
}
void pwn::postfix_writer::do_identity_node(pwn::identity_node * const node, int lvl) {
  //TODO
}
void pwn::postfix_writer::do_next_node(pwn::next_node * const node, int lvl) {
  //TODO
}
void pwn::postfix_writer::do_noob_node(pwn::noob_node * const node, int lvl) {
  //TODO
}
void pwn::postfix_writer::do_index_node(pwn::index_node * const node, int lvl) {
  //TODO
}
void pwn::postfix_writer::do_block_node(pwn::block_node * const node, int lvl) {
  //TODO
}
void pwn::postfix_writer::do_func_decl_node(pwn::func_decl_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  std::cout << "------ DECL -------" << std::endl;
	
	
	
  std::string * name = node->name(); //nome da funcao
  if(strcmp(name->c_str(), "pwn")==0){
    name = new std::string("_main");
  }
  else if(strcmp(name->c_str(), "_main")==0){
    name = new std::string("._main");
  }
  
  const std::string &sName = *name;
  std::shared_ptr<pwn::symbol> symbol = _symtab.find(sName);
  
  if (symbol == NULL){
		std::cout << "------ INSERTING " << sName << "-------" << std::endl;
    _symtab.insert(sName, std::make_shared<pwn::symbol> (node->type(), sName, -1));    
  }
  else {
    throw sName + "redeclaration";
  }
}
void pwn::postfix_writer::do_func_call_node(pwn::func_call_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  
  // stackcounter sc(node, _compiler);
  std::string * name = node->name(); //nome da funcao
  const std::string &fName = * name;
  if(node->arguments() != NULL){
    node->arguments()->accept(this, lvl+1);
  }
  
  _pf.CALL(fName);
  //_pf.TRASH(sc.size());
 
  if(strcmp(name->c_str(), "pwn")==0){
    name = new std::string("_main");
  }
  else if(strcmp(name->c_str(), "_main")==0){
    name = new std::string("._main");
  }
  const std::string &sName = * name;
  std::shared_ptr<pwn::symbol> symbol = _symtab.find(sName);
  
  if( (symbol->type()->name() == basic_type::TYPE_INT) || (symbol->type()->name() == basic_type::TYPE_STRING) || (symbol->type()->name() == basic_type::TYPE_POINTER) )
     _pf.PUSH();
  
  if(symbol->type()->name() == basic_type::TYPE_DOUBLE)
     _pf.DPUSH();
  
}
void pwn::postfix_writer::do_var_node(pwn::var_node * const node, int lvl) {
  //TODO
}
void pwn::postfix_writer::do_var_decl_node(pwn::var_decl_node * const node, int lvl) {
  //TODO
}
void pwn::postfix_writer::do_println_node(pwn::println_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value to print
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  }
  else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
  _pf.CALL("println"); // print a newline
}
void pwn::postfix_writer::do_maloc_node(pwn::maloc_node * const node, int lvl) {
  //TODO
}
void pwn::postfix_writer::do_mem_address_node(pwn::mem_address_node * const node, int lvl) {
  //TODO
}
void pwn::postfix_writer::do_not_node(pwn::not_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value
  _pf.NOT(); // 2-complement
}

