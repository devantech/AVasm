/*
 *  
 *  Copyright(C) 2018 Gerald Coe, Devantech Ltd <gerry@devantech.co.uk>
 * 
 *  Permission to use, copy, modify, and/or distribute this software for any purpose with or
 *  without fee is hereby granted, provided that the above copyright notice and 
 *  this permission notice appear in all copies.
 * 
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO
 *  THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. 
 *  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL 
 *  DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 *  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 *  CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 */
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