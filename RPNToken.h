#pragma once

#include "includes.h"
#include "Data.h"

enum class OperationEnum {

    BRACE, 

    INCREMENT,  DECREMENT,  NOT,  BIT_NOT,  UNARY_MINUS,  UNARY_PLUS, 

    EQLS,  PLUS,  MINUS,  MULT,  DIV,  MOD,  BIT_LEFT,  BIT_RIGHT,  POW,  OR,  AND, 
    BIT_OR,  BIT_AND,  BIT_XOR,  NOT_EQLS,  LESS,  MORE,  EQLS_LESS,  EQLS_MORE, 

    ASSIGN,  ASSIGN_PLUS,  ASSIGN_MINUS,  ASSIGN_MULT,  ASSIGN_DIV,  ASSIGN_MOD, 
    ASSIGN_POW,  ASSIGN_BIT_LEFT,  ASSIGN_BIT_RIGHT,  ASSIGN_BIT_OR,  ASSIGN_BIT_AND, 
    ASSIGN_BIT_XOR

};

enum class RPNTokenTypeEnum {
    BLANK,  GOTO,  FALSE_GOTO,  BACK_GOTO, 
    CONST,  VARIABLE,  TID_IN,  TID_OUT, 
    FUNC_CALL,  ARG_GET, 
    END,  OPERATION,  TYPE,  INIT,  ADDRESS, 
    PRINT,  KILL,  FUNC_IN,  FUNC_OUT,  PUT_TO_ARRAY, 
    CREATE_ARRAY
};

class RPNToken
{
public:
    RPNToken() : type_(RPNTokenTypeEnum::BLANK) {}
    RPNToken(RPNTokenTypeEnum type) : type_(type) {}
    RPNTokenTypeEnum Type() { return type_; }
    virtual void Show() {}
protected:
    RPNTokenTypeEnum type_;
};

class RPNBlank : public RPNToken {
public:
    RPNBlank() : RPNToken(RPNTokenTypeEnum::BLANK) {}
    virtual void Show() { std::cout << "Blank\n"; }
};

class RPNType : public RPNToken {
public:
    RPNType() : base_type_(DataTypesEnum::VOID),  dimension_(0) {}
    RPNType(std::string type) : RPNToken(RPNTokenTypeEnum::TYPE) {
        if (type == "bool") {
            base_type_ = DataTypesEnum::BOOL;
        }
        if (type == "char") {
            base_type_ = DataTypesEnum::CHAR;
        }
        if (type == "int") {
            base_type_ = DataTypesEnum::INT;
        }
        if (type == "float") {
            base_type_ = DataTypesEnum::FLOAT;
        }
        if (type == "string") {
            base_type_ = DataTypesEnum::STRING;
        }
        if (type == "void") {
            base_type_ = DataTypesEnum::VOID;
        }
        dimension_ = 0;
    }

    RPNType(DataTypesEnum base_type,  int dimension) :
        base_type_(base_type),  dimension_(dimension), 
        RPNToken(RPNTokenTypeEnum::TYPE)
    {}

    RPNType(const RPNType& other) {
        base_type_ = other.base_type_;
        dimension_ = other.dimension_;
        RPNToken::type_ = other.RPNToken::type_;
    }

    RPNType(RPNType&& other) {
        base_type_ = other.base_type_;
        dimension_ = other.dimension_;
        RPNToken::type_ = other.RPNToken::type_;
    }

    RPNType& operator=(RPNType other) {
        base_type_ = other.base_type_;
        dimension_ = other.dimension_;
        RPNToken::type_ = other.RPNToken::type_;
        return *this;
    }

    DataTypesEnum Type() const { return base_type_; }

    int Dimension() const { return dimension_; }

    void AddDimension() { dimension_++; }

    virtual void Show() {
        std::cout << "Type: ";
        if (base_type_ == DataTypesEnum::BOOL) std::cout << "bool";
        if (base_type_ == DataTypesEnum::CHAR) std::cout << "char";
        if (base_type_ == DataTypesEnum::INT) std::cout << "int";
        if (base_type_ == DataTypesEnum::FLOAT) std::cout << "float";
        if (base_type_ == DataTypesEnum::STRING) std::cout << "string";
        std::cout << ". Dimension: " << dimension_ << '\n';
    }

protected:
    DataTypesEnum base_type_;
    int dimension_;
};

