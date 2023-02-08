#include "SyntaxAnalyzer.h"

bool SyntaxAnalyzer::checkTokens(std::vector <Token>& tokens)
{
  _tokens = tokens;
  ptr_tokens_ = &tokens;
  program();
  return true;
}

void SyntaxAnalyzer::next_token()
{
  ++_current_token_index;
}

void SyntaxAnalyzer::prev_token()
{
  --_current_token_index;
}

int SyntaxAnalyzer::token_type()
{
  return _tokens[_current_token_index].type();
}

std::string SyntaxAnalyzer::token_name()
{
  return _tokens[_current_token_index].lecsema();
}

inline int SyntaxAnalyzer::string_num()
{
  return _tokens[_current_token_index].string_num();
}

inline int SyntaxAnalyzer::char_num()
{
  return _tokens[_current_token_index].char_num();
}

void SyntaxAnalyzer::program()
{
  try {
    while (true /*_current_token_index < _tokens.size()*/) {
      if (token_name() == "dynamic" || token_name() == "array") {
        array_declaration();

        if (token_name() != ";") throw Error(_tokens[_current_token_index].string_num(), 
          _tokens[_current_token_index].char_num(), 
          ";",  token_name());
        next_token();

      }
      else if (token_name() == "int" || token_name() == "float"
        || token_name() == "bool" || token_name() == "string"
        || token_name() == "char" || token_name() == "void")
      {
        next_token();
        if (token_name() == "main") {
          prev_token();
          _main();
          break;

        }
        else if (token_type() == name) {
          next_token();

          if (token_name() == "(") {
            prev_token(); prev_token();
            func_declaration();
          }
          else {
            prev_token(); prev_token();
            variable_declaration();

            if (token_name() != ";") throw Error(_tokens[_current_token_index].string_num(), 
              _tokens[_current_token_index].char_num(), 
              ";",  token_name());;
            next_token();
          }
        }
        else {
          throw Error(_tokens[_current_token_index].string_num(), 
            _tokens[_current_token_index].char_num(), 
            "identifier",  token_name());
        }
      }
      else {
        throw Error(_tokens[_current_token_index].string_num(), 
          _tokens[_current_token_index].char_num(), 
          "declaration",  token_name());
      }
    }

  }
  catch (Error err) {
    err.display();
  }
}
void SyntaxAnalyzer::non_func_declaration()
{
  try {
    if (token_name() == "dynamic" || token_name() == "array") {
      array_declaration();
    }
    else if (token_name() == "int" || token_name() == "float"
      || token_name() == "bool" || token_name() == "string"
      || token_name() == "char")
    {
      variable_declaration();
    }
    else {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "type name",  token_name());
    }
  }
  catch (Error err) {
    err.display();
  }
}

