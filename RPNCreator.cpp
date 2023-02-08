#include "RPNCreator.h"

void RPNCreator::Work() {

    SetPriors();

    while (cur_token_ < input_.size()) {
        RPNType type = Type();
        TokenNext();
        if (TokenName() == "(") {
            TokenPrev();
            if (TokenName() == "main") main_ = CurRpnInd();
            FunctionDeclaration(type);
        }
        else {
            TokenPrev();
            VariableDeclaration(type);
        }
    }
    Push(new RPNEnd());
}

void RPNCreator::Show() {
    int cnt = 0;
    for (auto& token : RPN_) {
        std::cout << cnt++ << ". ";
        token->Show();
    }
}

void RPNCreator::TidIn() {
    tid_ = new TID_RPN(tid_);
}

void RPNCreator::TidOut() {
    TID_RPN* tmp = tid_;
    tid_ = tid_->Parent();
    delete tmp;
}

RPNType RPNCreator::Type() {
    if (TokenName() != "array") {
        std::string cur_type = TokenName();
        TokenNext();
        return RPNType(cur_type);
    }
    TokenNext();
    TokenNext();
    RPNType ret = Type();
    ret.AddDimension();
    TokenNext();
    return ret;
}

void RPNCreator::VariableDeclaration(RPNType cur_type) {
    while (true) {
        Push(new RPNInit(TokenName(), cur_type));
        tid_->SetVar(TokenName(), cur_type);
        TokenNext();
        if (TokenName() == "=") {
            TokenPrev();
            Expression();
        }
        if (TokenName() == ";") break;
        TokenNext();
    }
}

void RPNCreator::FunctionDeclaration(RPNType cur_type) {
    std::string name = TokenName();
    TokenNext();
    int address = CurRpnInd();
    Push(new RPNFuncIn());
    TidIn();
    FuncId id;
    id.SetName(name);
    while (TokenName() != ")") {
        TokenNext();
        if (TokenName() == ")") {
            break;
        }
        RPNType arg_type = Type();
        tid_->SetVar(TokenName(), arg_type);
        std::string a_name = TokenName();
        Push(new RPNInit(a_name, arg_type));
        TokenNext();
        if (TokenName() == "=") {
            AddFunc(id, FuncData(cur_type, address));
            TokenPrev();
            Expression();
        }
        id.PushArg(arg_type);
        Push(new RPNArgGet(a_name));
    }
    AddFunc(id, FuncData(cur_type, address));
    TokenNext();
    CompoundOperator();
    Push(new RPNFuncOut());
    Push(new RPNBackGoto());
    TidOut();
}

void RPNCreator::CompoundOperator() {
    Push(new RPNTidIn());
    TidIn();
    TokenNext();
    while (TokenName() != "}") {
        DerivedOperator();
    }
    TokenNext();
    Push(new RPNTidOut());
    TidOut();
}

void RPNCreator::DerivedOperator() {
    if (TokenName() == "if") If();
    else if (TokenName() == "while") While();
    else if (TokenName() == "for") For();
    else if (TokenName() == "do") DoWhile();
    //else if (TokenName() == "switch") Switch();
    else if (TokenName() == "print") Print();
    else if (TokenName() == "{") CompoundOperator();
    else {
        bool to_kill = Operator();
        if (to_kill)
            Push(new RPNKill());
    }
}

bool RPNCreator::Operator() {
    bool to_kill = false;
    if (TokenType() == TokenTypes::reserved) {
        if (TokenName() == "break") {
            breaks_.top().push_back(CurRpnInd());
            Push(new RPNTidOut());
            Push(new RPNBlank());
            Push(new RPNGoto());
            TokenNext();
        }
        else if (TokenName() == "continue") {
            conts_.top().push_back(CurRpnInd());
            Push(new RPNBlank());
            Push(new RPNGoto());
            TokenNext();
        }
        else if (TokenName() == "return") {
            TokenNext();
            Expression();
            Push(new RPNTidOut());
            Push(new RPNFuncOut());
            Push(new RPNBackGoto());
        }
        else {
            VariableDeclaration(Type());
        }
    }
    else {
        RPNType type = Expression();
        if (type.Type() != DataTypesEnum::VOID) {
            to_kill = true;
        }
    }
    TokenNext(); // from ; to smth
    return to_kill;
}