class RPNInit : public RPNToken {
public:
    RPNInit(std::string name,  RPNType type) : type_(type),  name_(name),  RPNToken(RPNTokenTypeEnum::INIT) {}
    virtual void Show() {
        std::cout << "Init: " << name_ << " Type: ";
        type_.Show();
    }
    std::string Name() { return name_; }
    RPNType Type() { return type_; }
protected:
    std::string name_;
    RPNType type_;
};

class RPNTidIn : public RPNToken {
public:
    RPNTidIn() : RPNToken(RPNTokenTypeEnum::TID_IN) {}
    virtual void Show() {
        std::cout << "Tid in\n";
    }
};

class RPNTidOut : public RPNToken {
public:
    RPNTidOut() : RPNToken(RPNTokenTypeEnum::TID_OUT) {}
    virtual void Show() {
        std::cout << "Tid out\n";
    }
};

class RPNArgGet : public RPNToken {
public:
    RPNArgGet(std::string name) : name_(name),  RPNToken(RPNTokenTypeEnum::ARG_GET) {}
    virtual void Show() {
        std::cout << "Get argument\n";
    }
    std::string Name() { return name_; }
private:
    std::string name_;
};

class RPNFalseGoto : public RPNToken {
public:
    RPNFalseGoto() : RPNToken(RPNTokenTypeEnum::FALSE_GOTO) {}
    virtual void Show() {
        std::cout << "False GOTO\n";
    }
};

class RPNGoto : public RPNToken {
public:
    RPNGoto() : RPNToken(RPNTokenTypeEnum::GOTO) {}
    virtual void Show() {
        std::cout << "GOTO\n";
    }
};

class RPNBackGoto : public RPNToken {
public:
    RPNBackGoto() : RPNToken(RPNTokenTypeEnum::BACK_GOTO) {}
    virtual void Show() {
        std::cout << "Back GOTO\n";
    }
};

class RPNAddress : public RPNToken {
public:
    RPNAddress(int address) : address_(address),  RPNToken(RPNTokenTypeEnum::ADDRESS) {}
    virtual void Show() {
        std::cout << "Address: " << address_ << '\n';
    }
    int Address() { return address_; }
protected:
    int address_;
};

class RPNConst : public RPNToken {
public:
    RPNConst(Data* data) : data_(data),  RPNToken(RPNTokenTypeEnum::CONST) {}
    virtual void Show() {
        std::cout << "Const: ";
        data_->Show();
        std::cout << '\n';
    }
    Data* Get() { return data_; }
protected:
    Data* data_;
};

class RPNPrint : public RPNToken {
public:
    RPNPrint() : RPNToken(RPNTokenTypeEnum::PRINT) {}
    virtual void Show() {
        std::cout << "Print\n";
    }
};

class RPNKill : public RPNToken {
public:
    RPNKill() : RPNToken(RPNTokenTypeEnum::KILL) {}
    virtual void Show() {
        std::cout << "Delete from stack\n";
    }
};

class RPNFuncIn : public RPNToken {
public:
    RPNFuncIn() : RPNToken(RPNTokenTypeEnum::FUNC_IN) {}
    virtual void Show() {
        std::cout << "Function in\n";
    }
};

class RPNFuncOut : public RPNToken {
public:
    RPNFuncOut() : RPNToken(RPNTokenTypeEnum::FUNC_OUT) {}
    virtual void Show() {
        std::cout << "Function out\n";
    }
};

