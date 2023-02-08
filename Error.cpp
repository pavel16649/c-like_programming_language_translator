#include "Error.h"

void Error::display()
{

    if (_displayed) return;
    _displayed = true;

    if (is_semant_) {

      std::cout << _expected << '\n';
      return;
    }

    if (_expected == "identifier" || _expected == "value"
        || _expected == "type name" || _expected == "constant"
        || _expected == "operator" || _expected == "declaration"
        || _expected == "default value")
    {
        std::cout << "Error in " << _str_num << ":" << _ch_num << std::endl
            << "Expected " << _expected << ",  got \"" << _got << "\"" << std::endl;

        return;
    }

    std::cout << "Error in " << _str_num << ":" << _ch_num << std::endl
        << "Expected \"" << _expected << "\",  got \"" << _got << "\"" << std::endl;
}
