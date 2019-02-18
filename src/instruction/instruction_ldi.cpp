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

void instructions::LDI::build(void)
{
    Symbol target;
    getReg(target);
    if (state.error)
        return;

    if (!checkComma())
        return;

    int imm = getImmValue();
    if (state.error)
        return;
    
    if (!checkForMore())
        return;

    std::string inst = stutils::int_to_hex(command);
    inst += stutils::int_to_hex(target.location());
    inst += stutils::int_to_hex((imm >> 8) & 0xff);
    inst += stutils::int_to_hex(imm & 0xff);

    data.ins_list.push_back(inst);
    data.log(state.line_number, state.prog_count++, inst, state.line);
}

int instructions::LDI::getImmValue(void)
{
    Token t = token_list.expect(state.error, {NUMBER, IDENTIFIER});
    if (state.error)
        return 0;
    
    int val = numutils::getIValue(t, token_list, symbol_list, state);
    if (state.error)
        return 0;
    return val;
}