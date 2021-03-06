#ifndef __PWN_NEXTNODE_H__
#define __PWN_NEXTNODE_H__

#include "ast/lvalue_node.h"

namespace pwn {
  
  class next_node: public cdk::basic_node {
   int _value;
    
  public:
    inline next_node(int lineno, int value) :
    cdk::basic_node(lineno), _value(value) {
    }
    
  public:
    inline int value() {
      return _value;
    }
    
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_next_node(this, level);
    }
    
  };
  
} // pwn

#endif
