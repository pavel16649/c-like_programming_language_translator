#include "Executor.h"

void Executor::Work() {
    back_stack_.push(tokens_.size() - 1);
    for (; TokenType() != RPNTokenTypeEnum::END; TokenNext()) {
        switch (TokenType())
        {
        case RPNTokenTypeEnum::BLANK:
            BLANK();
            break;
        case RPNTokenTypeEnum::GOTO:
            GOTO();
            break;
        case RPNTokenTypeEnum::FALSE_GOTO:
            FALSE_GOTO();
            break;
        case RPNTokenTypeEnum::BACK_GOTO:
            BACK_GOTO();
            break;
        case RPNTokenTypeEnum::CONST:
            CONST();
            break;
        case RPNTokenTypeEnum::VARIABLE:
            VARIABLE();
            break;
        case RPNTokenTypeEnum::TID_IN:
            TID_IN();
            break;
        case RPNTokenTypeEnum::TID_OUT:
            TID_OUT();
            break;
        case RPNTokenTypeEnum::FUNC_CALL:
            FUNC_CALL();
            break;
        case RPNTokenTypeEnum::ARG_GET:
            ARG_GET();
            break;
        case RPNTokenTypeEnum::END:
            END();
            break;
        case RPNTokenTypeEnum::OPERATION:
            OPERATION();
            break;
        case RPNTokenTypeEnum::INIT:
            INIT();
            break;
        case RPNTokenTypeEnum::ADDRESS:
            ADDRESS();
            break;
        case RPNTokenTypeEnum::PRINT:
            PRINT();
            break;
        case RPNTokenTypeEnum::KILL:
            KILL();
            break;
        case RPNTokenTypeEnum::FUNC_IN:
            FUNC_IN();
            break;
        case RPNTokenTypeEnum::FUNC_OUT:
            FUNC_OUT();
            break;
        case RPNTokenTypeEnum::PUT_TO_ARRAY:
            PUT_TO_ARRAY();
            break;
        case RPNTokenTypeEnum::CREATE_ARRAY:
            CREATE_ARRAY();
            break;
        default:
            throw "default";
            break;
        }
    }
}

bool Executor::CastBool(Data* data) {
    if (data->Type() == DataTypesEnum::BOOL) {
        return static_cast<DataBool*>(data)->Get();
    }
    if (data->Type() == DataTypesEnum::CHAR) {
        return static_cast<DataChar*>(data)->Get();
    }
    if (data->Type() == DataTypesEnum::INT) {
        return static_cast<DataInt*>(data)->Get();
    }
    if (data->Type() == DataTypesEnum::FLOAT) {
        return static_cast<DataFloat*>(data)->Get();
    }
}

char Executor::CastChar(Data* data) {
    if (data->Type() == DataTypesEnum::BOOL) {
        return static_cast<DataBool*>(data)->Get();
    }
    if (data->Type() == DataTypesEnum::CHAR) {
        return static_cast<DataChar*>(data)->Get();
    }
    if (data->Type() == DataTypesEnum::INT) {
        return static_cast<DataInt*>(data)->Get();
    }
    if (data->Type() == DataTypesEnum::FLOAT) {
        return static_cast<DataFloat*>(data)->Get();
    }
}

int Executor::CastInt(Data* data) {
    if (data->Type() == DataTypesEnum::BOOL) {
        return static_cast<DataBool*>(data)->Get();
    }
    if (data->Type() == DataTypesEnum::CHAR) {
        return static_cast<DataChar*>(data)->Get();
    }
    if (data->Type() == DataTypesEnum::INT) {
        return static_cast<DataInt*>(data)->Get();
    }
    if (data->Type() == DataTypesEnum::FLOAT) {
        return static_cast<DataFloat*>(data)->Get();
    }
}

double Executor::CastFloat(Data* data) {
    if (data->Type() == DataTypesEnum::BOOL) {
        return static_cast<DataBool*>(data)->Get();
    }
    if (data->Type() == DataTypesEnum::CHAR) {
        return static_cast<DataChar*>(data)->Get();
    }
    if (data->Type() == DataTypesEnum::INT) {
        return static_cast<DataInt*>(data)->Get();
    }
    if (data->Type() == DataTypesEnum::FLOAT) {
        return static_cast<DataFloat*>(data)->Get();
    }
}

std::string Executor::CastString(Data* data) {
    return static_cast<DataString*>(data)->Get();
}

Data* Executor::CopyData(Data* data) {
    Data* ret = NULL;
    if (data->Type() == DataTypesEnum::BOOL) {
        ret = new DataBool(CastBool(data));
    }
    if (data->Type() == DataTypesEnum::CHAR) {
        ret = new DataChar(CastChar(data));
    }
    if (data->Type() == DataTypesEnum::INT) {
        ret = new DataInt(CastInt(data));
    }
    if (data->Type() == DataTypesEnum::FLOAT) {
        ret = new DataFloat(CastFloat(data));
    }
    if (data->Type() == DataTypesEnum::STRING) {
        ret = new DataString(CastString(data));
    }
    return ret;
}

void Executor::BLANK() {
    throw "blank";
}

void Executor::GOTO() {
    int address = CastInt(stack_.top());
    stack_.pop();
    cur_token_ = address;
    TokenPrev();
}

void Executor::FALSE_GOTO() {
    int address = CastInt(stack_.top());
    stack_.pop();
    bool expr = CastBool(stack_.top());
    stack_.pop();
    if (!expr) {
        cur_token_ = address;
        TokenPrev();
    }
}

