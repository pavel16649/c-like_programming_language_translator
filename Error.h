#pragma once

#include <string>
#include <iostream>

class Error
{
public:
  Error(int str_num,  int ch_num,  std::string expected,  std::string got,  int is_semant = 0)
    : _str_num(str_num)
    ,  _ch_num(ch_num)
    ,  _expected(expected)
    ,  _got(got)
    ,  _displayed(false)
    ,  is_semant_(is_semant)
  {

  }

  void display();


private:
  int is_semant_;
  int _str_num;
  int _ch_num;
  std::string _expected;
  std::string _got;
  bool _displayed;
};

