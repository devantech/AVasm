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

void instructions::ST::build()
{
    Symbol dest, reg;
    command |= 0x80;

    if (!getPossibleIndirectReg(dest))
    {
        state.error = 1;
        state.message = "Destination register for st command must be an indirection.";
        return;
    }

    if (!checkComma())
        return;

    getReg(reg);
    if (state.error)
        return;

    std::string cmd = stutils::int_to_hex(command);
    cmd += stutils::int_to_hex(dest.location());
    cmd += "00";
    cmd += stutils::int_to_hex(reg.location());

    data.ins_list.push_back(cmd);
    data.insertLog(state.line_number, state.prog_count++, cmd, state.line);
}

void instructions::LD::build()
{
    command |= 0x40;

    Symbol dest, reg;

    getReg(dest);
    if (state.error)
        return;

    if (!checkComma())
        return;

    if (!getPossibleIndirectReg(reg))
    {
        state.error = 1;
        state.message = "Value register for ld command must be an indirection.";
        return;
    }

    std::string cmd = stutils::int_to_hex(command);
    cmd += stutils::int_to_hex(dest.location());
    cmd += "00";
    cmd += stutils::int_to_hex(reg.location());

    data.ins_list.push_back(cmd);
    data.insertLog(state.line_number, state.prog_count++, cmd, state.line);
}