void Executor::BACK_GOTO() {
    int address = back_stack_.top();
    back_stack_.pop();
    cur_token_ = address;
    TokenPrev();
}

void Executor::CONST() {
    Data* cnst = static_cast<RPNConst*>(tokens_[cur_token_])->Get();
    Data* copied = CopyData(cnst);
    stack_.push(copied);
}

void Executor::VARIABLE() {
    stack_.push(tid_->GetVar(static_cast<RPNVariable*>(Token())->Name()));
}

void Executor::TID_IN() {
    tid_ = new ExecTid(tid_);
}

void Executor::TID_OUT() {
    ExecTid* tmp = tid_;
    tid_ = tmp->Parent();
    delete tmp;
}

void Executor::FUNC_CALL() {
    back_stack_.push(static_cast<RPNFuncCall*>(Token())->BackAddress());
    args_stack_.push(static_cast<RPNFuncCall*>(Token())->ArgsNum());
    cur_token_ = static_cast<RPNFuncCall*>(Token())->Address();
    TokenPrev();
}

void Executor::ARG_GET() {
    int args_num = args_stack_.top();
    if (args_num == 0) {
        return;
    }
    std::stack<Data*> tmp_st;
    while (args_num--) {
        tmp_st.push(stack_.top());
        stack_.pop();
    }
    std::string var_name = static_cast<RPNArgGet*>(Token())->Name();
    tid_->SetData(var_name, tmp_st.top());
    tmp_st.pop();
    while (!tmp_st.empty()) {
        stack_.push(tmp_st.top());
        tmp_st.pop();
    }
}

void Executor::END() {
    throw "END";
}

void Executor::INIT() {
    RPNInit* token = static_cast<RPNInit*>(Token());
    tid_->Init(token->Name(), token->Type());
}

void Executor::ADDRESS() {
    int address = static_cast<RPNAddress*>(Token())->Address();
    stack_.push(new DataInt(address));
}

void Executor::PRINT() {
    stack_.top()->Show();
    stack_.pop();
}

void Executor::KILL() {
    stack_.pop();
}

void Executor::FUNC_IN() {
    TID_IN();
}

void Executor::FUNC_OUT() {
    TID_OUT();
}

void Executor::PUT_TO_ARRAY() {
    Data* data = stack_.top();
    stack_.pop();
    DataArray* arr = static_cast<DataArray*>(stack_.top());
    arr->Push(data);
}

void Executor::CREATE_ARRAY() {
    stack_.push(new DataArray(std::vector<Data*>(0)));
}

void Executor::OPERATION() {
    OperationEnum oper = static_cast<RPNOperation*>(Token())->Type();
    Data* data;
    switch (oper)
    {
    case OperationEnum::INCREMENT:
        data = INCREMENT();
        break;
    case OperationEnum::DECREMENT:
        data = DECREMENT();
        break;
    case OperationEnum::NOT:
        data = NOT();
        break;
    case OperationEnum::BIT_NOT:
        data = BIT_NOT();
        break;
    case OperationEnum::UNARY_MINUS:
        data = UNARY_MINUS();
        break;
    case OperationEnum::UNARY_PLUS:
        data = UNARY_PLUS();
        break;
    case OperationEnum::EQLS:
        data = EQLS();
        break;
    case OperationEnum::PLUS:
        data = PLUS();
        break;
    case OperationEnum::MINUS:
        data = MINUS();
        break;
    case OperationEnum::MULT:
        data = MULT();
        break;
    case OperationEnum::DIV:
        data = DIV();
        break;
    case OperationEnum::MOD:
        data = MOD();
        break;
    case OperationEnum::BIT_LEFT:
        data = BIT_LEFT();
        break;
    case OperationEnum::BIT_RIGHT:
        data = BIT_RIGHT();
        break;
    case OperationEnum::POW:
        data = POW();
        break;
    case OperationEnum::OR:
        data = OR();
        break;
    case OperationEnum::AND:
        data = AND();
        break;
    case OperationEnum::BIT_OR:
        data = BIT_OR();
        break;
    case OperationEnum::BIT_AND:
        data = BIT_AND();
        break;
    case OperationEnum::BIT_XOR:
        data = BIT_XOR();
        break;
    case OperationEnum::NOT_EQLS:
        data = NOT_EQLS();
        break;
    case OperationEnum::LESS:
        data = LESS();
        break;
    case OperationEnum::MORE:
        data = MORE();
        break;
    case OperationEnum::EQLS_LESS:
        data = EQLS_LESS();
        break;
    case OperationEnum::EQLS_MORE:
        data = EQLS_MORE();
        break;
    case OperationEnum::ASSIGN:
        data = ASSIGN();
        break;
    case OperationEnum::ASSIGN_PLUS:
        data = ASSIGN_PLUS();
        break;
    case OperationEnum::ASSIGN_MINUS:
        data = ASSIGN_MINUS();
        break;
    case OperationEnum::ASSIGN_MULT:
        data = ASSIGN_MULT();
        break;
    case OperationEnum::ASSIGN_DIV:
        data = ASSIGN_DIV();
        break;
    case OperationEnum::ASSIGN_MOD:
        data = ASSIGN_MOD();
        break;
    case OperationEnum::ASSIGN_POW:
        data = ASSIGN_POW();
        break;
    case OperationEnum::ASSIGN_BIT_LEFT:
        data = ASSIGN_BIT_LEFT();
        break;
    case OperationEnum::ASSIGN_BIT_RIGHT:
        data = ASSIGN_BIT_RIGHT();
        break;
    case OperationEnum::ASSIGN_BIT_OR:
        data = ASSIGN_BIT_OR();
        break;
    case OperationEnum::ASSIGN_BIT_AND:
        data = ASSIGN_BIT_AND();
        break;
    case OperationEnum::ASSIGN_BIT_XOR:
        data = ASSIGN_BIT_XOR();
        break;
    default:
        break;
    }
    stack_.push(data);
}
/*
Data* TEMPLATE() {
    Data* first, * second;
    second = stack_.top();
    stack_.pop();
    first = stack_.top();
    stack_.pop();

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->Get();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();

    Data* ret;

    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res;
            ret = new DataBool(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res;
            ret = new DataChar(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res;
            ret = new DataInt(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            double res;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            double res;
            ret = new DataFloat(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            double res;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}*/

