#ifndef __PWN_FUNCCALLNODE_H__
#define __PWN_FUNCCALLNODE_H__

#include <cdk/ast/expression_node.h>

namespace pwn {

  class func_call_node: public cdk::expression_node {
     std::string  * _name;
    cdk::sequence_node * _arguments;

  public:
    inline func_call_node(int lineno,   std::string  * name,  cdk::sequence_node * arguments) :
        cdk::expression_node(lineno),  _name(name), _arguments(arguments) {
    }

  public:
    inline  std::string  * name() {
      return _name;
    }

	inline cdk::sequence_node * arguments() {
      return _arguments;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_func_call_node(this, level);
    }

  };

} // pwn

#endif
