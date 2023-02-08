#pragma once

#include "SemanticalAnalyzer.h"

class Token {
public:
  Token(int type,  std::string lecsema,  int str_num,  int c_num)
    : type_(type), 
    lecsema_(lecsema), 
    string_num_(str_num), 
    char_num_(c_num)
  {
  }

  int type() { return type_; }
  std::string lecsema() { return lecsema_; }
  int string_num() { return string_num_; }
  int char_num() { return char_num_; }

protected:
  int type_;
  std::string lecsema_;
  int string_num_,  char_num_;
};
