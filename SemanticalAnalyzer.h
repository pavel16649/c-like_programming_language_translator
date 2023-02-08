#pragma once

#include "TID.h"

struct TypeOperNode {
  TypeOperNode(std::string data,  bool is_type) : data_(data),  is_type_(is_type) {}
  bool is_type_;
  std::string data_;
};

class SemanticalAnalyzer
{
public:

  std::string cur_type;
  std::vector<std::string> cur_args;
  std::string cur_func;

  SemanticalAnalyzer() {
    global_tid_ = new GlobalTID;
    cur_tid_ = global_tid_;
  }

  void SetNode(std::string data,  bool is_type) {
    type_oper_stack_.push(TypeOperNode(data,  is_type));
  }

  void NewTid() {
    cur_tid_ = new TID(cur_tid_);
    return;
  }

  TypeOperNode GetNode() {
    if (type_oper_stack_.empty()) {
      return TypeOperNode("Error",  1);
    }
    TypeOperNode ret = type_oper_stack_.top();
    type_oper_stack_.pop();
    return ret;
  }

  bool CheckType(std::string type1,  std::string type2) {
    if ((type1 == "int" || type1 == "char" || type1 == "float" || type1 == "bool") && (type2 == "int" || type2 == "char" || type2 == "float" || type2 == "bool")) {
      return true;
    }
    if (type1 == "string" && (type2 == "string" || type2 == "char")) {
      return true;
    }
    return false;
  }

  