Data* Executor::INCREMENT() {
    DataVar* var = static_cast<DataVar*>(stack_.top());
    stack_.pop();
    DataTypesEnum type = var->Get()->Type();
    switch (type)
    {
    case DataTypesEnum::BOOL:
        static_cast<DataBool*>(var->Get())->Set(static_cast<DataBool*>(var->Get())->Get() + 1);
        break;
    case DataTypesEnum::CHAR:
        static_cast<DataChar*>(var->Get())->Set(static_cast<DataChar*>(var->Get())->Get() + 1);
        break;
    case DataTypesEnum::INT:
        static_cast<DataInt*>(var->Get())->Set(static_cast<DataInt*>(var->Get())->Get() + 1);
        break;
    case DataTypesEnum::FLOAT:
        static_cast<DataFloat*>(var->Get())->Set(static_cast<DataFloat*>(var->Get())->Get() + 1);
        break;
    case DataTypesEnum::STRING:
        throw "String Increment";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Increment";
        break;
    case DataTypesEnum::VOID:
        throw "Void Increment";
        break;
    case DataTypesEnum::VAR:
        throw "Var Increment";
        break;
    default:
        break;
    }
    return var;
}

Data* Executor::DECREMENT() {
    DataVar* var = static_cast<DataVar*>(stack_.top());
    stack_.pop();
    DataTypesEnum type = var->Get()->Type();
    switch (type)
    {
    case DataTypesEnum::BOOL:
        static_cast<DataBool*>(var->Get())->Set(static_cast<DataBool*>(var->Get())->Get() - 1);
        break;
    case DataTypesEnum::CHAR:
        static_cast<DataChar*>(var->Get())->Set(static_cast<DataChar*>(var->Get())->Get() - 1);
        break;
    case DataTypesEnum::INT:
        static_cast<DataInt*>(var->Get())->Set(static_cast<DataInt*>(var->Get())->Get() - 1);
        break;
    case DataTypesEnum::FLOAT:
        static_cast<DataFloat*>(var->Get())->Set(static_cast<DataFloat*>(var->Get())->Get() - 1);
        break;
    case DataTypesEnum::STRING:
        throw "String Decrement";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Decrement";
        break;
    case DataTypesEnum::VOID:
        throw "Void Decrement";
        break;
    case DataTypesEnum::VAR:
        throw "Var Decrement";
        break;
    default:
        break;
    }
    return var;
}