class RPNOperation : public RPNToken {
public:
    RPNOperation(std::string oper) : RPNToken(RPNTokenTypeEnum::OPERATION) {
        if (oper == "(") type_ = OperationEnum::BRACE;
        if (oper == "++") type_ = OperationEnum::INCREMENT;
        if (oper == "--") type_ = OperationEnum::DECREMENT;
        if (oper == "!") type_ = OperationEnum::NOT;
        if (oper == "~") type_ = OperationEnum::BIT_NOT;
        if (oper == "_") type_ = OperationEnum::UNARY_MINUS;
        if (oper == "@") type_ = OperationEnum::UNARY_PLUS;
        if (oper == "==") type_ = OperationEnum::EQLS;
        if (oper == "+") type_ = OperationEnum::PLUS;
        if (oper == "-") type_ = OperationEnum::MINUS;
        if (oper == "*") type_ = OperationEnum::MULT;
        if (oper == "/") type_ = OperationEnum::DIV;
        if (oper == "%") type_ = OperationEnum::MOD;
        if (oper == "<<") type_ = OperationEnum::BIT_LEFT;
        if (oper == ">>") type_ = OperationEnum::BIT_RIGHT;
        if (oper == "**") type_ = OperationEnum::POW;
        if (oper == "||") type_ = OperationEnum::OR;
        if (oper == "&&") type_ = OperationEnum::AND;
        if (oper == "|") type_ = OperationEnum::BIT_OR;
        if (oper == "&") type_ = OperationEnum::BIT_AND;
        if (oper == "^") type_ = OperationEnum::BIT_XOR;
        if (oper == "!=") type_ = OperationEnum::NOT_EQLS;
        if (oper == "<") type_ = OperationEnum::LESS;
        if (oper == ">") type_ = OperationEnum::MORE;
        if (oper == "<=") type_ = OperationEnum::EQLS_LESS;
        if (oper == ">=") type_ = OperationEnum::EQLS_MORE;
        if (oper == "=") type_ = OperationEnum::ASSIGN;
        if (oper == "+=") type_ = OperationEnum::ASSIGN_PLUS;
        if (oper == "-=") type_ = OperationEnum::ASSIGN_MINUS;
        if (oper == "*=") type_ = OperationEnum::ASSIGN_MULT;
        if (oper == "/=") type_ = OperationEnum::ASSIGN_DIV;
        if (oper == "%=") type_ = OperationEnum::ASSIGN_MOD;
        if (oper == "**=") type_ = OperationEnum::ASSIGN_POW;
        if (oper == "<<=") type_ = OperationEnum::ASSIGN_BIT_LEFT;
        if (oper == ">>=") type_ = OperationEnum::ASSIGN_BIT_RIGHT;
        if (oper == "|=") type_ = OperationEnum::ASSIGN_BIT_OR;
        if (oper == "&=") type_ = OperationEnum::ASSIGN_BIT_AND;
        if (oper == "^=") type_ = OperationEnum::ASSIGN_BIT_XOR;
    }
    RPNOperation(OperationEnum type) : type_(type),  RPNToken(RPNTokenTypeEnum::OPERATION) {}
    OperationEnum Type() { return type_; }

    virtual void Show() {
        std::cout << "Operation: " << int(type_) << '\n';
    }
private:
    OperationEnum type_;
};

class RPNVariable : public RPNToken {
public:
    RPNVariable(std::string name) : name_(name),  RPNToken(RPNTokenTypeEnum::VARIABLE) {}
    virtual void Show() {
        std::cout << "Variable: " << name_ << '\n';
    }
    std::string Name() { return name_; }
private:
    std::string name_;
};

class RPNFuncCall : public RPNToken {
public:
    RPNFuncCall(int address,  int back_address,  int args_num) :
        address_(address),  back_address_(back_address),  args_num_(args_num), 
        RPNToken(RPNTokenTypeEnum::FUNC_CALL) {}
    virtual void Show() {
        std::cout << "Function call. Address: " << address_ <<
            ". Back address: " << back_address_ << ". Args number: " << args_num_ << '\n';
    }
    int Address() { return address_; }
    int BackAddress() { return back_address_; }
    int ArgsNum() { return args_num_; }
private:
    int address_;
    int back_address_;
    int args_num_;
};

class RPNEnd : public RPNToken {
public:
    RPNEnd() : RPNToken(RPNTokenTypeEnum::END) {}
    virtual void Show() {
        std::cout << "The END\n";
    }
};

class RPNCreateArray : public RPNToken {
public:
    RPNCreateArray(RPNType type) : type_(type),  RPNToken(RPNTokenTypeEnum::CREATE_ARRAY) {}
private:
    RPNType type_;
};

class RPNPutToArray : public RPNToken {
public:
    RPNPutToArray() : RPNToken(RPNTokenTypeEnum::PUT_TO_ARRAY) {}
};