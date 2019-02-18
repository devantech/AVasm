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
#include "data_type.hpp"

void RegType::create(void)
{
    Token iden;
    if (!getIdentifier(iden))
        return;

    int val = 0;
    if (checkForMore())
    {
        Token t = token_list.expect(state.error, {NUMBER, IDENTIFIER});
        if (state.error)
            return;

        val = numutils::getAValue(t, token_list, symbol_list, state);
        if (state.error)
            return;
    }

    if (checkForMore())
    {
        state.error = 1;
        state.message = "unexpected token after setting register value -> " + token_list.getNext().s_value;
        return;
    }

    int location = state.register_count++;
    int type = REGISTER;
    int size = 1;

    std::string name = state.process_name + iden.s_value;

    std::string line = stutils::int_to_hex((val >> 8) & 0xff);
    line += stutils::int_to_hex(val & 0xff);
    data.reg_list.push_back(line);
    data.log(state.line_number, location, line, state.line);

    symbol_list.addSymbolToTable(state.error, type, val, size, location, name);
    if (state.error)
        state.message = "Duplicate register definition -> " + iden.s_value;
}