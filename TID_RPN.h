#pragma once
#include "RPNToken.h"

class TID_RPN
{
public:
  TID_RPN() : parent_(NULL) {}
  TID_RPN(TID_RPN* parent) : parent_(parent) {}
  void SetVar(std::string name,  RPNType type) {
    vars_[name] = type;
  }
  RPNType GetVar(std::string name) {
    if (vars_.find(name) == vars_.end()) return parent_->GetVar(name);
    return vars_[name];
  }
  TID_RPN* Parent() { return parent_; }
private:
  TID_RPN* parent_;
  std::map<std::string,  RPNType> vars_;
};