void SyntaxAnalyzer::func_declaration()
{
  try {

    sema_.PushTid();
    if (token_name() == "void") {
      sema_.cur_type = "void";
      next_token();
    }
    else {
      type();
    }
    std::string func_type = sema_.cur_type;
    sema_.cur_args.clear();

    int default_cnt = 0;
    if (token_type() != name) {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "identifier",  token_name());
    }
    sema_.cur_func = token_name();
    next_token();

    if (token_name() != "(") {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "(",  token_name());
    }
    next_token();
    if (token_name() != ")") {
      formal_parameter();
      bool flag = 0;
      if (token_name() == "=") {
        flag = 1;
        default_cnt++;
        next_token();
        const_expr();
        std::string par_type = sema_.GetNode().data_;
        if (!sema_.CheckType(par_type,  *(sema_.cur_args.end() - 1))) {
          throw Error(_tokens[_current_token_index].string_num(), 
            _tokens[_current_token_index].char_num(), 
            "Wrong type of parametr. Got: " + par_type + ",  expected: " + *(sema_.cur_args.end() - 1),  "",  1);
        }
      }

      while (token_name() == ",") {
        next_token();
        formal_parameter();
        if (token_name() == "=") {
          flag = 1;
          default_cnt++;
          next_token();
          const_expr();
          std::string par_type = sema_.GetNode().data_;
          if (!sema_.CheckType(par_type,  *(sema_.cur_args.end() - 1))) {
            throw Error(_tokens[_current_token_index].string_num(), 
              _tokens[_current_token_index].char_num(), 
              "Wrong type of parametr. Got: " + par_type + ",  expected: " + *(sema_.cur_args.end() - 1),  "",  1);
          }
        }
        else if (flag && token_name() == ")") {
          throw Error(_tokens[_current_token_index].string_num(), 
            _tokens[_current_token_index].char_num(), 
            "default value",  token_name());
        }
      }
    }

    if (token_name() != ")") {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        ")",  token_name());
    }
    next_token();
    for (int i = 0; i <= default_cnt; ++i) {
      std::vector<std::string> args;
      for (int j = 0; j < sema_.cur_args.size() - i; ++j) {
        args.push_back(sema_.cur_args[j]);
      }
      if (sema_.FuncExist(sema_.cur_func,  args)) {
        throw Error(_tokens[_current_token_index].string_num(), 
          _tokens[_current_token_index].char_num(), 
          "Function redeclaration",  "",  1);
      }
      sema_.PushFunc(func_type,  sema_.cur_func,  args);
    }
    compound_operator();
    sema_.LeaveTid();

  }
  catch (Error err) {
    err.display();
  }
}

void SyntaxAnalyzer::array_declaration()
{
  try {
    if (token_name() == "dynamic" || token_name() == "array") {
      sema_.cur_type = "";
      array_template();
    }
    else {
      throw 123;
    }

    if (token_type() != name) {
      throw  Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "identifier",  token_name());
    }

    if (!sema_.CheckDeclrVar(token_name())) {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(),  "Variable redeclaration: " + token_name(),  "",  1);
    }
    sema_.PushVar(token_name(),  sema_.cur_type);
    next_token();

    while (token_name() == "[") {
      next_token();

      if (token_type() == name) {
        variable();
      }
      else {
        expression();
      }

      std::string type_indx = sema_.GetNode().data_;
      if (type_indx != "int") {
        throw Error(_tokens[_current_token_index].string_num(), 
          _tokens[_current_token_index].char_num(), 
          "Wrong type of index",  "",  1);
      }

      if (token_name() != "]") throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "]",  token_name());
      next_token();
    }

    while (token_name() == "[") {
      next_token();

      if (token_type() == constant && std::isdigit(token_name()[0])) {
        next_token();
      }
      else if (token_type() == name) {
        variable();
      }
      else {
        throw  Error(_tokens[_current_token_index].string_num(), 
          _tokens[_current_token_index].char_num(), 
          "value",  token_name());;
      }

      if (token_name() != "]") throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "]",  token_name());
      next_token();
    }

  }
  catch (Error err) {
    err.display();
  }
}

void SyntaxAnalyzer::array_template()
{
  try {
    if (token_name() == "dynamic") {
      sema_.cur_type += token_name();
      next_token();
    }

    if (token_name() != "array") throw  Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      "array",  token_name());;

    sema_.cur_type += " " + token_name();
    next_token();

    if (token_name() != "<") throw  Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      "<",  token_name());

    sema_.cur_type += token_name();
    next_token();

    if (token_name() == "dynamic" || token_name() == "array") {
      array_template();
    }
    else if (token_name() == "int" || token_name() == "float"
      || token_name() == "bool" || token_name() == "string"
      || token_name() == "char")
    {

      sema_.cur_type += token_name();
      next_token();
    }
    else {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "type name",  token_name());
    }

    if (token_name() != ">") throw Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      ">",  token_name());

    sema_.cur_type += token_name();
    next_token();

  }
  catch (Error err) {
    err.display();
  }
}

void SyntaxAnalyzer::formal_parameter()
{
  type();

  sema_.cur_args.push_back(sema_.cur_type);

  try {
    if (token_type() == name) {
      sema_.PushVar(token_name(),  sema_.cur_type);
      next_token();
    }
    else {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "identifier",  token_name());
    }
  }
  catch (Error err) {
    err.display();
  }
}

