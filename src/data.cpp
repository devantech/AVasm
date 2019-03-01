#include "data.hpp"
#include <iostream>

namespace data
{

AsmData data;
AssemblerState state;
SymbolList symbol_list;
TokenList token_list;

void setError(std::string s)
{
    state.error = 1;
    state.message = s;
}

void printError()
{
    std::cout << "Error on line - " << data::state.line_number << std::endl;
    std::cout << ">>> " << data::state.line << std::endl;
    std::cout << ">>> " << data::state.message << std::endl;
    std::cout << "Build failed" << std::endl;
}

void log(std::string s)
{
    data::data.ins_list.push_back(s);
    data::data.log(data::state.line_number, data::state.prog_count++, s, data::state.line);
}

void logMacro(std::string s)
{
    data::data.ins_list.push_back(s);
    data::data.log(data::state.line_number, data::state.prog_count++, s, "<________");
}

} // namespace data