  std::string CheckOperType(std::string type2,  std::string oper,  std::string type1) {
    std::map < std::string,  std::map<std::string,  std::vector<std::string>>> opers;
    std::vector<std::string> types = { "int",  "float",  "bool",  "char",  "string" };
    std::vector<std::string> bool_opers = { "&&",  "||",  "==",  "!=",  "<" ,  ">",  "<=",  ">="};
    std::vector<std::string> arr_opers = { "=",  "==",  "!=" };
    std::map<std::string,  int> type_size;
    type_size["bool"] = 0;
    type_size["char"] = 1;
    type_size["int"] = 2;
    type_size["float"] = 3;

    {
      // =
      opers["="]["int"] = { "int",  "float",  "bool",  "char" };
      opers["="]["float"] = { "int",  "float",  "bool",  "char" };
      opers["="]["bool"] = { "int",  "float",  "bool",  "char" };
      opers["="]["char"] = { "int",  "char" };
      opers["="]["string"] = { "string",  "char" };

      // +=
      opers["+="]["int"] = { "int",  "float",  "bool",  "char" };
      opers["+="]["float"] = { "int",  "float",  "bool",  "char" };
      opers["+="]["bool"] = { "bool" };
      opers["+="]["char"] = { "int",   "char" };
      opers["+="]["string"] = { "string",  "char" };

      // -=
      opers["-="]["int"] = { "int",  "float",  "bool",  "char" };
      opers["-="]["float"] = { "int",  "float",  "bool",  "char" };
      opers["-="]["bool"] = { "bool" };
      opers["-="]["char"] = { "int",   "char" };
      opers["-="]["string"] = { };

      // *=
      opers["*="]["int"] = { "int",  "float",  "bool",  "char" };
      opers["*="]["float"] = { "int",  "float",  "bool",  "char" };
      opers["*="]["bool"] = {  };
      opers["*="]["char"] = {  };
      opers["*="]["string"] = { "int" };

      // /=
      opers["/="]["int"] = { "int",  "float",  "bool",  "char" };
      opers["/="]["float"] = { "int",  "float",  "bool",  "char" };
      opers["/="]["bool"] = {  };
      opers["/="]["char"] = {  };
      opers["/="]["string"] = {  };

      // **=
      opers["**="]["int"] = { "int",  "float",  "bool",  "char" };
      opers["**="]["float"] = { "int",  "float",  "bool",  "char" };
      opers["**="]["bool"] = {  };
      opers["**="]["char"] = {  };
      opers["**="]["string"] = {  };

      // >>=
      opers[">>="]["int"] = { "int",  "float",  "bool",  "char" };
      opers[">>="]["float"] = {  };
      opers[">>="]["bool"] = {  };
      opers[">>="]["char"] = {  };
      opers[">>="]["string"] = {  };

      // <<=
      opers["<<="]["int"] = { "int",  "float",  "bool",  "char" };
      opers["<<="]["float"] = {  };
      opers["<<="]["bool"] = {  };
      opers["<<="]["char"] = { "int",  "float",  "bool",  "char" };
      opers["<<="]["string"] = {  };

      // %=
      opers["%="]["int"] = { "int",  "bool",  "char" };
      opers["%="]["float"] = {  };
      opers["%="]["bool"] = {  };
      opers["%="]["char"] = {  };
      opers["%="]["string"] = {  };

      // &=
      opers["&="]["int"] = { "int",  "float",  "bool",  "char" };
      opers["&="]["float"] = {  };
      opers["&="]["bool"] = {  };
      opers["&="]["char"] = {  };
      opers["&="]["string"] = {  };

      // |=
      opers["|="]["int"] = { "int",  "float",  "bool",  "char" };
      opers["|="]["float"] = {  };
      opers["|="]["bool"] = {  };
      opers["|="]["char"] = {  };
      opers["|="]["string"] = {  };

      // ^=
      opers["^="]["int"] = { "int",  "float",  "bool",  "char" };
      opers["^="]["float"] = {  };
      opers["^="]["bool"] = {  };
      opers["^="]["char"] = {  };
      opers["^="]["string"] = {  };

      // **
      opers["**"]["int"] = { "int",  "float",  "bool",  "char" };
      opers["**"]["float"] = { "int",  "float",  "bool",  "char" };
      opers["**"]["bool"] = {  };
      opers["**"]["char"] = {  };
      opers["**"]["string"] = {  };

      // *
      opers["*"]["int"] = { "int",  "float",  "bool",  "char" };
      opers["*"]["float"] = { "int",  "float",  "bool",  "char" };
      opers["*"]["bool"] = { "int",  "float",  "bool",  "char" };
      opers["*"]["char"] = { "int",  "float",  "bool",  "char" };
      opers["*"]["string"] = { "int" };

      // /
      opers["/"]["int"] = { "int",  "float",  "bool",  "char" };
      opers["/"]["float"] = { "int",  "float",  "bool",  "char" };
      opers["/"]["bool"] = { "int",  "float",  "bool",  "char" };
      opers["/"]["char"] = { "int",  "float",  "bool",  "char" };
      opers["/"]["string"] = {  };

      // %
      opers["%"]["int"] = { "int",  "bool",  "char" };
      opers["%"]["float"] = {  };
      opers["%"]["bool"] = { };
      opers["%"]["char"] = { "int",  "bool",  "char" };
      opers["%"]["string"] = {  };

      // +
      opers["+"]["int"] = { "int",  "float",  "bool",  "char" };
      opers["+"]["float"] = { "int",  "float",  "bool",  "char" };
      opers["+"]["bool"] = { "int",  "float",  "bool",  "char" };
      opers["+"]["char"] = { "int",  "float",  "bool",  "char" };
      opers["+"]["string"] = { "string",  "char" };

      // -
      opers["-"]["int"] = { "int",  "float",  "bool",  "char" };
      opers["-"]["float"] = { "int",  "float",  "bool",  "char" };
      opers["-"]["bool"] = { "int",  "float",  "bool",  "char" };
      opers["-"]["char"] = { "int",  "float",  "bool",  "char" };
      opers["-"]["string"] = {  };

      // >>
      opers[">>"]["int"] = { "int",  "float",  "bool",  "char" };
      opers[">>"]["float"] = {  };
      opers[">>"]["bool"] = {  };
      opers[">>"]["char"] = {  };
      opers[">>"]["string"] = {  };

      // <<
      opers["<<"]["int"] = { "int",  "float",  "bool",  "char" };
      opers["<<"]["float"] = {  };
      opers["<<"]["bool"] = {  };
      opers["<<"]["char"] = {  };
      opers["<<"]["string"] = {  };

      // >
      opers[">"]["int"] = { "int",  "float",  "bool",  "char" };
      opers[">"]["float"] = { "int",  "float",  "bool",  "char" };
      opers[">"]["bool"] = { "int",  "float",  "bool",  "char" };
      opers[">"]["char"] = { "int",  "float",  "bool",  "char" };
      opers[">"]["string"] = { "string" };

      // <
      opers["<"]["int"] = { "int",  "float",  "bool",  "char" };
      opers["<"]["float"] = { "int",  "float",  "bool",  "char" };
      opers["<"]["bool"] = { "int",  "float",  "bool",  "char" };
      opers["<"]["char"] = { "int",  "float",  "bool",  "char" };
      opers["<"]["string"] = { "string" };

      // >=
      opers[">="]["int"] = { "int",  "float",  "bool",  "char" };
      opers[">="]["float"] = { "int",  "float",  "bool",  "char" };
      opers[">="]["bool"] = { "int",  "float",  "bool",  "char" };
      opers[">="]["char"] = { "int",  "float",  "bool",  "char" };
      opers[">="]["string"] = { "string" };

      // <=
      opers["<="]["int"] = { "int",  "float",  "bool",  "char" };
      opers["<="]["float"] = { "int",  "float",  "bool",  "char" };
      opers["<="]["bool"] = { "int",  "float",  "bool",  "char" };
      opers["<="]["char"] = { "int",  "float",  "bool",  "char" };
      opers["<="]["string"] = { "string" };

      // ==
      opers["=="]["int"] = { "int",  "float",  "bool",  "char" };
      opers["=="]["float"] = { "int",  "float",  "bool",  "char" };
      opers["=="]["bool"] = { "int",  "float",  "bool",  "char" };
      opers["=="]["char"] = { "int",  "float",  "bool",  "char" };
      opers["=="]["string"] = { "string" };

      // !=
      opers["!="]["int"] = { "int",  "float",  "bool",  "char" };
      opers["!="]["float"] = { "int",  "float",  "bool",  "char" };
      opers["!="]["bool"] = { "int",  "float",  "bool",  "char" };
      opers["!="]["char"] = { "int",  "float",  "bool",  "char" };
      opers["!="]["string"] = { "string" };

      // &
      opers["&"]["int"] = { "int",  "float",  "bool",  "char" };
      opers["&"]["float"] = {  };
      opers["&"]["bool"] = {  };
      opers["&"]["char"] = {  };
      opers["&"]["string"] = {  };

      // ^
      opers["^"]["int"] = { "int",  "float",  "bool",  "char" };
      opers["^"]["float"] = {  };
      opers["^"]["bool"] = {  };
      opers["^"]["char"] = {  };
      opers["^"]["string"] = {  };

      // |
      opers["|"]["int"] = { "int",  "float",  "bool",  "char" };
      opers["|"]["float"] = {  };
      opers["|"]["bool"] = {  };
      opers["|"]["char"] = {  };
      opers["|"]["string"] = {  };

      // &&
      opers["&"]["int"] = { "int",  "float",  "bool",  "char" };
      opers["&"]["float"] = { "int",  "float",  "bool",  "char" };
      opers["&"]["bool"] = { "int",  "float",  "bool",  "char" };
      opers["&"]["char"] = { "int",  "float",  "bool",  "char" };
      opers["&"]["string"] = {  };

      // ||
      opers["&"]["int"] = { "int",  "float",  "bool",  "char" };
      opers["&"]["float"] = { "int",  "float",  "bool",  "char" };
      opers["&"]["bool"] = { "int",  "float",  "bool",  "char" };
      opers["&"]["char"] = { "int",  "float",  "bool",  "char" };
      opers["&"]["string"] = {  };
    }

    if (find(types.begin(),  types.end(),  type1) != types.end()) {
      if (find(types.begin(),  types.end(),  type2) == types.end()) {
        return "Error";
      }

      if (find(opers[oper][type1].begin(),  opers[oper][type1].end(),  type2) == opers[oper][type1].end()) {
        return "Error";
      }

      if (find(bool_opers.begin(),  bool_opers.end(),  oper) != bool_opers.end()) {
        return "bool";
      }

      if (type1 == "string" || type2 == "string") {
        return "string";
      }

      if (type_size[type1] > type_size[type2]) {
        return type1;
      }
      else {
        return type2;
      }
    }
    else {
      if (find(arr_opers.begin(),  arr_opers.end(),  oper) == arr_opers.end()) {
        return "Error";
      }
      if (type1 != type2) {
        return "Error";
      }
      return type1;
    }
  }

