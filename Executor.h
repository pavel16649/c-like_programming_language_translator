#pragma once
#include "RPNCreator.h"
#include "ExecTid.h"

class Executor
{
public:
    Executor(std::vector<RPNToken*>& tokens, int main_ind) : tokens_(tokens) {
        cur_token_ = main_ind;
        tid_ = NULL;
    }

    RPNToken* Token() { return tokens_[cur_token_]; }
    RPNTokenTypeEnum TokenType() { return tokens_[cur_token_]->Type(); }
    void TokenNext() { cur_token_++; }
    void TokenPrev() { cur_token_--; }

    bool CastBool(Data*);
    char CastChar(Data*);
    int CastInt(Data*);
    double CastFloat(Data*);
    std::string CastString(Data*);
    Data* CopyData(Data*);

    void Work();
    void BLANK();
    void GOTO();
    void FALSE_GOTO();
    void BACK_GOTO();
    void CONST();
    void VARIABLE();
    void TID_IN();
    void TID_OUT();
    void FUNC_CALL();
    void ARG_GET();
    void END();
    void INIT();
    void ADDRESS();
    void PRINT();
    void KILL();
    void FUNC_IN();
    void FUNC_OUT();
    void PUT_TO_ARRAY();
    void CREATE_ARRAY();
    void OPERATION();

    // Operations
    Data* INCREMENT();
    Data* DECREMENT();
    Data* NOT();
    Data* BIT_NOT();
    Data* UNARY_MINUS();
    Data* UNARY_PLUS();

    Data* PLUS();
    Data* MINUS();
    Data* MULT();
    Data* DIV();
    Data* POW();
    Data* MOD();

    Data* BIT_LEFT();
    Data* BIT_RIGHT();
    Data* BIT_OR();
    Data* BIT_AND();
    Data* BIT_XOR();

    Data* OR();
    Data* AND();

    Data* EQLS();
    Data* NOT_EQLS();
    Data* LESS();
    Data* MORE();
    Data* EQLS_LESS();
    Data* EQLS_MORE();

    Data* ASSIGN();
    Data* ASSIGN_PLUS();
    Data* ASSIGN_MINUS();
    Data* ASSIGN_MULT();
    Data* ASSIGN_DIV();
    Data* ASSIGN_MOD();
    Data* ASSIGN_POW();
    Data* ASSIGN_BIT_LEFT();
    Data* ASSIGN_BIT_RIGHT();
    Data* ASSIGN_BIT_OR();
    Data* ASSIGN_BIT_AND();
    Data* ASSIGN_BIT_XOR();

private:
    std::vector<RPNToken*>& tokens_;
    std::stack<Data*> stack_;
    std::stack<int> back_stack_, args_stack_;
    int cur_token_;
    ExecTid* tid_;
};

