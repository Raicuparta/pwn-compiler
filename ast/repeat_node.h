// $Id: while_node.h,v 1.1 2014/05/02 22:33:16 david Exp $ -*- c++ -*-
#ifndef __PWN_REPEATNODE_H__
#define __PWN_REPEATNODE_H__

#include <cdk/ast/expression_node.h>

namespace pwn {

  class repeat_node: public cdk::basic_node {
    cdk::expression_node *_condition;
    cdk::basic_node *_block;

  public:
    inline repeat_node(int lineno, cdk::expression_node *condition, cdk::basic_node *block) :
        cdk::basic_node(lineno), _condition(condition), _block(block) {
    }

  public:
    inline cdk::expression_node *condition() {
      return _condition;
    }
    inline cdk::basic_node *block() {
      return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_repeat_node(this, level);
    }

  };

} // cdk

#endif