void SyntaxAnalyzer::_main()
{
  try {
    std::string func_type,  func_name;

    if (token_name() != "int") throw Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      "int",  token_name());
    next_token();

    func_type = "int";

    if (token_name() != "main") throw Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      "main",  token_name());
    next_token();

    func_name = "main";

    if (token_name() != "(") throw Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      "(",  token_name());
    next_token();

    if (token_name() != ")") throw Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      ")",  token_name());
    next_token();

    sema_.PushFunc(func_type,  func_name,  {});

    sema_.PushTid();
    compound_operator();
    sema_.LeaveTid();

  }
  catch (Error err) {
    err.display();
  }
}

void SyntaxAnalyzer::variable_declaration()
{
  type();
  section();
  while (token_name() == ", ") {
    next_token();
    section();
  }
}

void SyntaxAnalyzer::variable()
{
  try {
    if (token_type() != name) {
      throw  Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "identifier",  token_name());
    }
    if (!sema_.CheckVar(token_name())) {
      throw  Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "Udeclared variable: " + token_name(),  "",  1);
    }
    std::string type = sema_.VarGetType(token_name());
    next_token();

    while (token_name() == "[") {
      next_token();

      while (!type.empty() && type[0] != '<') {
        type.erase(type.begin());
      }
      if (type.empty()) {
        throw Error(_tokens[_current_token_index].string_num(), 
          _tokens[_current_token_index].char_num(), 
          "Variable can not be indexed: " + token_name(),  "",  1);
      }
      type.erase(type.begin());
      type.erase(type.end() - 1);

      if (token_type() == name) {
        variable();
      }
      else {
        expression();
      }

      std::string type_indx = sema_.GetNode().data_;
      if (type_indx != "int") {
        throw Error(_tokens[_current_token_index].string_num(), 
          _tokens[_current_token_index].char_num(), 
          "wrong type of index",  token_name());
      }

      if (token_name() != "]") throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "]",  token_name());
      next_token();
    }

    sema_.SetNode(type,  1);

  }
  catch (Error err) {
    err.display();
  }
}

void SyntaxAnalyzer::type()
{
  try {
    if (token_name() == "int" || token_name() == "float"
      || token_name() == "bool" || token_name() == "string"
      || token_name() == "char")
    {
      sema_.cur_type = token_name();
      next_token();
    }
    else {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "type name",  token_name());
    }
  }
  catch (Error err) {
    err.display();
  }
}

void SyntaxAnalyzer::section()
{
  try {
    if (token_type() == name) {
      if (!sema_.CheckDeclrVar(token_name())) {
        throw Error(_tokens[_current_token_index].string_num(), 
          _tokens[_current_token_index].char_num(),  "Variable redeclaration: " + token_name(),  "",  1);
      }
      sema_.PushVar(token_name(),  sema_.cur_type);
      next_token();
      if (token_name() == "=") {
        next_token();
        expression();
        std::string type = sema_.GetNode().data_;
        if (sema_.cur_type == "int" || sema_.cur_type == "float" || sema_.cur_type == "bool" || sema_.cur_type == "char") {
          if (type == "string") {
            throw Error(_tokens[_current_token_index].string_num(), 
              _tokens[_current_token_index].char_num(),  "Wrong type of expression: " + type,  "",  1);
          }
        }
        if (sema_.cur_type == "string") {
          if (type != "char" && type != "string") {
            throw Error(_tokens[_current_token_index].string_num(), 
              _tokens[_current_token_index].char_num(),  "Wrong type of expression: " + type,  "",  1);
          }
        }
      }
    }
    else {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "identifier",  token_name());
    }
  }
  catch (Error err) {
    err.display();
  }
}

void SyntaxAnalyzer::expression()
{
  priority15();
}

