#ifndef __PWN_FUNCDEFNODE_H__
#define __PWN_FUNCDEFNODE_H__

#include <cdk/ast/expression_node.h>

namespace pwn {

  class func_def_node: public cdk::expression_node {
    pwn::func_decl_node * _name;
    block_node * _instructions;
    cdk::expression_node * _value;
    
  public:
    inline func_def_node(int lineno,  pwn::func_decl_node * name,  block_node * instructions, cdk::expression_node *value) :
		cdk::expression_node(lineno),  _name(name), _instructions(instructions) , _value(value) {
    }
  
  public:
    inline pwn::func_decl_node *name() {
      return _name;
    }
    
    inline block_node * instructions() {
      return _instructions;
    }
    
    inline cdk::expression_node * value() {
      return _value;
    }
    
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_func_def_node(this, level);
    }
    
  };
  
} // pwn

#endif
