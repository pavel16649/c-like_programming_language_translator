#pragma once

#include <vector>
#include <string>
#include "Token.h"
#include <iostream>
#include "Error.h"

class SyntaxAnalyzer {
public:
  SyntaxAnalyzer()
    : _ok(true)
    ,  _current_token_index(0)
  {
    ptr_tokens_ = NULL;
  }

  bool checkTokens(std::vector <Token>& tokens);

private:
  int _current_token_index;
  std::vector < Token > _tokens;
  std::vector< Token> *ptr_tokens_;
  bool _ok;
  SemanticalAnalyzer sema_;

  enum types {
    reserved = 1, 
    name = 2, 
    constant = 3
  };

  inline void next_token();
  inline void prev_token();
  inline int token_type();
  inline std::string token_name();
  inline int string_num();
  inline int char_num();

  void program();
  void non_func_declaration();
  void func_declaration();
  void array_declaration();
  void array_template();
  void formal_parameter();
  void _main();
  void variable_declaration();
  void variable();
  void type();
  void section();
  void expression();
  void const_expr();
  void compound_operator();
  void _operator();
  void print();
  void _if();
  void _while();
  void _for();
  void do_while();
  void func_call();
  void derived_operator();
  void value();
  void _switch();
  void non_formal_parameter();
  void priority1();
  void priority2();
  void priority3();
  void priority4();
  void priority5();
  void priority6();
  void priority7();
  void priority8();
  void priority9();
  void priority10();
  void priority11();
  void priority12();
  void priority13();
  void priority14();
  void priority15();
};