  std::string CheckUnoOperType(std::string type,  std::string oper) {
    std::map < std::string,  std::map<std::string,  std::string>> opers;
    // ++
    opers["++"]["int"] = "int";
    opers["++"]["char"] = "char";
    opers["++"]["bool"] = "Error";
    opers["++"]["float"] = "Error";
    opers["++"]["string"] = "Error";

    // --
    opers["--"]["int"] = "int";
    opers["--"]["char"] = "char";
    opers["--"]["bool"] = "Error";
    opers["--"]["float"] = "Error";
    opers["--"]["string"] = "Error";

    // +
    opers["+"]["int"] = "int";
    opers["+"]["char"] = "Error";
    opers["+"]["bool"] = "Error";
    opers["+"]["float"] = "float";
    opers["+"]["string"] = "Error";

    // -
    opers["-"]["int"] = "int";
    opers["-"]["char"] = "Error";
    opers["-"]["bool"] = "Error";
    opers["-"]["float"] = "float";
    opers["-"]["string"] = "Error";

    // !
    opers["!"]["int"] = "bool";
    opers["!"]["char"] = "bool";
    opers["!"]["bool"] = "bool";
    opers["!"]["float"] = "Error";
    opers["!"]["string"] = "Error";

    // ~
    opers["~"]["int"] = "int";
    opers["~"]["char"] = "char";
    opers["~"]["bool"] = "bool";
    opers["~"]["float"] = "Error";
    opers["~"]["string"] = "Error";

    return opers[type][oper];
  }

