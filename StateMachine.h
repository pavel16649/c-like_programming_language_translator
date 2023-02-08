#pragma once

#include "Token.h"

class StateMachine {
public:
  StateMachine() : term(0) { states.resize(256,  nullptr); }
  void add(char c,  StateMachine* st) { states[c] = st; }
  void make_term(int t) { term = t; }
  int is_term() { return term; }
  StateMachine* get(int ch) { return states[ch]; }

private:
  std::vector<StateMachine*> states;
  int term;
};
