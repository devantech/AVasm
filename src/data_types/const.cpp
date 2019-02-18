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

void ConstType::create(void)
{
    if (state.in_process)
    {
        state.error = 1;
        state.message = "Constant valaues must be declared in global scope";
    }

    Token iden;
    if (!getIdentifier(iden))
        return;
    
    if (!token_list.hasNext())
    {
        state.error = 1;
        state.message = "No value set for constant";
        return;
    }

    Token t = token_list.expect(state.error, {NUMBER, IDENTIFIER});
    if (state.error)
        return;
    
    int val = numutils::getAValue(t, token_list, symbol_list, state);
    if (state.error)
        return;
    
    // if (!getValue(val))
    //     return;

    if (checkForMore()){        // There should be no more tokens here
        state.error = 1;
        state.message = "Unexpected token after setting constant value -> " + token_list.getNext().s_value;
        return;
    }

    int size = 1;               // Constvalues always have a size of 1
    int loc = 0;                // Const values alwayshave a location of 0 as they are not actually put in processor memory
    int type = CONST;
    std::string name = state.process_name + iden.s_value;

    symbol_list.addSymbolToTable(state.error, type, val, size, loc, name);
    if (state.error)
        state.message = "Duplicate constant definition -> " + iden.s_value;
        
    std::string line = stutils::int_to_hex((val >> 8) & 0xff);
    line += stutils::int_to_hex(val & 0xff);

    data.log(state.line_number, loc, line, state.line);
}


