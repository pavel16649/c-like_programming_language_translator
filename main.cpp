#include "SyntaxAnalyzer.h"
#include "LexicalAnalyzer.h"
#include "RPNCreator.h"
#include "Executor.h"

int main() {
    LexicalAnalyzer analyzer("program.txt",  "statemachine.txt",  "reserved.txt");
    std::vector<Token> tokens = analyzer.work();
    std::ofstream lout("lecsical.txt");
    for (auto& i : tokens) {
        lout << i.lecsema() << ' ' << i.type() << ' ' << i.string_num() << ' '
            << i.char_num() << std::endl;
    }

    SyntaxAnalyzer SA;
    bool ok = SA.checkTokens(tokens);
    if (!ok) throw 123;
    
    RPNCreator rpn_creator(tokens);
    rpn_creator.Show();
    Executor exec(rpn_creator.GetRPN(), rpn_creator.GetMain());
    exec.Work();
    return 0;
}
