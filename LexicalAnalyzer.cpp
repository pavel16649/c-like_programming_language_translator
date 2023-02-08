#include "LexicalAnalyzer.h"

StateMachine* LexicalAnalyzer::create_machine()
{
    int state_num;
    smin >> state_num;
    all_states.resize(state_num);
    for (auto& i : all_states) {
        int x;
        smin >> x;
        i.make_term(x);
    }
    all_states[0].add(32, &all_states[1]);
    all_states[1].make_term(-1);
    all_states[0].add(10, &all_states[1]);
    all_states[1].make_term(-1);
    all_states[0].add(int('\t'), &all_states[1]);
    all_states[1].make_term(-1);
    int from, to;
    std::string chars;
    while (smin >> from) {
        smin >> to >> chars;
        for (auto& ch : chars) {
            if (ch == '`') ch = ' ';
            all_states[from].add(ch, &all_states[to]);
        }
    }
    return &all_states[0];
}

void LexicalAnalyzer::get_reserved()
{
    std::string inp;
    while (rin >> inp) {
        reserved.insert(inp);
    }
}

std::vector<Token> LexicalAnalyzer::work()
{
    int str_cnt = 1;
    std::vector<Token> output;
    get_reserved();
    StateMachine* machine = create_machine();
    ind = 0;
    while (std::getline(pin, line)) {
        line += "\n";
        machine = &all_states[0];
        std::string cur = "";
        for (ind = 0; ind < line.size(); ++ind) {
            int is_term = machine->is_term();
            machine = machine->get(line[ind]);
            if (!machine) {
                if (is_term == -2) {
                    break;
                }
                if (is_term == 0) {
                    throw "Unexpected symbol";
                    return output;
                }
                if (is_term == 2) {
                    if (reserved.find(cur) != reserved.end()) {
                        is_term = 1;
                    }
                }
                if (is_term != -1) {
                    output.push_back(Token(is_term, cur, str_cnt, ind - cur.size() + 1));
                }
                cur = "";
                ind--;
                machine = &all_states[0];
            }
            else {
                cur += line[ind];
            }
        }
        str_cnt++;
    }
    return output;
}
