#pragma once
#include "RPNData.h"

class RPNTID
{
public:
  RPNTID(RPNTID* parent = NULL) : parent_(parent) {}

  void PushBackVar(RPNData* data, std::string name) {
    name_to_ind_[name] = memory_.size();
    memory_.push_back(data);
  }

  void SetData(std::string name, RPNData* data) {
    if (name_to_ind_.find(name) == name_to_ind_.end()) {
      parent_->SetData(name, data);
      return;
    }
    delete memory_[name_to_ind_[name]];
    memory_[name_to_ind_[name]] = data;
  }

  RPNData* GetVar(std::string name) {
    if (name_to_ind_.find(name) == name_to_ind_.end()) {
      return parent_->GetVar(name);
    }
    return memory_[name_to_ind_[name]];
  }

  RPNData GetData(std::string name) {
    if (name_to_ind_.find(name) == name_to_ind_.end()) {
      return parent_->GetData(name);
    }
    return *memory_[name_to_ind_[name]];
  }

  RPNTID* parent() { return parent_; }

  ~RPNTID() {
    for (auto& i : memory_) delete i;
  }
protected:
  std::map<std::string, int> name_to_ind_;
  std::vector<RPNData*> memory_;
  RPNTID* parent_;
};