void SyntaxAnalyzer::const_expr()
{
  try {
    /*if (token_name() == "+" || token_name() == "-") {
      next_token();
      const_expr();

    }
    else if (token_name() == "!") {
      next_token();
      const_expr();

    }
    else */if (token_type() == constant || token_name() == "true" || token_name() == "false") {
      std::string name = token_name();
      if (name == "true" || name == "false") {
        sema_.SetNode("bool",  1);
      }
      else if (name[0] == '\'') {
        sema_.SetNode("char",  1);
      }
      else if (name[0] == '\"') {
        sema_.SetNode("string",  1);
      }
      else if (name.find('.') != std::string::npos) {
        sema_.SetNode("float",  1);
      }
      else {
        sema_.SetNode("int",  1);
      }
      next_token();
    }
    else {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "constant",  token_name());
    }
  }
  catch (Error err) {
    err.display();
  }
}

void SyntaxAnalyzer::compound_operator()
{
  try {
    // sema_.NewTid();
    if (token_name() != "{") throw Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      "{",  token_name());
    next_token();

    while (token_name() != "}") {
      if (token_name() == "if") {
        _if();
      }
      else if (token_name() == "while") {
        _while();
      }
      else if (token_name() == "do") {
        do_while();
      }
      else if (token_name() == "for") {
        _for();
      }
      else if (token_name() == "switch") {
        _switch();
      }
      else if (token_name() == "int" || token_name() == "float"
        || token_name() == "bool" || token_name() == "string"
        || token_name() == "char" || token_name() == "array"
        || token_name() == "dynamic")
      {
        non_func_declaration();
        if (token_name() != ";") throw Error(_tokens[_current_token_index].string_num(), 
          _tokens[_current_token_index].char_num(), 
          ";",  token_name());
        next_token();
      }
      else {
        _operator();
        if (token_name() != ";") throw Error(_tokens[_current_token_index].string_num(), 
          _tokens[_current_token_index].char_num(), 
          ";",  token_name());
        next_token();
      }
    }
    next_token();
    // sema_.LeaveFunc();
  }
  catch (Error err) {
    err.display();
  }
}

void SyntaxAnalyzer::_operator()
{
  if (token_name() == "break" || token_name() == "continue") {
    next_token();
  }
  else if (token_name() == "return") {
    next_token();
    if (token_name() != ";") {
      expression();
    }
    std::string ret_type = sema_.GetNode().data_;
    std::string func_type = sema_.GetFunc(sema_.CurTID()->FuncId()).type_;
    if (ret_type != func_type) {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "Wrong returning type: " + ret_type + ". Expected: " + func_type,  "",  1);
    }
  }
  else if (token_name() == "print") {
    print();
  }
  else {
    expression();
  }
}

void SyntaxAnalyzer::print() {
  next_token();
  if (token_name() != "(") throw Error(_tokens[_current_token_index].string_num(), 
    _tokens[_current_token_index].char_num(), 
    "(",  token_name());
  next_token();
  while (token_name() != ")") {
    expression();

    std::string exp_type = sema_.GetNode().data_;

    if (exp_type == "void") {
      Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(),  "non void expression",  "void expression");
    }

    if (token_name() != ", " && token_name() != ")") Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      ",  or )",  token_name());
    if (token_name() == ", ") next_token();
  }
  next_token();
}

void SyntaxAnalyzer::_if()
{
  try {
    if (token_name() != "if") throw Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      "if",  token_name());
    next_token();

    if (token_name() != "(") throw Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      "(",  token_name());
    next_token();

    expression();

    std::string exp_type = sema_.GetNode().data_;

    if (exp_type != "bool" && exp_type != "int" && exp_type != "char" && exp_type != "float") {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "Not bool expression.",  "",  1);
    }

    if (token_name() != ")") throw Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      ")",  token_name());
    next_token();

    if (token_name() == "{") {
      sema_.PushTid();
      compound_operator();
      sema_.LeaveTid();
    }
    else {
      _operator();
      if (token_name() != ";") throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        ";",  token_name());
      next_token();
    }

    if (token_name() == "else") {
      next_token();
      if (token_name() == "if") {
        _if();
      }
      else if (token_name() == "{") {
        sema_.PushTid();
        compound_operator();
        sema_.LeaveTid();
      }
      else {
        _operator();
        if (token_name() != ";") throw Error(_tokens[_current_token_index].string_num(), 
          _tokens[_current_token_index].char_num(), 
          ";",  token_name());;
        next_token();
      }
    }
  }
  catch (Error err) {
    err.display();
  }
}

