#pragma once
#include "includes.h"
#include "Data.h"
#include "RPNToken.h"

class ExecTid
{
public:
    ExecTid(ExecTid* parent = NULL) : parent_(parent) {}
    void Init(std::string name, RPNType type) {
        Data* data = NULL;
        if (type.Dimension() != 0) {
            data = new DataArray(type.Type(), type.Dimension());
        }
        else if (type.Type() == DataTypesEnum::BOOL) {
            data = new DataBool();
        }
        else if (type.Type() == DataTypesEnum::CHAR) {
            data = new DataChar();
        }
        else if (type.Type() == DataTypesEnum::INT) {
            data = new DataInt();
        }
        else if (type.Type() == DataTypesEnum::FLOAT) {
            data = new DataFloat();
        }
        else if (type.Type() == DataTypesEnum::STRING) {
            data = new DataString();
        }
        memory_[name] = new DataVar(data);
    }
    void SetData(std::string name, Data* data) {
        memory_[name]->Set(data);
    }
    DataVar* GetVar(std::string name) {
        if (memory_.find(name) == memory_.end()) {
            return parent_->GetVar(name);
        }
        return memory_[name];
    }
    ExecTid* Parent() { return parent_; }
private:
    ExecTid* parent_;
    std::map<std::string, DataVar*> memory_;
};