void RPNCreator::If() {
    TokenNext(); // from if to (
    TokenNext(); // from ( to expr
    Expression();
    TokenNext(); // from ) to {
    int p2 = CurRpnInd();
    Push(new RPNBlank());
    Push(new RPNFalseGoto());

    Push(new RPNTidIn());
    TidIn();
    DerivedOperator();
    Push(new RPNTidOut());
    TidOut();

    Insert(new RPNAddress(CurRpnInd()), p2);
    if (TokenName() == "else") {
        int p3 = CurRpnInd();
        Push(new RPNBlank());
        Push(new RPNGoto());

        Push(new RPNTidIn());
        TidIn();
        DerivedOperator();
        Push(new RPNTidOut());
        TidOut();

        Insert(new RPNAddress(CurRpnInd()), p3);
    }
}

void RPNCreator::While() {
    breaks_.push(std::vector<int>());
    conts_.push(std::vector<int>());
    TokenNext(); //from while to (
    int p1 = CurRpnInd();
    TokenNext(); // from ( to expr
    Expression();
    TokenNext(); // from ) to {
    int p2 = CurRpnInd();
    Push(new RPNBlank());
    Push(new RPNFalseGoto());

    Push(new RPNTidIn());
    TidIn();
    DerivedOperator();
    Push(new RPNTidOut());
    TidOut();

    Push(new RPNAddress(p1));
    Push(new RPNGoto());

    Insert(new RPNAddress(CurRpnInd()), p2);
    SetBreaks(CurRpnInd());
    SetConts(p1);
}

void RPNCreator::For() {
    Push(new RPNTidIn());
    TidIn();

    breaks_.push(std::vector<int>());
    conts_.push(std::vector<int>());
    TokenNext(); // from for to (
    TokenNext(); // from ( to oper
    if (TokenName() != ";") {
        Operator();
    }
    else {
        TokenNext();
    }
    int p1 = CurRpnInd();
    if (TokenName() != ";") {
        Expression();
    }
    else {
        Push(new RPNConst(new DataBool(true)));
    }
    TokenNext(); // from ; to expr
    int p2 = CurRpnInd();
    Push(new RPNBlank());
    Push(new RPNFalseGoto());
    int p3 = CurRpnInd();
    Push(new RPNBlank());
    Push(new RPNGoto());
    int p4 = CurRpnInd();
    if (TokenName() != ")") {
        Expression();
    }
    Push(new RPNAddress(p1));
    Push(new RPNGoto());
    TokenNext();
    Insert(new RPNAddress(CurRpnInd()), p3);

    Push(new RPNTidIn());
    TidIn();
    DerivedOperator();
    Push(new RPNTidOut());
    TidOut();

    Push(new RPNAddress(p4));
    Push(new RPNGoto());
    Insert(new RPNAddress(CurRpnInd()), p2);

    Push(new RPNTidOut());
    TidOut();

    SetBreaks(CurRpnInd());
    SetConts(p4);
}

void RPNCreator::DoWhile() {
    breaks_.push(std::vector<int>());
    conts_.push(std::vector<int>());
    TokenNext(); // from do to smth
    int p1 = CurRpnInd();

    Push(new RPNTidIn());
    TidIn();
    DerivedOperator();
    Push(new RPNTidOut());
    TidOut();

    int p2 = CurRpnInd();
    TokenNext(); // from while to (
    TokenNext(); // from ( to expr
    Expression();
    TokenNext(); // from ( to ;
    TokenNext(); // from ; to smth
    int p3 = CurRpnInd();
    Push(new RPNBlank());
    Push(new RPNFalseGoto());
    Push(new RPNAddress(p1));
    Push(new RPNGoto());
    Insert(new RPNAddress(CurRpnInd()), p3);

    SetBreaks(CurRpnInd());
    SetConts(p2);
}

