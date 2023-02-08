#pragma once

#include "includes.h"


class TID;
struct FunctionFirst;
struct FunctionSecond;

struct FunctionFirst {
  FunctionFirst(std::string name,  std::vector<std::string> args) : name_(name),  args_(args) {}
  std::string name_;
  std::vector<std::string> args_;
};


struct MapCmp {
  bool operator() (const FunctionFirst& a,  const FunctionFirst& b) const {
    if (a.name_ == b.name_) {
      return a.args_ < b.args_;
    }
    return a.name_ < b.name_;
  }
};

struct FunctionSecond {
  FunctionSecond(std::string type = "",  TID* tid = NULL) : type_(type),  tid_(tid) {}
  std::string type_;
  TID* tid_;
};

class TID {
public:
  TID(TID* parent = NULL,  FunctionFirst func = FunctionFirst("",  {})) : parent_(parent),  func_(func) {
  }

  bool CheckVarNotExist(std::string name) {
    return var_name_map_.find(name) == var_name_map_.end();
  }

  void PushVar(std::string name,  std::string type) {
    var_name_map_[name] = type;
    return;
  }

  TID* parent() {
    return parent_;
  }

  std::string GetVarType(std::string name) {
    return var_name_map_[name];
  }

  FunctionFirst FuncId() {
    return func_;
  }

private:
  FunctionFirst func_;
  TID* parent_;
  std::map<std::string,  std::string> var_name_map_;
};

class GlobalTID : public TID {
public:
  GlobalTID() : TID() { funcs_ = std::map <FunctionFirst,  FunctionSecond,  MapCmp>(); }

  FunctionSecond PushFunc(std::string type,  std::string name,  std::vector<std::string> args) {
    return funcs_[FunctionFirst(name,  args)] = FunctionSecond(type,  new TID(this,  FunctionFirst(name,  args)));
  }

  bool FuncExist(std::string name,  std::vector<std::string> args) {
    return funcs_.find({ name,  args }) != funcs_.end();
  }

  FunctionSecond GetFunc(FunctionFirst func) {
    return funcs_[func];
  }


private:
  std::map<std::string,  std::string> var_name_map_;
  std::map <FunctionFirst,  FunctionSecond,  MapCmp> funcs_;
};