  bool CheckVar(std::string name) {
    TID* tmp = cur_tid_;
    while (tmp) {
      if (!tmp->CheckVarNotExist(name)) { return true; }
      tmp = tmp->parent();
    }
    return false;
  }

  bool CheckDeclrVar(std::string name) {
    return cur_tid_->CheckVarNotExist(name);
  }

  void PushVar(std::string name,  std::string type) {
    cur_tid_->PushVar(name,  type);
  }

  bool FuncExist(std::string name,  std::vector<std::string> args) {
    return global_tid_->FuncExist(name,  args);
  }

  void LeaveTid() {
    cur_tid_ = cur_tid_->parent();
  }

  void PushFunc(std::string type,  std::string name,  std::vector<std::string> args) {
    global_tid_->PushFunc(type,  name,  args).tid_;
  }

  void PushTid() {
    cur_tid_ = new TID(cur_tid_);
  }

  std::string VarGetType(std::string name) {
    TID* tmp = cur_tid_;
    while (tmp) {
      if (!tmp->CheckVarNotExist(name)) {
        return tmp->GetVarType(name);
      }
      tmp = tmp->parent();
    }
    return "Error";
  }

  FunctionSecond GetFunc(FunctionFirst func) {
    return global_tid_->GetFunc(cur_tid_->FuncId());
  }

  TID* CurTID() { return cur_tid_; }

private:
  std::stack<TypeOperNode> type_oper_stack_;
  TID* cur_tid_;
  GlobalTID* global_tid_;
};