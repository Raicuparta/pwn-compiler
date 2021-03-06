#ifndef __PWN_VARNODE_H__
#define __PWN_VARNODE_H__

#include <cdk/ast/expression_node.h>

namespace pwn {
  
  class var_node: public lvalue_node {
    std::string * _var;
    
  public:
    inline var_node(int lineno,  std::string *  var) :
    lvalue_node(lineno),  _var(var) {
    }
    
  public: 
    inline std::string *  var() {
      return _var;
    }
    
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_var_node(this, level);
    }
    
  };
  
} // pwn

#endif