void RPNCreator::Print() {
    TokenNext(); // from print to (
    while (TokenName() != ")") {
        TokenNext();
        if (TokenName() == ")") break;
        Expression();
        Push(new RPNPrint());
    }
    TokenNext(); // from ) to ;
    TokenNext(); // from ; to smth
}

RPNType RPNCreator::FuncCall() {
    std::string name = TokenName();
    std::vector<RPNType> args;
    TokenNext(); // from name to (
    int cnt = 0;
    while (TokenName() != ")") {
        TokenNext();
        if (TokenName() == ")") break;
        cnt++;
        args.push_back(Expression());
    }
    funcs_[FuncId(name, args)];
    int func_ind = funcs_[FuncId(name, args)].address_;
    Push(new RPNFuncCall(func_ind, CurRpnInd() + 1, cnt));
    return funcs_[FuncId(name, args)].type_;
}

RPNType RPNCreator::Array() {
    Push(new RPNBlank());
    TokenNext();
    RPNType type;
    while (TokenName() != "}") {
        type = Expression();
        Push(new RPNPutToArray());
        if (TokenName() == "}") {
            break;
        }
    }
    TokenNext();
    type.AddDimension();
    return type;
}

RPNType RPNCreator::Expression() {
    std::stack<RPNOperation*> Stack;
    std::vector<RPNToken*> local_rpn;
    int balance = 0;
    int pref_func = false;
    for (; !(TokenName() == ";" || TokenName() == "," || TokenName() == "{" || TokenName() == ")" && balance == 0); TokenNext()) {
        if (TokenType() == TokenTypes::operation) {

            RPNOperation* oper;
            if (TokenName() == "-" || TokenName() == "+") {
                TokenPrev();
                if (TokenType() != TokenTypes::constant && TokenType() != TokenTypes::name && !pref_func) {
                    TokenNext();
                    oper = new RPNOperation((TokenName() == "+" ? "@" : "_"));
                }
                else {
                    TokenNext();
                    oper = new RPNOperation(TokenName());
                }
            }
            else {
                oper = new RPNOperation(TokenName());
            }

            if (IsRightAssoc(oper->Type())) {
                while (!Stack.empty() && oper_prior_[int(Stack.top()->Type())] < oper_prior_[int(oper->Type())]) {
                    Push(Stack.top());
                    local_rpn.push_back(Stack.top());
                    Stack.pop();
                }
            }
            else {
                while (!Stack.empty() && oper_prior_[int(Stack.top()->Type())] <= oper_prior_[int(oper->Type())]) {
                    Push(Stack.top());
                    local_rpn.push_back(Stack.top());
                    Stack.pop();
                }
            }
            Stack.push(oper);
            pref_func = false;

        }
        else if (TokenName() == "{") {
            RPNType type = Array();
        }
        else if (TokenType() == TokenTypes::name) {

            TokenNext();
            if (TokenName() == "(") {
                TokenPrev();
                RPNType type = FuncCall();
                local_rpn.push_back(new RPNType(type));
                pref_func = true;
            }
            else {
                TokenPrev();
                Push(new RPNVariable(TokenName()));
                local_rpn.push_back(new RPNType(tid_->GetVar(TokenName())));
                pref_func = false;
            }

        }
        else if (TokenType() == TokenTypes::constant || TokenType() == TokenTypes::reserved) {

            std::string name = TokenName();
            Data* data = NULL;
            if (name[0] == '\"') {
                data = TokenToString(name);
                local_rpn.push_back(new RPNType(DataTypesEnum::STRING, 0));
            }
            else if (name[0] == '\'') {
                data = TokenToChar(name);
                local_rpn.push_back(new RPNType(DataTypesEnum::CHAR, 0));
            }
            else if (name == "true" || name == "false") {
                data = TokenToBool(name);
                local_rpn.push_back(new RPNType(DataTypesEnum::BOOL, 0));
            }
            else if (std::find(name.begin(), name.end(), '.') != name.end()) {
                data = TokenToFloat(name);
                local_rpn.push_back(new RPNType(DataTypesEnum::FLOAT, 0));
            }
            else {
                data = TokenToInt(name);
                local_rpn.push_back(new RPNType(DataTypesEnum::INT, 0));
            }
            Push(new RPNConst(data));
            pref_func = false;

        }
        else if (TokenName() == "(") {

            Stack.push(new RPNOperation(OperationEnum::BRACE));
            balance++;
            pref_func = false;

        }
        else if (TokenName() == ")") {

            balance--;
            while (Stack.top()->Type() != OperationEnum::BRACE) {
                Push(Stack.top());
                local_rpn.push_back(Stack.top());
                Stack.pop();
            }
            Stack.pop();
            pref_func = false;

        }
        else {

            std::cout << "WTF?";

        }
    }

    while (!Stack.empty()) {
        Push(Stack.top());
        local_rpn.push_back(Stack.top());
        Stack.pop();
    }

    return CalcType(local_rpn);
}
/*
void RPNCreator::Expression() {
  int balance = 0;
  std::vector <StackNode> Stack;
  StackNode token(EMPTY);
  bool prefix = true;
  for (; !(TokenName() == ";" || TokenName() == ", " || TokenName() == ")" && balance == 0); TokenNext()) {
    //является ли символ операндом
    if (!OperationPriority.count(TokenName()) && prefix) {
      token.data_ = TokenName();
      prefix = false;
      TokenPrev();
      continue;
    }
    else if (token.data_ != EMPTY) {
      if (TokenType() == TType::NAME) {
        TokenNext();
        std::string tk = TokenName();
        TokenPrev();
        if (tk != "(") {
          PushBackRPN(new RPNVariable(token.data_));
          token.data_ = EMPTY;
          continue;
        }
        else {
          FunctionCall();
          token.data_ = EMPTY;
          continue;
        }
      }
      else {
        std::string str = TokenName();
        if (TokenName()[0] == '\"') {
          str = str.substr(1,  str.size() - 2);
          // string
          PushBackRPN(new RPNConst(new RPNDataString(str)));
        }
        else if (TokenName()[0] == '\'') {
          str = str.substr(1,  str.size() - 2);
          // char
          PushBackRPN(new RPNConst(new RPNDataChar(StringToChar(TokenName()))));
        }
        else if (TokenName()[0] == 't' || TokenName()[0] == 'f') {
          // bool
          if (TokenName() == "true") {
            PushBackRPN(new RPNConst(new RPNDataBool(true)));
          }
          else {
            PushBackRPN(new RPNConst(new RPNDataBool(false)));
          }
        }
        else if (std::find(str.begin(),  str.end(),  '.') != str.end()) {
          // float
          PushBackRPN(new RPNConst(new RPNDataFloat(std::stod(str))));
        }
        else {
          //int
          PushBackRPN(new RPNConst(new RPNDataInt(std::stoi(str))));
        }
        token.data_ = EMPTY;
        continue;
      }

      token.data_ = TokenName();
      prefix = false;
    }
    else if (prefix && TokenName() == "-") {
      token.data_ = "_";
    }
    else if (prefix && TokenName() == "+") {
      token.data_ = "@";
    }
    else {
      token.data_ = TokenName();
      prefix = false;
    }

    if (prefix) {
      Stack.push_back(token);
    }
    else if (token.data_ == "(") {
      balance++;
      Stack.push_back(token);
      prefix = true;
    }
    else if (token.data_ == ")") {
      balance--;
      while (Stack.back().data_ != "(") {
        PushBackRPN(new RPNOperation(OperationToWord[Stack.back().data_]));
        Stack.pop_back();
      }
      Stack.pop_back();
      prefix = false;
    }
    else {
      int priority = OperationPriority[token.data_];
      while (!Stack.empty() && OperationPriority[Stack.back().data_] != 1
        && (OperationPriority[Stack.back().data_] == 2 || OperationPriority[Stack.back().data_] == 4
          || OperationPriority[Stack.back().data_] < priority
          || (OperationPriority[Stack.back().data_] == priority && priority != 3 && priority != 15)))
      {
        PushBackRPN(new RPNOperation(OperationToWord[Stack.back().data_]));
        Stack.pop_back();
      }
      Stack.push_back(token);
      prefix = true;
    }
    token.data_ = EMPTY;
  }
  while (!Stack.empty()) {
    if (Stack.back().data_ == "(") break;
    PushBackRPN(new RPNOperation(OperationToWord[Stack.back().data_]));
    Stack.pop_back();
  }
}*/