void SyntaxAnalyzer::_while()
{
  try {
    if (token_name() != "while") throw Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      "while",  token_name());
    next_token();

    if (token_name() != "(") throw Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      "(",  token_name());
    next_token();

    expression();

    std::string exp_type = sema_.GetNode().data_;

    if (exp_type != "bool" && exp_type != "int" && exp_type != "char" && exp_type != "float") {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "Not bool expression.",  "",  1);
    }

    if (token_name() != ")") throw Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      ")",  token_name());
    next_token();

    if (token_name() == "{") {
      sema_.PushTid();
      compound_operator();
      sema_.LeaveTid();
    }
    else {
      _operator();
      if (token_name() != ";") throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        ";",  token_name());
      next_token();
    }

  }
  catch (Error err) {
    err.display();
  }
}

void SyntaxAnalyzer::_for()
{
  try {
    if (token_name() != "for") throw Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      "for",  token_name());
    next_token();

    if (token_name() != "(") throw Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      "(",  token_name());
    next_token();

    // expression 1

    if (token_name() == "dynamic" || token_name() == "array") {
      array_declaration();
    }
    else if (token_name() == "int" || token_name() == "float"
      || token_name() == "bool" || token_name() == "string"
      || token_name() == "char")
    {
      variable_declaration();
    }
    else if (token_name() == ";") {
      ;
    }
    else {
      expression();
    }

    if (token_name() != ";") {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        ";",  token_name());
    }
    next_token();

    // expression 2

    if (token_name() != ";") {
      expression();

      std::string exp_type = sema_.GetNode().data_;

      if (exp_type != "bool" && exp_type != "int" && exp_type != "char" && exp_type != "float") {
        throw Error(_tokens[_current_token_index].string_num(), 
          _tokens[_current_token_index].char_num(), 
          "Not bool expression.",  "",  1);
      }
    }

    if (token_name() != ";") {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        ";",  token_name());
    }
    next_token();

    // expression 3

    if (token_name() != ")") {
      expression();
    }

    if (token_name() != ")") {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        ")",  token_name());
    }
    next_token();

    if (token_name() == "{") {
      sema_.PushTid();
      compound_operator();
      sema_.LeaveTid();
    }
    else {
      _operator();
      if (token_name() != ";") {
        throw Error(_tokens[_current_token_index].string_num(), 
          _tokens[_current_token_index].char_num(), 
          ";",  token_name());
      }
      next_token();
    }

  }
  catch (Error err) {
    err.display();
  }
}

void SyntaxAnalyzer::do_while()
{
  try {
    if (token_name() != "do") throw Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      "do",  token_name());
    next_token();

    if (token_name() == "{") {
      sema_.PushTid();
      compound_operator();
      sema_.LeaveTid();
    }
    else {
      _operator();
      if (token_name() != ";") throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        ";",  token_name());
      next_token();
    }

    if (token_name() != "while") throw Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      "while",  token_name());
    next_token();

    if (token_name() != "(") throw Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      "(",  token_name());
    next_token();

    expression();

    std::string exp_type = sema_.GetNode().data_;

    if (exp_type != "bool" && exp_type != "int" && exp_type != "char" && exp_type != "float") {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "Not bool expression.",  "",  1);
    }

    if (token_name() != ")") throw Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      ")",  token_name());
    next_token();

    if (token_name() != ";") throw Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      ";",  token_name());
    next_token();

  }
  catch (Error err) {
    err.display();
  }
}

