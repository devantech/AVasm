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

void instructions::JLE::build()
{
    Symbol sym, rs1, rs2, lab;
    getMacroRegister(sym, "1");
    if (state.error == 1)
        return;
    getReg(rs1);
    if (state.error == 1)
        return;
    if (!checkComma())
        return;
    getReg(rs2);
    if (state.error)
        return;
    if (!checkComma())
        return;
    getLabel(lab);
    if (state.error)
        return;
    if (!checkForMore())
        return;

    std::string inst1 = stutils::int_to_hex(INSTRUCTION_LDI_VALUE);
    inst1 += stutils::int_to_hex(sym.location());
    inst1 += stutils::int_to_hex((lab.location() >> 8) & 0xff);
    inst1 += stutils::int_to_hex(lab.location() & 0xff);

    std::string inst2 = stutils::int_to_hex(command);
    inst2 += stutils::int_to_hex(sym.location());
    inst2 += stutils::int_to_hex(rs2.location());
    inst2 += stutils::int_to_hex(rs1.location());

    data.ins_list.push_back(inst1);
    data.insertLog(state.line_number, state.prog_count++, inst1, state.line);
    data.ins_list.push_back(inst2);
    data.insertLog(state.line_number, state.prog_count++, inst2, COMMAND_EXPANDER);
}