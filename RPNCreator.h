#pragma once
#include "includes.h"
#include "RPNToken.h"
#include "Token.h"
#include "TID_RPN.h"



class RPNCreator
{
public:

    struct FuncId {
        FuncId() {}
        FuncId(std::string name,  std::vector<RPNType> args) :
            name_(name),  args_(args)
        {}
        void SetName(std::string name) { name_ = name; }
        void PushArg(RPNType type) { args_.push_back(type); }
        std::string name_;
        std::vector<RPNType> args_;
    };

    struct FuncData {
        FuncData() {}
        FuncData(RPNType type,  int address) : type_(type),  address_(address) {}
        FuncData& operator=(FuncData& other) {
            type_ = other.type_;
            address_ = other.address_;
            return *this;
        }
        RPNType type_;
        int address_;
    };

    struct FuncComp {
        bool operator() (const FuncId& a,  const FuncId& b) const {
            if (a.name_ != b.name_)
                return a.name_ < b.name_;
            if (a.args_.size() != b.args_.size())
                return a.args_.size() < b.args_.size();
            for (int i = 0; i < a.args_.size(); ++i) {
                if (a.args_[i].Type() != b.args_[i].Type())
                    return a.args_[i].Type() < b.args_[i].Type();
                if (a.args_[i].Dimension() != b.args_[i].Dimension())
                    return a.args_[i].Dimension() < b.args_[i].Dimension();
            }
            return false;
        }
    };

    enum class TokenTypes {
        reserved = 1, 
        name = 2, 
        constant = 3, 
        operation = 4
    };

    RPNCreator(std::vector<Token>& input) :
        input_(input),  RPN_(*new std::vector<RPNToken*>)
    {
        tid_ = new TID_RPN();
        cur_token_ = 0;
        cur_rpn_ = 0;
        Work();
    }

    std::vector<RPNToken*>& GetRPN() { return RPN_; }
    int GetMain() { return main_; }

    std::string TokenName() {
        return input_[cur_token_].lecsema();
    }
    TokenTypes TokenType() { return TokenTypes(input_[cur_token_].type()); }
    void TokenNext() { cur_token_++; }
    void TokenPrev() { cur_token_--; }
    void Push(RPNToken* token) {
        RPN_.push_back(token);
        cur_rpn_++;
    }
    void Insert(RPNToken* token,  int address) {
        delete RPN_[address];
        RPN_[address] = token;
    }
    int CurRpnInd() { return cur_rpn_; }
    void AddFunc(FuncId id,  FuncData data) {
        funcs_[id] = data;
    }

    void SetBreaks(int address) {
        for (auto& i : breaks_.top()) {
            RPN_[i] = new RPNAddress(address);
        }
        breaks_.pop();
    }

    void SetConts(int address) {
        for (auto& i : conts_.top()) {
            RPN_[i] = new RPNAddress(address);
        }
        conts_.pop();
    }

    void Show();
    void TidIn();
    void TidOut();
    RPNType CalcType(std::vector<RPNToken*>& local_rpn);
    RPNType* MakeOperation(RPNType*,  RPNType*,  RPNOperation*);

    void SetPriors();
    bool IsUnary(OperationEnum type);
    bool IsRightAssoc(OperationEnum type);
    Data* TokenToString(std::string);
    Data* TokenToChar(std::string);
    Data* TokenToBool(std::string);
    Data* TokenToFloat(std::string);
    Data* TokenToInt(std::string);


    void Work();
    void FunctionDeclaration(RPNType);
    void VariableDeclaration(RPNType);
    RPNType Type();
    RPNType Expression();
    RPNType Array();
    RPNType FuncCall();
    void CompoundOperator();
    void DerivedOperator();
    bool Operator();
    void If();
    void While();
    void For();
    void DoWhile();
    void Print();

protected:
    std::vector<RPNToken*>& RPN_;
    std::vector<Token>& input_;
    std::stack<std::vector<int>> breaks_,  conts_;
    int cur_token_,  cur_rpn_;
    std::map<FuncId,  FuncData,  FuncComp> funcs_;
    std::vector<int> oper_prior_;
    TID_RPN* tid_;
    int main_;
};

