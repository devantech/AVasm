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



void instructions::Sub::build(void)
{
    Symbol sym, dest, rs1, rs2;
    getMacroRegister(sym, "1");
    if (state.error == 1)
        return;

    int xor_command = INSTRUCTION_XOR_VALUE;
    int add_command = INSTRUCTION_ADD_VALUE;   
    /*
    Get the symbols for the instructions
    */
    if (getPossibleIndirectReg(dest))
        add_command |= 0x80;
        
    if (state.error == 1)
        return;

   if (!checkComma())
        return;

    getReg(rs1);
    if (state.error == 1)
        return;

    if (!checkComma())
        return;

    if(getPossibleIndirectReg(rs2))
        xor_command |= 0x40;
    if (state.error == 1)
        return;

    if (!checkForMore())
        return;

    /*
    Create the instruction strings
    */
    std::string xor_ins = stutils::int_to_hex(xor_command);
    xor_ins += stutils::int_to_hex(sym.location());
    xor_ins += "02"; // the location of -1 in memory
    xor_ins += stutils::int_to_hex(rs2.location());

    std::string add1 = stutils::int_to_hex(INSTRUCTION_ADD_VALUE);
    add1 += stutils::int_to_hex(sym.location());
    add1 += stutils::int_to_hex(sym.location());
    add1 += "01"; // The location of 1 in memory

    std::string add2 = stutils::int_to_hex(add_command);
    add2 += stutils::int_to_hex(dest.location());
    add2 += stutils::int_to_hex(rs1.location());
    add2 += stutils::int_to_hex(sym.location());

    /*
    Push the instructions into the instruction SymbolList
    */
    data.ins_list.push_back(xor_ins);
    data.insertLog(state.line_number, state.prog_count++, xor_ins, state.line);
    data.ins_list.push_back(add1);
    data.insertLog(state.line_number, state.prog_count++, add1, COMMAND_EXPANDER);
    data.ins_list.push_back(add2);
    data.insertLog(state.line_number, state.prog_count++, add2, COMMAND_EXPANDER);
}

