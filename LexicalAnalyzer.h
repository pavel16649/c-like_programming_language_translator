#pragma once


#include "StateMachine.h"

class LexicalAnalyzer {
public:
  LexicalAnalyzer(std::string programm_file,  std::string state_machine_file, 
    std::string reserved_file)
  {
    rin = std::ifstream(reserved_file);
    pin = std::ifstream(programm_file);
    smin = std::ifstream(state_machine_file);
  }

  StateMachine* create_machine();
  void get_reserved();

  std::vector<Token> work();

protected:
  std::vector<StateMachine> all_states;
  std::set<std::string> reserved;
  int ind = 0;
  std::string line;
  std::ifstream smin,  pin,  rin;
};