void RPNCreator::SetPriors() {
    oper_prior_.resize(int(OperationEnum::ASSIGN_BIT_XOR) + 1);
    oper_prior_[int(OperationEnum::INCREMENT)] = 2;
    oper_prior_[int(OperationEnum::DECREMENT)] = 2;
    oper_prior_[int(OperationEnum::POW)] = 3;
    oper_prior_[int(OperationEnum::NOT)] = 4;
    oper_prior_[int(OperationEnum::BIT_XOR)] = 4;
    oper_prior_[int(OperationEnum::UNARY_MINUS)] = 4;
    oper_prior_[int(OperationEnum::UNARY_PLUS)] = 4;
    oper_prior_[int(OperationEnum::MULT)] = 5;
    oper_prior_[int(OperationEnum::DIV)] = 5;
    oper_prior_[int(OperationEnum::MOD)] = 5;
    oper_prior_[int(OperationEnum::PLUS)] = 6;
    oper_prior_[int(OperationEnum::MINUS)] = 6;
    oper_prior_[int(OperationEnum::BIT_LEFT)] = 7;
    oper_prior_[int(OperationEnum::BIT_RIGHT)] = 7;
    oper_prior_[int(OperationEnum::LESS)] = 8;
    oper_prior_[int(OperationEnum::MORE)] = 8;
    oper_prior_[int(OperationEnum::EQLS_LESS)] = 8;
    oper_prior_[int(OperationEnum::EQLS_MORE)] = 8;
    oper_prior_[int(OperationEnum::EQLS)] = 9;
    oper_prior_[int(OperationEnum::NOT_EQLS)] = 9;
    oper_prior_[int(OperationEnum::BIT_AND)] = 10;
    oper_prior_[int(OperationEnum::BIT_XOR)] = 11;
    oper_prior_[int(OperationEnum::BIT_OR)] = 12;
    oper_prior_[int(OperationEnum::AND)] = 13;
    oper_prior_[int(OperationEnum::OR)] = 14;
    oper_prior_[int(OperationEnum::ASSIGN)] = 15;
    oper_prior_[int(OperationEnum::ASSIGN_PLUS)] = 15;
    oper_prior_[int(OperationEnum::ASSIGN_MINUS)] = 15;
    oper_prior_[int(OperationEnum::ASSIGN_MULT)] = 15;
    oper_prior_[int(OperationEnum::ASSIGN_DIV)] = 15;
    oper_prior_[int(OperationEnum::ASSIGN_MOD)] = 15;
    oper_prior_[int(OperationEnum::ASSIGN_POW)] = 15;
    oper_prior_[int(OperationEnum::ASSIGN_BIT_LEFT)] = 15;
    oper_prior_[int(OperationEnum::ASSIGN_BIT_RIGHT)] = 15;
    oper_prior_[int(OperationEnum::ASSIGN_BIT_OR)] = 15;
    oper_prior_[int(OperationEnum::ASSIGN_BIT_AND)] = 15;
    oper_prior_[int(OperationEnum::ASSIGN_BIT_XOR)] = 15;
    oper_prior_[int(OperationEnum::BRACE)] = 16;
}