void SyntaxAnalyzer::func_call()
{
  try {

    std::string func_name;

    if (token_type() != name) throw Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      "identifier",  token_name());
    func_name = token_name();
    int ind = _current_token_index;
    next_token();

    sema_.cur_args.clear();

    if (token_name() != "(") throw Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      "(",  token_name());
    next_token();

    if (token_name() != ")") {
      non_formal_parameter();
      while (token_name() == ", ") {
        next_token();
        non_formal_parameter();
      }

      if (token_name() != ")") throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        ")",  token_name());
    }
    next_token();

    if (!sema_.FuncExist(func_name,  sema_.cur_args)) {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "Undeclared function",  "",  1);
    }

    std::vector<std::string> *func_id = new std::vector<std::string>; *func_id = sema_.cur_args;
    func_id->insert(func_id->begin(),  func_name);

    sema_.SetNode(sema_.GetFunc(FunctionFirst(func_name,  sema_.cur_args)).type_,  1);
  }
  catch (Error err) {
    err.display();
  }
}

void SyntaxAnalyzer::derived_operator()
{
  try {
    if (token_name() == "if") {
      _if();
    }
    else if (token_name() == "while") {
      _while();
    }
    else if (token_name() == "do") {
      do_while();
    }
    else if (token_name() == "for") {
      _for();
    }
    else if (token_name() == "switch") {
      _switch();
    }
    else {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "operator",  token_name());
    }
  }
  catch (Error err) {
    err.display();
  }
}

void SyntaxAnalyzer::value()
{
  if (token_type() == name) {
    next_token();
    if (token_name() == "(") {
      prev_token();
      func_call();
    }
    else {
      prev_token();
      variable();
    }
  }
  else {
    const_expr();
  }
}

void SyntaxAnalyzer::_switch()
{
  try {
    if (token_name() != "switch") throw Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      "switch",  token_name());
    next_token();

    if (token_name() != "(") throw Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      "(",  token_name());
    next_token();

    variable();

    std::string vtype = sema_.GetNode().data_;

    if (token_name() != ")") throw Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      ")",  token_name());
    next_token();

    if (token_name() != "{") throw Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      "{",  token_name());
    next_token();

    while (token_name() == "case") {
      next_token();
      const_expr();

      std::string ctype = sema_.GetNode().data_;

      if (!sema_.CheckType(vtype,  ctype)) {
        throw Error(_tokens[_current_token_index].string_num(), 
          _tokens[_current_token_index].char_num(), 
          "Wrong type of constant: " + ctype + ". Expected:" + vtype,  "",  1);
      }

      if (token_name() != ":") throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        ":",  token_name());
      next_token();

      if (token_name() == "{") {
        sema_.PushTid();
        compound_operator();
        sema_.LeaveTid();
      }
      else {
        _operator();
      }
      if (token_name() != ";") throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        ";",  token_name());
      next_token();
    }

    if (token_name() == "default") {
      next_token();
      if (token_name() != ":") throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        ":",  token_name());
      next_token();

      if (token_name() == "{") {
        sema_.PushTid();
        compound_operator();
        sema_.LeaveTid();
      }
      else {
        _operator();
        if (token_name() != ";") throw Error(_tokens[_current_token_index].string_num(), 
          _tokens[_current_token_index].char_num(), 
          ";",  token_name());
        next_token();
      }
    }

    if (token_name() != "}") throw Error(_tokens[_current_token_index].string_num(), 
      _tokens[_current_token_index].char_num(), 
      "}",  token_name());
    next_token();

  }
  catch (Error err) {
    err.display();
  }
}

void SyntaxAnalyzer::non_formal_parameter()
{
  next_token();
  if (token_name() == "(") {
    prev_token();
    func_call();
  }
  else {
    prev_token();
    expression();
  }

  sema_.cur_args.push_back(sema_.GetNode().data_);
}

void SyntaxAnalyzer::priority1()
{
  try {
    if (token_name() == "(") {
      next_token();
      if (token_name() == "(") {
        priority1();
      }
      else {
        priority15();
      }
      if (token_name() != ")") {
        throw Error(_tokens[_current_token_index].string_num(), 
          _tokens[_current_token_index].char_num(), 
          ")",  token_name());
      }
      next_token();
    }
    else {
      value();
    }
  }
  catch (Error err) {
    err.display();
  }
}