Data* Executor::NOT() {
    Data* data = stack_.top();
    stack_.pop();
    if (data->Type() == DataTypesEnum::VAR) data = static_cast<DataVar*>(data)->Get();
    DataTypesEnum type = data->Type();
    Data* ret = NULL;
    switch (type)
    {
    case DataTypesEnum::BOOL:
    {
        bool num = static_cast<DataBool*>(data)->Get();
        num = !num;
        ret = new DataBool(num);
    }
    break;
    case DataTypesEnum::CHAR:
    {
        bool num = static_cast<DataChar*>(data)->Get();
        num = !num;
        ret = new DataBool(num);
    }
    break;
    case DataTypesEnum::INT:
    {
        bool num = static_cast<DataInt*>(data)->Get();
        num = !num;
        ret = new DataBool(num);
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        bool num = static_cast<DataFloat*>(data)->Get();
        num = !num;
        ret = new DataBool(num);
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Not";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Not";
        break;
    case DataTypesEnum::VOID:
        throw "Void Not";
        break;
    case DataTypesEnum::VAR:
        throw "Var Not";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::BIT_NOT() {
    Data* data = stack_.top();
    stack_.pop();
    if (data->Type() == DataTypesEnum::VAR) data = static_cast<DataVar*>(data)->Get();
    DataTypesEnum type = data->Type();
    Data* ret = NULL;
    switch (type)
    {
    case DataTypesEnum::BOOL:
    {
        bool num = static_cast<DataBool*>(data)->Get();
        num = ~num;
        ret = new DataBool(num);
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char num = static_cast<DataChar*>(data)->Get();
        num = ~num;
        ret = new DataChar(num);
    }
    break;
    case DataTypesEnum::INT:
    {
        int num = static_cast<DataInt*>(data)->Get();
        num = ~num;
        ret = new DataInt(num);
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double num = static_cast<DataFloat*>(data)->Get();
        int64_t tmp = reinterpret_cast<double&>(num);
        tmp = ~tmp;
        num = reinterpret_cast<int64_t&>(tmp);
        ret = new DataFloat(num);
    }
    break;
    case DataTypesEnum::STRING:
        throw "String BitNot";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array BitNot";
        break;
    case DataTypesEnum::VOID:
        throw "Void Bitnot";
        break;
    case DataTypesEnum::VAR:
        throw "Var BitNot";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::UNARY_PLUS() {
    Data* data = stack_.top();
    stack_.pop();
    if (data->Type() == DataTypesEnum::VAR) data = static_cast<DataVar*>(data)->Get();
    DataTypesEnum type = data->Type();
    Data* ret = NULL;
    switch (type)
    {
    case DataTypesEnum::BOOL:
    {
        bool num = static_cast<DataBool*>(data)->Get();
        num = +num;
        ret = new DataBool(num);
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char num = static_cast<DataChar*>(data)->Get();
        num = +num;
        ret = new DataBool(num);
    }
    break;
    case DataTypesEnum::INT:
    {
        int num = static_cast<DataInt*>(data)->Get();
        num = +num;
        ret = new DataInt(num);
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double num = static_cast<DataFloat*>(data)->Get();
        num = +num;
        ret = new DataFloat(num);
    }
    break;
    case DataTypesEnum::STRING:
        throw "String UnPlus";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array UnPlus";
        break;
    case DataTypesEnum::VOID:
        throw "Void UnPlus";
        break;
    case DataTypesEnum::VAR:
        throw "Var UnPlus";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::UNARY_MINUS() {
    Data* data = stack_.top();
    stack_.pop();
    if (data->Type() == DataTypesEnum::VAR) data = static_cast<DataVar*>(data)->Get();
    DataTypesEnum type = data->Type();
    Data* ret = NULL;
    switch (type)
    {
    case DataTypesEnum::BOOL:
    {
        bool num = static_cast<DataBool*>(data)->Get();
        num = -num;
        ret = new DataBool(num);
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char num = static_cast<DataChar*>(data)->Get();
        num = -num;
        ret = new DataBool(num);
    }
    break;
    case DataTypesEnum::INT:
    {
        int num = static_cast<DataInt*>(data)->Get();
        num = -num;
        ret = new DataInt(num);
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double num = static_cast<DataFloat*>(data)->Get();
        num = -num;
        ret = new DataFloat(num);
    }
    break;
    case DataTypesEnum::STRING:
        throw "String UnPlus";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array UnPlus";
        break;
    case DataTypesEnum::VOID:
        throw "Void UnPlus";
        break;
    case DataTypesEnum::VAR:
        throw "Var UnPlus";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::PLUS() {
    Data* first, * second;
    second = stack_.top();
    stack_.pop();
    first = stack_.top();
    stack_.pop();

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->Get();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();

    Data* ret = NULL;

    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num + s_num;
            ret = new DataBool(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num + s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num + s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num + s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num + s_num;
            ret = new DataChar(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num + s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num + s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num + s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
        {
            std::string s_num = static_cast<DataString*>(second)->Get();
            std::string res = "" + f_num + s_num;
            ret = new DataString(res);
        }
        break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num + s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num + s_num;
            ret = new DataInt(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num + s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num + s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            double res = f_num + s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            double res = f_num + s_num;
            ret = new DataFloat(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            double res = f_num + s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num + s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
    {
        std::string f_num = static_cast<DataString*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
            break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            std::string res = f_num + s_num;
            ret = new DataString(res);
        }
        break;
        case DataTypesEnum::INT:
            break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
        {
            std::string s_num = static_cast<DataString*>(second)->Get();
            std::string res = f_num + s_num;
            ret = new DataString(res);
        }
        break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::MINUS() {
    Data* first, * second;
    second = stack_.top();
    stack_.pop();
    first = stack_.top();
    stack_.pop();

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->Get();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();

    Data* ret = NULL;

    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num - s_num;
            ret = new DataBool(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num - s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num - s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num - s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num - s_num;
            ret = new DataChar(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num - s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num - s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num - s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num - s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num - s_num;
            ret = new DataInt(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num - s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num - s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            double res = f_num - s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            double res = f_num - s_num;
            ret = new DataFloat(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            double res = f_num - s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num - s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::MULT() {
    Data* first, * second;
    second = stack_.top();
    stack_.pop();
    first = stack_.top();
    stack_.pop();

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->Get();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();

    Data* ret = NULL;

    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num * s_num;
            ret = new DataBool(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num * s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num * s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num * s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num * s_num;
            ret = new DataChar(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num * s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num * s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num * s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num * s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num * s_num;
            ret = new DataInt(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num * s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num * s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            double res = f_num * s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            double res = f_num * s_num;
            ret = new DataFloat(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            double res = f_num * s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num * s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::DIV() {
    Data* first, * second;
    second = stack_.top();
    stack_.pop();
    first = stack_.top();
    stack_.pop();

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->Get();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();

    Data* ret = NULL;

    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num / s_num;
            ret = new DataBool(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num / s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num / s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num / s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num / s_num;
            ret = new DataChar(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num / s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num / s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num / s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num / s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num / s_num;
            ret = new DataInt(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num / s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num / s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            double res = f_num / s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            double res = f_num / s_num;
            ret = new DataFloat(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            double res = f_num / s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num / s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::POW() {
    Data* first, * second;
    second = stack_.top();
    stack_.pop();
    first = stack_.top();
    stack_.pop();

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->Get();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();

    Data* ret = NULL;

    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = std::pow(f_num, s_num);
            ret = new DataBool(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = std::pow(f_num, s_num);
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = std::pow(f_num, s_num);
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = std::pow(f_num, s_num);
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = std::pow(f_num, s_num);
            ret = new DataChar(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = std::pow(f_num, s_num);
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = std::pow(f_num, s_num);
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = std::pow(f_num, s_num);
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = std::pow(f_num, s_num);
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = std::pow(f_num, s_num);
            ret = new DataInt(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = std::pow(f_num, s_num);
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = std::pow(f_num, s_num);
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            double res = std::pow(f_num, s_num);
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            double res = std::pow(f_num, s_num);
            ret = new DataFloat(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            double res = std::pow(f_num, s_num);
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = std::pow(f_num, s_num);
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::MOD() {
    Data* first, * second;
    second = stack_.top();
    stack_.pop();
    first = stack_.top();
    stack_.pop();

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->Get();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();

    Data* ret = NULL;

    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num % s_num;
            ret = new DataBool(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num % s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num % s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num % s_num;
            ret = new DataChar(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num % s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num % s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num % s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num % s_num;
            ret = new DataInt(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num % s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
        break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}


Data* Executor::BIT_LEFT() {
    Data* first, * second;
    second = stack_.top();
    stack_.pop();
    first = stack_.top();
    stack_.pop();

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->Get();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();

    Data* ret = NULL;

    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num << s_num;
            ret = new DataBool(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num << s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num << s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num << s_num;
            ret = new DataChar(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num << s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num << s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num << s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num << s_num;
            ret = new DataInt(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num << s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num) << s_num;
            double res = reinterpret_cast<double&>(tmp);
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num) << s_num;
            double res = reinterpret_cast<double&>(tmp);
            ret = new DataFloat(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num) << s_num;
            double res = reinterpret_cast<double&>(tmp);
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::BIT_RIGHT() {
    Data* first, * second;
    second = stack_.top();
    stack_.pop();
    first = stack_.top();
    stack_.pop();

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->Get();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();

    Data* ret = NULL;

    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num >> s_num;
            ret = new DataBool(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num >> s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num >> s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num >> s_num;
            ret = new DataChar(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num >> s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num >> s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num >> s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num >> s_num;
            ret = new DataInt(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num >> s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num) >> s_num;
            double res = reinterpret_cast<double&>(tmp);
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num) >> s_num;
            double res = reinterpret_cast<double&>(tmp);
            ret = new DataFloat(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num) >> s_num;
            double res = reinterpret_cast<double&>(tmp);
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::BIT_OR() {
    Data* first, * second;
    second = stack_.top();
    stack_.pop();
    first = stack_.top();
    stack_.pop();

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->Get();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();

    Data* ret = NULL;

    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num | s_num;
            ret = new DataBool(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num | s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num | s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num | s_num;
            ret = new DataChar(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num | s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num | s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num | s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num | s_num;
            ret = new DataInt(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num | s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num) | s_num;
            double res = reinterpret_cast<double&>(tmp);
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num) | s_num;
            double res = reinterpret_cast<double&>(tmp);
            ret = new DataFloat(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num) | s_num;
            double res = reinterpret_cast<double&>(tmp);
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::BIT_AND() {
    Data* first, * second;
    second = stack_.top();
    stack_.pop();
    first = stack_.top();
    stack_.pop();

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->Get();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();

    Data* ret = NULL;

    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num & s_num;
            ret = new DataBool(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num & s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num & s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num & s_num;
            ret = new DataChar(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num & s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num & s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num & s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num & s_num;
            ret = new DataInt(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num & s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num) & s_num;
            double res = reinterpret_cast<double&>(tmp);
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num) & s_num;
            double res = reinterpret_cast<double&>(tmp);
            ret = new DataFloat(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num) & s_num;
            double res = reinterpret_cast<double&>(tmp);
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::BIT_XOR() {
    Data* first, * second;
    second = stack_.top();
    stack_.pop();
    first = stack_.top();
    stack_.pop();

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->Get();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();

    Data* ret = NULL;

    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num ^ s_num;
            ret = new DataBool(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num ^ s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num ^ s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num ^ s_num;
            ret = new DataChar(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num ^ s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num ^ s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num ^ s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num ^ s_num;
            ret = new DataInt(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num ^ s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num) ^ s_num;
            double res = reinterpret_cast<double&>(tmp);
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num) ^ s_num;
            double res = reinterpret_cast<double&>(tmp);
            ret = new DataFloat(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num) ^ s_num;
            double res = reinterpret_cast<double&>(tmp);
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}


Data* Executor::OR() {
    Data* first, * second;
    second = stack_.top();
    stack_.pop();
    first = stack_.top();
    stack_.pop();

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->Get();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();

    Data* ret = NULL;

    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num || s_num;
            ret = new DataBool(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num || s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num || s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num || s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num || s_num;
            ret = new DataChar(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num || s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num || s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num || s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num || s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num || s_num;
            ret = new DataInt(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num || s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num || s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            double res = f_num || s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            double res = f_num || s_num;
            ret = new DataFloat(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            double res = f_num || s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num || s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::AND() {
    Data* first, * second;
    second = stack_.top();
    stack_.pop();
    first = stack_.top();
    stack_.pop();

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->Get();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();

    Data* ret = NULL;

    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num && s_num;
            ret = new DataBool(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num && s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num && s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num && s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num && s_num;
            ret = new DataChar(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num && s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num && s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num && s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num && s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num && s_num;
            ret = new DataInt(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num && s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num && s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            double res = f_num && s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            double res = f_num && s_num;
            ret = new DataFloat(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            double res = f_num && s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num && s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}


Data* Executor::EQLS() {
    Data* first, * second;
    second = stack_.top();
    stack_.pop();
    first = stack_.top();
    stack_.pop();

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->Get();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();

    Data* ret = NULL;

    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num == s_num;
            ret = new DataBool(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num == s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num == s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num == s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num == s_num;
            ret = new DataChar(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num == s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num == s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num == s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num == s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num == s_num;
            ret = new DataInt(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num == s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num == s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            double res = f_num == s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            double res = f_num == s_num;
            ret = new DataFloat(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            double res = f_num == s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num == s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::NOT_EQLS() {
    Data* first, * second;
    second = stack_.top();
    stack_.pop();
    first = stack_.top();
    stack_.pop();

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->Get();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();

    Data* ret = NULL;

    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num != s_num;
            ret = new DataBool(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num != s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num != s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num != s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num != s_num;
            ret = new DataChar(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num != s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num != s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num != s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num != s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num != s_num;
            ret = new DataInt(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num != s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num != s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            double res = f_num != s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            double res = f_num != s_num;
            ret = new DataFloat(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            double res = f_num != s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num != s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::LESS() {
    Data* first, * second;
    second = stack_.top();
    stack_.pop();
    first = stack_.top();
    stack_.pop();

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->Get();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();

    Data* ret = NULL;

    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num < s_num;
            ret = new DataBool(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num < s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num < s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num < s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num < s_num;
            ret = new DataChar(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num < s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num < s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num < s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num < s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num < s_num;
            ret = new DataInt(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num < s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num < s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            double res = f_num < s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            double res = f_num < s_num;
            ret = new DataFloat(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            double res = f_num < s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num < s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::MORE() {
    Data* first, * second;
    second = stack_.top();
    stack_.pop();
    first = stack_.top();
    stack_.pop();

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->Get();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();

    Data* ret = NULL;

    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num > s_num;
            ret = new DataBool(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num > s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num > s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num > s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num > s_num;
            ret = new DataChar(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num > s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num > s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num > s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num > s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num > s_num;
            ret = new DataInt(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num > s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num > s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            double res = f_num > s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            double res = f_num > s_num;
            ret = new DataFloat(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            double res = f_num > s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num > s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::EQLS_LESS() {
    Data* first, * second;
    second = stack_.top();
    stack_.pop();
    first = stack_.top();
    stack_.pop();

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->Get();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();

    Data* ret = NULL;

    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num <= s_num;
            ret = new DataBool(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num <= s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num <= s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num <= s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num <= s_num;
            ret = new DataChar(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num <= s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num <= s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num <= s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num <= s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num <= s_num;
            ret = new DataInt(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num <= s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num <= s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            double res = f_num <= s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            double res = f_num <= s_num;
            ret = new DataFloat(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            double res = f_num <= s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num <= s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::EQLS_MORE() {
    Data* first, * second;
    second = stack_.top();
    stack_.pop();
    first = stack_.top();
    stack_.pop();

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->Get();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();

    Data* ret = NULL;

    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num >= s_num;
            ret = new DataBool(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num >= s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num >= s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num >= s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num >= s_num;
            ret = new DataChar(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num >= s_num;
            ret = new DataChar(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num >= s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num >= s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num >= s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num >= s_num;
            ret = new DataInt(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num >= s_num;
            ret = new DataInt(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num >= s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            double res = f_num >= s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            double res = f_num >= s_num;
            ret = new DataFloat(s_num);
        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            double res = f_num >= s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num >= s_num;
            ret = new DataFloat(res);
        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}


Data* Executor::ASSIGN() {
    Data* second;
    second = stack_.top();
    stack_.pop();
    Data*& first = stack_.top();
    stack_.pop();

    Data* ret = first;

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->GetLink();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();


    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            double res = s_num;
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            double res = s_num;
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            double res = s_num;
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = s_num;
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::ASSIGN_PLUS() {
    Data* second;
    second = stack_.top();
    stack_.pop();
    Data*& first = stack_.top();
    stack_.pop();

    Data* ret = first;

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->GetLink();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();


    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num + s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num + s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num + s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num + s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num + s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num + s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num + s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num + s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num + s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num + s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num + s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num + s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            double res = f_num + s_num;
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            double res = f_num + s_num;
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            double res = f_num + s_num;
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num + s_num;
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::ASSIGN_MINUS() {
    Data* second;
    second = stack_.top();
    stack_.pop();
    Data*& first = stack_.top();
    stack_.pop();

    Data* ret = first;

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->GetLink();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();


    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num - s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num - s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num - s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num - s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num - s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num - s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num - s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num - s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num - s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num - s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num - s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num - s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            double res = f_num - s_num;
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            double res = f_num - s_num;
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            double res = f_num - s_num;
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num - s_num;
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::ASSIGN_MULT() {
    Data* second;
    second = stack_.top();
    stack_.pop();
    Data*& first = stack_.top();
    stack_.pop();

    Data* ret = first;

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->GetLink();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();


    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num * s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num * s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num * s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num * s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num * s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num * s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num * s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num * s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num * s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num * s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num * s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num * s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            double res = f_num * s_num;
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            double res = f_num * s_num;
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            double res = f_num * s_num;
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num * s_num;
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::ASSIGN_DIV() {
    Data* second;
    second = stack_.top();
    stack_.pop();
    Data*& first = stack_.top();
    stack_.pop();

    Data* ret = first;

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->GetLink();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();


    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num / s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num / s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num / s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num / s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num / s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num / s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num / s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num / s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num / s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num / s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num / s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num / s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            double res = f_num / s_num;
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            double res = f_num / s_num;
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            double res = f_num / s_num;
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = f_num / s_num;
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::ASSIGN_MOD() {
    Data* second;
    second = stack_.top();
    stack_.pop();
    Data*& first = stack_.top();
    stack_.pop();

    Data* ret = first;

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->GetLink();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();


    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num % s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num % s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num % s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num % s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num % s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num % s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num % s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num % s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num % s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
        break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::ASSIGN_POW() {
    Data* second;
    second = stack_.top();
    stack_.pop();
    Data*& first = stack_.top();
    stack_.pop();

    Data* ret = first;

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->GetLink();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();


    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = std::pow(f_num, s_num);
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = std::pow(f_num, s_num);
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = std::pow(f_num, s_num);
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = std::pow(f_num, s_num);
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = std::pow(f_num, s_num);
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = std::pow(f_num, s_num);
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = std::pow(f_num, s_num);
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = std::pow(f_num, s_num);
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = std::pow(f_num, s_num);
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = std::pow(f_num, s_num);
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = std::pow(f_num, s_num);
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = std::pow(f_num, s_num);
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            double res = std::pow(f_num, s_num);
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            double res = std::pow(f_num, s_num);
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            double res = std::pow(f_num, s_num);
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double s_num = static_cast<DataFloat*>(second)->Get();
            double res = std::pow(f_num, s_num);
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::ASSIGN_BIT_LEFT() {
    Data* second;
    second = stack_.top();
    stack_.pop();
    Data*& first = stack_.top();
    stack_.pop();

    Data* ret = first;

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->GetLink();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();


    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num << s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num << s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num << s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num << s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num << s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num << s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num << s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num << s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num << s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num);
            tmp <<= s_num;
            double res = reinterpret_cast<double&>(s_num);
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num);
            tmp <<= s_num;
            double res = reinterpret_cast<double&>(s_num);
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num);
            tmp <<= s_num;
            double res = reinterpret_cast<double&>(s_num);
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::ASSIGN_BIT_RIGHT() {
    Data* second;
    second = stack_.top();
    stack_.pop();
    Data*& first = stack_.top();
    stack_.pop();

    Data* ret = first;

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->GetLink();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();


    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num >> s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num >> s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num >> s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num >> s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num >> s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num >> s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num >> s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num >> s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num >> s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num);
            tmp >>= s_num;
            double res = reinterpret_cast<double&>(s_num);
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num);
            tmp >>= s_num;
            double res = reinterpret_cast<double&>(s_num);
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num);
            tmp >>= s_num;
            double res = reinterpret_cast<double&>(s_num);
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::ASSIGN_BIT_OR() {
    {
        Data* second;
        second = stack_.top();
        stack_.pop();
        Data*& first = stack_.top();
        stack_.pop();

        Data* ret = first;

        if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->GetLink();
        if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

        DataTypesEnum f_type = first->Type(), s_type = second->Type();


        switch (f_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool f_num = static_cast<DataBool*>(first)->Get();
            switch (s_type)
            {
            case DataTypesEnum::BOOL:
            {
                bool s_num = static_cast<DataBool*>(second)->Get();
                bool res = f_num | s_num;
                static_cast<DataBool*>(first)->Set(res);

            }
            break;
            case DataTypesEnum::CHAR:
            {
                char s_num = static_cast<DataChar*>(second)->Get();
                char res = f_num | s_num;
                static_cast<DataBool*>(first)->Set(res);

            }
            break;
            case DataTypesEnum::INT:
            {
                int s_num = static_cast<DataInt*>(second)->Get();
                int res = f_num | s_num;
                static_cast<DataBool*>(first)->Set(res);

            }
            break;
            case DataTypesEnum::FLOAT:
                break;
            case DataTypesEnum::STRING:
                break;
            case DataTypesEnum::ARRAY:
                break;
            case DataTypesEnum::VOID:
                break;
            case DataTypesEnum::VAR:
                break;
            default:
                break;
            }
        }
        break;
        case DataTypesEnum::CHAR:
        {
            char f_num = static_cast<DataChar*>(first)->Get();
            switch (s_type)
            {
            case DataTypesEnum::BOOL:
            {
                bool s_num = static_cast<DataBool*>(second)->Get();
                char res = f_num | s_num;
                static_cast<DataChar*>(first)->Set(res);

            }
            break;
            case DataTypesEnum::CHAR:
            {
                char s_num = static_cast<DataChar*>(second)->Get();
                char res = f_num | s_num;
                static_cast<DataChar*>(first)->Set(res);

            }
            break;
            case DataTypesEnum::INT:
            {
                int s_num = static_cast<DataInt*>(second)->Get();
                int res = f_num | s_num;
                static_cast<DataChar*>(first)->Set(res);

            }
            break;
            case DataTypesEnum::FLOAT:
                break;
            case DataTypesEnum::STRING:
                break;
            case DataTypesEnum::ARRAY:
                break;
            case DataTypesEnum::VOID:
                break;
            case DataTypesEnum::VAR:
                break;
            default:
                break;
            }
        }
        break;
        case DataTypesEnum::INT:
        {
            int f_num = static_cast<DataInt*>(first)->Get();
            switch (s_type)
            {
            case DataTypesEnum::BOOL:
            {
                bool s_num = static_cast<DataBool*>(second)->Get();
                int res = f_num | s_num;
                static_cast<DataInt*>(first)->Set(res);

            }
            break;
            case DataTypesEnum::CHAR:
            {
                char s_num = static_cast<DataChar*>(second)->Get();
                int res = f_num | s_num;
                static_cast<DataInt*>(first)->Set(res);

            }
            break;
            case DataTypesEnum::INT:
            {
                int s_num = static_cast<DataInt*>(second)->Get();
                int res = f_num | s_num;
                static_cast<DataInt*>(first)->Set(res);

            }
            break;
            case DataTypesEnum::FLOAT:
                break;
            case DataTypesEnum::STRING:
                break;
            case DataTypesEnum::ARRAY:
                break;
            case DataTypesEnum::VOID:
                break;
            case DataTypesEnum::VAR:
                break;
            default:
                break;
            }
        }
        break;
        case DataTypesEnum::FLOAT:
        {
            double f_num = static_cast<DataFloat*>(first)->Get();
            switch (s_type)
            {
            case DataTypesEnum::BOOL:
            {
                bool s_num = static_cast<DataBool*>(second)->Get();
                int64_t tmp = reinterpret_cast<int64_t&>(f_num);
                tmp |= s_num;
                double res = reinterpret_cast<double&>(s_num);
                static_cast<DataFloat*>(first)->Set(res);

            }
            break;
            case DataTypesEnum::CHAR:
            {
                char s_num = static_cast<DataChar*>(second)->Get();
                int64_t tmp = reinterpret_cast<int64_t&>(f_num);
                tmp |= s_num;
                double res = reinterpret_cast<double&>(s_num);
                static_cast<DataFloat*>(first)->Set(res);

            }
            break;
            case DataTypesEnum::INT:
            {
                int s_num = static_cast<DataInt*>(second)->Get();
                int64_t tmp = reinterpret_cast<int64_t&>(f_num);
                tmp |= s_num;
                double res = reinterpret_cast<double&>(s_num);
                static_cast<DataFloat*>(first)->Set(res);

            }
            break;
            case DataTypesEnum::FLOAT:
                break;
            case DataTypesEnum::STRING:
                break;
            case DataTypesEnum::ARRAY:
                break;
            case DataTypesEnum::VOID:
                break;
            case DataTypesEnum::VAR:
                break;
            default:
                break;
            }
        }
        break;
        case DataTypesEnum::STRING:
            throw "String Smth";
            break;
        case DataTypesEnum::ARRAY:
            throw "Array Smth";
            break;
        case DataTypesEnum::VOID:
            throw "Void Smth";
            break;
        case DataTypesEnum::VAR:
            throw "Var Smth";
            break;
        default:
            break;
        }
        return ret;
    }
}

Data* Executor::ASSIGN_BIT_AND() {
    Data* second;
    second = stack_.top();
    stack_.pop();
    Data*& first = stack_.top();
    stack_.pop();

    Data* ret = first;

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->GetLink();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();


    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num & s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num & s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num & s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num & s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num & s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num & s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num & s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num & s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num & s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num);
            tmp &= s_num;
            double res = reinterpret_cast<double&>(s_num);
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num);
            tmp &= s_num;
            double res = reinterpret_cast<double&>(s_num);
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num);
            tmp &= s_num;
            double res = reinterpret_cast<double&>(s_num);
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}

Data* Executor::ASSIGN_BIT_XOR() {
    Data* second;
    second = stack_.top();
    stack_.pop();
    Data*& first = stack_.top();
    stack_.pop();

    Data* ret = first;

    if (first->Type() == DataTypesEnum::VAR) first = static_cast<DataVar*>(first)->GetLink();
    if (second->Type() == DataTypesEnum::VAR) second = static_cast<DataVar*>(second)->Get();

    DataTypesEnum f_type = first->Type(), s_type = second->Type();


    switch (f_type)
    {
    case DataTypesEnum::BOOL:
    {
        bool f_num = static_cast<DataBool*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            bool res = f_num ^ s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num ^ s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num ^ s_num;
            static_cast<DataBool*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::CHAR:
    {
        char f_num = static_cast<DataChar*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            char res = f_num ^ s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            char res = f_num ^ s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num ^ s_num;
            static_cast<DataChar*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::INT:
    {
        int f_num = static_cast<DataInt*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int res = f_num ^ s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int res = f_num ^ s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int res = f_num ^ s_num;
            static_cast<DataInt*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::FLOAT:
    {
        double f_num = static_cast<DataFloat*>(first)->Get();
        switch (s_type)
        {
        case DataTypesEnum::BOOL:
        {
            bool s_num = static_cast<DataBool*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num);
            tmp ^= s_num;
            double res = reinterpret_cast<double&>(s_num);
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::CHAR:
        {
            char s_num = static_cast<DataChar*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num);
            tmp ^= s_num;
            double res = reinterpret_cast<double&>(s_num);
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::INT:
        {
            int s_num = static_cast<DataInt*>(second)->Get();
            int64_t tmp = reinterpret_cast<int64_t&>(f_num);
            tmp ^= s_num;
            double res = reinterpret_cast<double&>(s_num);
            static_cast<DataFloat*>(first)->Set(res);

        }
        break;
        case DataTypesEnum::FLOAT:
            break;
        case DataTypesEnum::STRING:
            break;
        case DataTypesEnum::ARRAY:
            break;
        case DataTypesEnum::VOID:
            break;
        case DataTypesEnum::VAR:
            break;
        default:
            break;
        }
    }
    break;
    case DataTypesEnum::STRING:
        throw "String Smth";
        break;
    case DataTypesEnum::ARRAY:
        throw "Array Smth";
        break;
    case DataTypesEnum::VOID:
        throw "Void Smth";
        break;
    case DataTypesEnum::VAR:
        throw "Var Smth";
        break;
    default:
        break;
    }
    return ret;
}