bool RPNCreator::IsUnary(OperationEnum type) {
    if (type == OperationEnum::UNARY_MINUS) return true;
    if (type == OperationEnum::UNARY_PLUS) return true;
    if (type == OperationEnum::NOT) return true;
    if (type == OperationEnum::BIT_NOT) return true;
    if (type == OperationEnum::INCREMENT) return true;
    if (type == OperationEnum::DECREMENT) return true;
    return false;
}

bool RPNCreator::IsRightAssoc(OperationEnum type) {
    if (type == OperationEnum::UNARY_MINUS) return true;
    if (type == OperationEnum::UNARY_PLUS) return true;
    if (type == OperationEnum::NOT) return true;
    if (type == OperationEnum::BIT_NOT) return true;
    if (type == OperationEnum::INCREMENT) return true;
    if (type == OperationEnum::DECREMENT) return true;
    if (type == OperationEnum::POW) return true;
    return false;
}

Data* RPNCreator::TokenToString(std::string token) {
    token = token.substr(1, token.size() - 2);
    for (int i = 0; i < token.size() - 1; ++i) {
        if (token[i] == '\\') {
            if (token[i + 1] == 'a') {
                token.replace(token.begin() + i, token.begin() + i + 2, "\a");
            }
            else if (token[i + 1] == 'b') {
                token.replace(token.begin() + i, token.begin() + i + 2, "\b");
            }
            else if (token[i + 1] == 'f') {
                token.replace(token.begin() + i, token.begin() + i + 2, "\f");
            }
            else if (token[i + 1] == 'n') {
                token.replace(token.begin() + i, token.begin() + i + 2, "\n");
            }
            else if (token[i + 1] == 'r') {
                token.replace(token.begin() + i, token.begin() + i + 2, "\r");
            }
            else if (token[i + 1] == 't') {
                token.replace(token.begin() + i, token.begin() + i + 2, "\t");
            }
            else if (token[i + 1] == 'v') {
                token.replace(token.begin() + i, token.begin() + i + 2, "\v");
            }
            else if (token[i + 1] == '\'') {
                token.replace(token.begin() + i, token.begin() + i + 2, "\'");
            }
            else if (token[i + 1] == '\"') {
                token.replace(token.begin() + i, token.begin() + i + 2, "\"");
            }
            else if (token[i + 1] == '\\') {
                token.replace(token.begin() + i, token.begin() + i + 2, "\\");
            }
            else if (token[i + 1] == '?') {
                token.replace(token.begin() + i, token.begin() + i + 2, "\?");
            }
        }
    }
    return new DataString(token);
}