void SyntaxAnalyzer::priority2()
{
  if (token_name() == "++" || token_name() == "--") {
    std::string oper;
    sema_.SetNode(oper = token_name(),  0);
    next_token();
    priority2();
    std::string type;
    std::string operand1;
    sema_.SetNode(type = sema_.CheckUnoOperType(operand1 = sema_.GetNode().data_,  sema_.GetNode().data_),  1);
    if (type == "Error") {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "Wrong type of operand",  "",  1);
    }
  }
  else {
    priority1();
  }
}

void SyntaxAnalyzer::priority3()
{
  priority2();
  if (token_name() == "**") {
    std::string oper;
    sema_.SetNode(oper = token_name(),  0);
    next_token();
    priority3();
    std::string type = "";
    std::string o1,  o2;
    sema_.SetNode(type = sema_.CheckOperType(o1 = sema_.GetNode().data_,  sema_.GetNode().data_,  o2 = sema_.GetNode().data_),  1);
    if (type == "Error") {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "Wrong type of operand",  "",  1);
    }
  }
}

void SyntaxAnalyzer::priority4()
{
  if (token_name() == "!" || token_name() == "~"
    || token_name() == "-" || token_name() == "+")
  {
    std::string oper;
    sema_.SetNode(oper = token_name(),  0);
    next_token();
    priority4();
    std::string type;
    sema_.SetNode(type = sema_.CheckUnoOperType(sema_.GetNode().data_,  sema_.GetNode().data_),  1);
    if (type == "Error") {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "Wrong type of operand",  "",  1);
    }
  }
  else {
    priority3();
  }
}

void SyntaxAnalyzer::priority5()
{
  priority4();
  if (token_name() == "*" || token_name() == "/" || token_name() == "%") {
    std::string oper;
    sema_.SetNode(oper = token_name(),  0);
    next_token();
    priority5();
    std::string type = "";
    std::string o1,  o2;
    sema_.SetNode(type = sema_.CheckOperType(o1 = sema_.GetNode().data_,  sema_.GetNode().data_,  o2 = sema_.GetNode().data_),  1);
    if (type == "Error") {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "Wrong type of operand",  "",  1);
    }
  }
}

void SyntaxAnalyzer::priority6()
{
  priority5();
  if (token_name() == "+" || token_name() == "-") {
    std::string oper;
    sema_.SetNode(oper = token_name(),  0);
    next_token();
    priority6();
    std::string type = "";
    std::string o1,  o2;
    sema_.SetNode(type = sema_.CheckOperType(o1 = sema_.GetNode().data_,  sema_.GetNode().data_,  o2 = sema_.GetNode().data_),  1);
    if (type == "Error") {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "Wrong type of operand",  "",  1);
    }
  }
}

void SyntaxAnalyzer::priority7()
{
  priority6();
  if (token_name() == "<<" || token_name() == ">>") {
    std::string oper;
    sema_.SetNode(oper = token_name(),  0);
    next_token();
    priority7();
    std::string type = "";
    std::string o1,  o2;
    sema_.SetNode(type = sema_.CheckOperType(o1 = sema_.GetNode().data_,  sema_.GetNode().data_,  o2 = sema_.GetNode().data_),  1);
    if (type == "Error") {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "Wrong type of operand",  "",  1);
    }
  }
}

void SyntaxAnalyzer::priority8()
{
  priority7();
  if (token_name() == ">=" || token_name() == "<="
    || token_name() == ">" || token_name() == "<")
  {
    std::string oper;
    sema_.SetNode(oper = token_name(),  0);
    next_token();
    priority8();
    std::string type = "";
    std::string o1,  o2;
    sema_.SetNode(type = sema_.CheckOperType(o1 = sema_.GetNode().data_,  sema_.GetNode().data_,  o2 = sema_.GetNode().data_),  1);
    if (type == "Error") {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "Wrong type of operand",  "",  1);
    }
  }
}



