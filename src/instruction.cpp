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
#include "data.hpp"
#include "string_utils.hpp"
#include "num_utils.hpp"

namespace instructions
{

void getReg(Symbol &sym)
{
    getSymbol(sym);
    if (data::state.error == 1)
        return;
    if (data::token_list.get().type == INDIRECTION)
    {
        data::setError(data::token_list.get().s_value + " can not be an indirection.");
    }
    if (sym.type() != REGISTER)
    {
        data::setError(data::token_list.get().s_value + " is not a valid register");
    }
}

void getSymbol(Symbol &sym)
{
    Token dest = data::token_list.expect(data::state.error, {IDENTIFIER, INDIRECTION});
    if (data::state.error)
    {
        data::setError("Expected identifier but found -> " + data::token_list.get().s_value);
        return;
    }
    sym = data::symbol_list.getSymbolFromTable(data::state.error, dest.s_value, data::state.in_process, data::state.process_name);
    if (data::state.error)
    {
        data::setError(dest.s_value + " not declared in this scope");
        return;
    }
}

int checkComma(void)
{
    data::token_list.expect(data::state.error, {COMMA});
    if (data::state.error)
    {
        data::setError("Expected , but found -> " + data::token_list.getNext().s_value);
        return 0;
    }
    return 1;
}

int checkForMore(void)
{
    if (data::token_list.hasNext())
    {
        data::setError("Unexpected token found after instruction -> " + data::token_list.getNext().s_value);
        return 0;
    }
    return 1;
}

void createALUInstruction(int command)
{
    Symbol target, rs1, rs2;

    getALUTarget(target, command);
    if (data::state.error)
        return;

    if (!checkComma())
        return;

    getReg(rs1);

    if (!checkComma())
        return;

    getALUReg(rs2, command);
    if (data::state.error)
        return;

    if (!checkForMore())
        return;

    std::string inst = stutils::int_to_hex(command);
    inst += stutils::int_to_hex(target.location());
    inst += stutils::int_to_hex(rs1.location());
    inst += stutils::int_to_hex(rs2.location());

    data::log(inst);
    // data::data.ins_list.push_back(inst);
    // data::data.log(data::state.line_number, data::state.prog_count++, inst, data::state.line);
}

void createSetbClrbInstruction(int command)
{
    Symbol target, rs1;
    std::string inst;

    getALUTarget(target, command);
    if (data::state.error)
        return;

    if (!checkComma())
        return;

    int imm = getImmValue();

    if (data::token_list.hasNext())
    {
        if (!checkComma())
            return;

        getALUReg(rs1, command);
        if (data::state.error)
            return;

        inst = stutils::int_to_hex(command);
        inst += stutils::int_to_hex(target.location());
        inst += stutils::int_to_hex(imm & 0xff);
        inst += stutils::int_to_hex(rs1.location());
    }
    else
    {
        if (command & 0x80)
        {
            command |= 0x40;
        }
        inst = stutils::int_to_hex(command);
        inst += stutils::int_to_hex(target.location());
        inst += stutils::int_to_hex(imm & 0xff);
        inst += stutils::int_to_hex(target.location());
    }
    data::log(inst);
}

void getALUTarget(Symbol &sym, int &command)
{
    getSymbol(sym);
    if (sym.type() == REGISTER)
    {
        Token t = data::token_list.get();
        if (data::state.error)
            return;
        if (t.type == INDIRECTION)
            command |= 0x80;
    }
    else
    {
        data::setError("Invalid target register for instruction -> " + data::token_list.get().s_value);
    }
}

int getImmValue(void)
{
    Token t = data::token_list.expect(data::state.error, {NUMBER, IDENTIFIER});
    if (data::state.error)
        return 0;

    int val = numutils::getIValue(t);
    if (data::state.error)
        return 0;
    return val;
}

void getALUReg(Symbol &sym, int &command)
{
    getSymbol(sym);
    if (sym.type() == REGISTER)
    {
        Token t = data::token_list.get();
        if (data::state.error)
            return;
        if (t.type == INDIRECTION)
            command |= 0x40;
    }
    else
    {
        data::setError("Invalid register for instruction -> " + data::token_list.get().s_value);
    }
}

void createJALRInstruction(int command)
{
    Symbol target, rs1, rs2;

    getReg(target);
    if (data::state.error)
        return;

    if (!checkComma())
        return;

    getReg(rs1);
    if (data::state.error)
        return;

    if (!checkForMore())
        return;

    std::string inst = stutils::int_to_hex(command);
    inst += stutils::int_to_hex(target.location());
    inst += stutils::int_to_hex(rs1.location());
    inst += stutils::int_to_hex(0);

    data::log(inst);
    // data::data.ins_list.push_back(inst);
    // data::data.log(data::state.line_number, data::state.prog_count++, inst, data::state.line);
}

void createJBSRInstruction(int command)
{
    Symbol target, rs1;
    int imm;

    getReg(target);
    if (data::state.error)
        return;
    if (!checkComma())
        return;

    // getALUReg(rs1, command);
    imm = getImmValue();
    if (data::state.error)
        return;
    if (!checkComma())
        return;

    getReg(rs1);
    if (data::state.error)
        return;
    if (!checkForMore())
        return;

    std::string inst = stutils::int_to_hex(command);
    inst += stutils::int_to_hex(target.location());
    inst += stutils::int_to_hex(imm & 0xff);
    inst += stutils::int_to_hex(rs1.location());

    data::log(inst);
}

void createJEQRInstruction(int command)
{
    Symbol target, rs1, rs2;

    getReg(target);
    if (data::state.error)
        return;

    if (!checkComma())
        return;

    getReg(rs1);
    if (data::state.error)
        return;

    if (!checkComma())
        return;

    getReg(rs2);
    if (data::state.error)
        return;

    if (!checkForMore())
        return;

    std::string inst = stutils::int_to_hex(command);
    inst += stutils::int_to_hex(target.location());
    inst += stutils::int_to_hex(rs1.location());
    inst += stutils::int_to_hex(rs2.location());

    data::log(inst);
}

void createLDIInstruction(int command)
{
    Symbol target;
    getReg(target);
    if (data::state.error)
        return;

    if (!checkComma())
        return;

    int imm = getImmValue();
    if (data::state.error)
        return;

    if (!checkForMore())
        return;

    std::string inst = stutils::int_to_hex(command);
    inst += stutils::int_to_hex(target.location());
    inst += stutils::int_to_hex((imm >> 8) & 0xff);
    inst += stutils::int_to_hex(imm & 0xff);

    data::log(inst);
}

void createSRLInstruction(int command)
{
    Symbol target, rs1, rs2;

    getALUTarget(target, command);
    if (data::state.error)
        return;

    if (!checkComma())
        return;

    getReg(rs1);

    if (!checkForMore())
        return;

    std::string inst = stutils::int_to_hex(command);
    inst += stutils::int_to_hex(target.location());
    inst += stutils::int_to_hex(rs1.location());
    inst += stutils::int_to_hex(0);

    data::log(inst);
}

} // namespace instructions