Data* RPNCreator::TokenToChar(std::string token) {
    token = token.substr(1, token.size() - 2);
    char c = token[0];
    if (token.size() != 1) {
        if (token == "\\a")
            c = '\a';
        if (token == "\\b")
            c = '\b';
        if (token == "\\f")
            c = '\f';
        if (token == "\\n")
            c = '\n';
        if (token == "\\r")
            c = '\r';
        if (token == "\\t")
            c = '\t';
        if (token == "\\v")
            c = '\v';
        if (token == "\\\'")
            c = '\'';
        if (token == "\\\"")
            c = '\"';
        if (token == "\\\\")
            c = '\\';
        if (token == "\\?")
            c = '\?';
    }
    return new DataChar(c);
}

Data* RPNCreator::TokenToBool(std::string token) {
    return new DataBool(token == "true");
}

Data* RPNCreator::TokenToFloat(std::string token) {
    return new DataFloat(std::stod(token));
}

Data* RPNCreator::TokenToInt(std::string token) {
    return new DataInt(std::stoi(token));
}

RPNType RPNCreator::CalcType(std::vector<RPNToken*>& local_rpn) {
    // possible memory leaks. TO-DO
    std::stack<RPNType*> st;
    for (auto& token : local_rpn) {
        if (token->Type() == RPNTokenTypeEnum::TYPE) {
            st.push(static_cast<RPNType*>(token));
        }
        else {
            RPNType* type = NULL;
            RPNOperation* oper = static_cast<RPNOperation*>(token);
            if (!IsUnary(oper->Type())) {
                RPNType* second = st.top();
                st.pop();
                RPNType* first = st.top();
                st.pop();
                type = MakeOperation(first, second, oper);
                delete first;
                delete second;
            }
            else {
                RPNType* first = st.top();
                st.pop();
                type = new RPNType(*first);
                delete first;
            }
            st.push(type);
        }
    }
    return *st.top();
}