void SyntaxAnalyzer::priority9()
{
  priority8();
  if (token_name() == "==" || token_name() == "!=") {
    std::string oper;
    sema_.SetNode(oper = token_name(),  0);
    next_token();
    priority9();
    std::string type = "";
    std::string o1,  o2;
    sema_.SetNode(type = sema_.CheckOperType(o1 = sema_.GetNode().data_,  sema_.GetNode().data_,  o2 = sema_.GetNode().data_),  1);
    if (type == "Error") {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "Wrong type of operand",  "",  1);
    }
  }
}

void SyntaxAnalyzer::priority10()
{
  priority9();
  if (token_name() == "&") {
    std::string oper;
    sema_.SetNode(oper = token_name(),  0);
    next_token();
    priority10();
    std::string type = "";
    std::string o1,  o2;
    sema_.SetNode(type = sema_.CheckOperType(o1 = sema_.GetNode().data_,  sema_.GetNode().data_,  o2 = sema_.GetNode().data_),  1);
    if (type == "Error") {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "Wrong type of operand",  "",  1);
    }
  }
}

void SyntaxAnalyzer::priority11()
{
  priority10();
  if (token_name() == "^") {
    std::string oper;
    sema_.SetNode(oper = token_name(),  0);
    next_token();
    priority11();
    std::string type = "";
    std::string o1,  o2;
    sema_.SetNode(type = sema_.CheckOperType(o1 = sema_.GetNode().data_,  sema_.GetNode().data_,  o2 = sema_.GetNode().data_),  1);
    if (type == "Error") {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "Wrong type of operand",  "",  1);
    }
  }
}

void SyntaxAnalyzer::priority12()
{
  priority11();
  if (token_name() == "|") {
    std::string oper;
    sema_.SetNode(oper = token_name(),  0);
    next_token();
    priority12();
    std::string type = "";
    std::string o1,  o2;
    sema_.SetNode(type = sema_.CheckOperType(o1 = sema_.GetNode().data_,  sema_.GetNode().data_,  o2 = sema_.GetNode().data_),  1);
    if (type == "Error") {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "Wrong type of operand",  "",  1);
    }
  }
}

void SyntaxAnalyzer::priority13()
{
  priority12();
  if (token_name() == "&&") {
    std::string oper;
    sema_.SetNode(oper = token_name(),  0);
    next_token();
    priority13();
    std::string type = "";
    std::string o1,  o2;
    sema_.SetNode(type = sema_.CheckOperType(o1 = sema_.GetNode().data_,  sema_.GetNode().data_,  o2 = sema_.GetNode().data_),  1);
    if (type == "Error") {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "Wrong type of operand",  "",  1);
    }
  }
}

void SyntaxAnalyzer::priority14()
{
  priority13();
  if (token_name() == "||") {
    std::string oper;
    sema_.SetNode(oper = token_name(),  0);
    next_token();
    priority14();
    std::string type = "";
    std::string o1,  o2;
    sema_.SetNode(type = sema_.CheckOperType(o1 = sema_.GetNode().data_,  sema_.GetNode().data_,  o2 = sema_.GetNode().data_),  1);
    if (type == "Error") {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "Wrong type of operand",  "",  1);
    }
  }
}

void SyntaxAnalyzer::priority15()
{
  priority14();
  if (token_name() == "=" || token_name() == "+="
    || token_name() == "-=" || token_name() == "*="
    || token_name() == "/=" || token_name() == "**="
    || token_name() == ">>=" || token_name() == "<<="
    || token_name() == "%=" || token_name() == "&="
    || token_name() == "^=" || token_name() == "|=")
  {
    std::string oper;
    sema_.SetNode(oper = token_name(),  0);
    next_token();
    priority15();
    std::string type = "";
    std::string o1,  o2;
    sema_.SetNode(type = sema_.CheckOperType(o1 = sema_.GetNode().data_,  sema_.GetNode().data_,  o2 = sema_.GetNode().data_),  1);
    if (type == "Error") {
      throw Error(_tokens[_current_token_index].string_num(), 
        _tokens[_current_token_index].char_num(), 
        "Wrong type of operand",  "",  1);
    }
  }
}
