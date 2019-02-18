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
#include "instruction.hpp"

void instructions::ADD::build(void)
{
    Symbol target, rs1, rs2;
    
    getALUTarget(target);
    if (state.error)
        return;
    
    if (!checkComma())
        return;

    getReg(rs1);

    if (!checkComma())
        return;
    
    getALUReg(rs2);
    if (state.error)
        return;
    
    if (!checkForMore())
        return;

    std::string inst = stutils::int_to_hex(command);
    inst += stutils::int_to_hex(target.location());
    inst += stutils::int_to_hex(rs1.location());
    inst += stutils::int_to_hex(rs2.location());

    data.ins_list.push_back(inst);
    data.log(state.line_number, state.prog_count++, inst, state.line);
}

void instructions::ADD::getALUTarget(Symbol & sym)
{
    getSymbol(sym);
    if (sym.type() == REGISTER)
    {
        Token t = token_list.get();
        if (state.error)
            return;
        if (t.type == INDIRECTION)
            command |= 0x80;
    } else {
        state.error = 1;
        state.message = "Invalid target register for instruction -> " + token_list.get().s_value;
    }
}

void instructions::ADD::getALUReg(Symbol & sym)
{
    getSymbol(sym);
    if (sym.type() == REGISTER)
    {
        Token t = token_list.get();
        if (state.error)
            return;
        if (t.type == INDIRECTION)
            command |= 0x40;
    } else {
        state.error = 1;
        state.message = "Invalid register for instruction -> " + token_list.get().s_value;
    }
}