RPNType* RPNCreator::MakeOperation(RPNType* first, RPNType* second, RPNOperation* oper) {
    switch (oper->Type())
    {
    case OperationEnum::BRACE:
        throw "BRACE";
        break;
    case OperationEnum::EQLS:
        return new RPNType(DataTypesEnum::BOOL, 0);
        break;
    case OperationEnum::PLUS:
        return new RPNType(std::max(first->Type(), second->Type()), 0);
        break;
    case OperationEnum::MINUS:
        return new RPNType(std::max(first->Type(), second->Type()), 0);
        break;
    case OperationEnum::MULT:
        return new RPNType(std::max(first->Type(), second->Type()), 0);
        break;
    case OperationEnum::DIV:
        return new RPNType(std::max(first->Type(), second->Type()), 0);
        break;
    case OperationEnum::MOD:
        return new RPNType(std::max(first->Type(), second->Type()), 0);
        break;
    case OperationEnum::BIT_LEFT:
        return new RPNType(std::max(first->Type(), second->Type()), 0);
        break;
    case OperationEnum::BIT_RIGHT:
        return new RPNType(std::max(first->Type(), second->Type()), 0);
        break;
    case OperationEnum::POW:
        return new RPNType(std::max(first->Type(), second->Type()), 0);
        break;
    case OperationEnum::OR:
        return new RPNType(DataTypesEnum::BOOL, 0);
        break;
    case OperationEnum::AND:
        return new RPNType(DataTypesEnum::BOOL, 0);
        break;
    case OperationEnum::BIT_OR:
        return new RPNType(std::max(first->Type(), second->Type()), 0);
        break;
    case OperationEnum::BIT_AND:
        return new RPNType(std::max(first->Type(), second->Type()), 0);
        break;
    case OperationEnum::BIT_XOR:
        return new RPNType(std::max(first->Type(), second->Type()), 0);
        break;
    case OperationEnum::NOT_EQLS:
        return new RPNType(DataTypesEnum::BOOL, 0);
        break;
    case OperationEnum::LESS:
        return new RPNType(DataTypesEnum::BOOL, 0);
        break;
    case OperationEnum::MORE:
        return new RPNType(DataTypesEnum::BOOL, 0);
        break;
    case OperationEnum::EQLS_LESS:
        return new RPNType(DataTypesEnum::BOOL, 0);
        break;
    case OperationEnum::EQLS_MORE:
        return new RPNType(DataTypesEnum::BOOL, 0);
        break;
    case OperationEnum::ASSIGN:
        return new RPNType(*first);
        break;
    case OperationEnum::ASSIGN_PLUS:
        return new RPNType(*first);
        break;
    case OperationEnum::ASSIGN_MINUS:
        return new RPNType(*first);
        break;
    case OperationEnum::ASSIGN_MULT:
        return new RPNType(*first);
        break;
    case OperationEnum::ASSIGN_DIV:
        return new RPNType(*first);
        break;
    case OperationEnum::ASSIGN_MOD:
        return new RPNType(*first);
        break;
    case OperationEnum::ASSIGN_POW:
        return new RPNType(*first);
        break;
    case OperationEnum::ASSIGN_BIT_LEFT:
        return new RPNType(*first);
        break;
    case OperationEnum::ASSIGN_BIT_RIGHT:
        return new RPNType(*first);
        break;
    case OperationEnum::ASSIGN_BIT_OR:
        return new RPNType(*first);
        break;
    case OperationEnum::ASSIGN_BIT_AND:
        return new RPNType(*first);
        break;
    case OperationEnum::ASSIGN_BIT_XOR:
        return new RPNType(*first);
        break;
    default:
        break;
    }
}

