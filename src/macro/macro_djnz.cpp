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
#include "macro.hpp"

void instructions::DJNZ::build(void)
{
    Symbol dest, lab;

    getReg(dest);
    if (state.error)
        return;

    if (!checkComma())
        return;

    getLabel(lab);
    if (state.error)
        return;
    
    if (!checkForMore())
        return;

    std::string add1 = stutils::int_to_hex(INSTRUCTION_ADD_VALUE);
    add1 += stutils::int_to_hex(dest.location());
    add1 += stutils::int_to_hex(dest.location());
    add1 += "02"; // The location of -1 in memory

    std::string jnz = stutils::int_to_hex(INSTRUCTION_JNZ_VALUE);
    jnz += stutils::int_to_hex(dest.location());
    jnz += stutils::int_to_hex((lab.location() >> 8) & 0xff);
    jnz += stutils::int_to_hex(lab.location() & 0xff);

    data.ins_list.push_back(add1);
    data.insertLog(state.line_number, state.prog_count++, add1, state.line);
    data.ins_list.push_back(jnz);
    data.insertLog(state.line_number, state.prog_count++, jnz, COMMAND_EXPANDER);
}