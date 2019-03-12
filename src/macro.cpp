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
#include "data.hpp"
#include "string_utils.hpp"

namespace instructions
{
const int MACRO_SUB_LENGTH = 3;
const int MACRO_DJNZ_LENGTH = 2;
const int MACRO_JMP_LENGTH = 1;
const int MACRO_CALL_LENGTH = 1;
const int MACRO_RETURN_LENGTH = 1;
const int MACRO_SLL_LENGTH = 1;
const int MACRO_JLER_LENGTH = 1;
const int MACRO_JGTR_LENGTH = 1;
const int MACRO_INC_LENGTH = 1;
const int MACRO_DEC_LENGTH = 1;
const int MACRO_JEQ_LENGTH = 2;
const int MACRO_JNE_LENGTH = 2;
const int MACRO_JLE_LENGTH = 2;
const int MACRO_JGT_LENGTH = 2;
const int MACRO_JBS_LENGTH = 2;
const int MACRO_JBC_LENGTH = 2;
const int MACRO_JGE_LENGTH = 2;
const int MACRO_JLT_LENGTH = 2;
const int MACRO_ST_LENGTH = 1;
const int MACRO_LD_LENGTH = 1;
const int MACRO_MOV_LENGTH = 1;

int getLengthOfMacro(std::string m)
{
    if (m == MACRO_CALL)
        return MACRO_CALL_LENGTH;
    else if (m == MACRO_DJNZ)
        return MACRO_DJNZ_LENGTH;
    else if (m == MACRO_JMP)
        return MACRO_JMP_LENGTH;
    else if (m == MACRO_RETURN)
        return MACRO_RETURN_LENGTH;
    else if (m == MACRO_SLL)
        return MACRO_SLL_LENGTH;
    else if (m == MACRO_SUB)
        return MACRO_SUB_LENGTH;
    else if (m == MACRO_JLER)
        return MACRO_JLER_LENGTH;
    else if (m == MACRO_JGTR)
        return MACRO_JGTR_LENGTH;
    else if (m == MACRO_INC)
        return MACRO_INC_LENGTH;
    else if (m == MACRO_DEC)
        return MACRO_DEC_LENGTH;
    else if (m == MACRO_JEQ)
        return MACRO_JEQ_LENGTH;
    else if (m == MACRO_JNE)
        return MACRO_JNE_LENGTH;
    else if (m == MACRO_JLE)
        return MACRO_JLE_LENGTH;
    else if (m == MACRO_JGT)
        return MACRO_JGT_LENGTH;
    else if (m == MACRO_JBS)
        return MACRO_JBS_LENGTH;
    else if (m == MACRO_JBC)
        return MACRO_JBC_LENGTH;
    else if (m == MACRO_JGE)
        return MACRO_JGE_LENGTH;
    else if (m == MACRO_JLT)
        return MACRO_JLT_LENGTH;
    else if (m == MACRO_LD)
        return MACRO_LD_LENGTH;
    else if (m == MACRO_ST)
        return MACRO_ST_LENGTH;
    else if (m == MACRO_MOV)
        return MACRO_MOV_LENGTH;
    return 0; // Not a macro so no length
}

/***********************************************
 * 
 * MacroCommand defs
 * 
 ***********************************************/

void getLabel(Symbol &sym)
{
    getSymbol(sym);
    if (data::state.error == 1)
        return;
    if (sym.type() != LABEL)
    {
        data::setError(data::token_list.get().s_value + " is not a valid label");
    }
}

void getMacroRegister(Symbol &sym, std::string suffix)
{
    std::string data_reg_name = "___macro" + data::state.process_name + suffix + "__";

    try
    {
        sym = data::symbol_list.getSymbol(data_reg_name);
    }
    catch (std::invalid_argument &e)
    {
        Symbol s(REGISTER, 0, 1, data::state.register_count++);
        data::symbol_list.addSymbol(data_reg_name, s);
        data::data.reg_list.push_back("0000"); // The register has a value of 0
        try
        {
            sym = data::symbol_list.getSymbol(data_reg_name);
        }
        catch (std::invalid_argument &e)
        {
            data::setError("Error processing sub command");
        }
    }
}

bool getPossibleIndirectReg(Symbol & sym)
{
    getSymbol(sym);
    if (sym.type() == REGISTER)
    {
        Token t = data::token_list.get();
        if (data::state.error)
            return false;
        if (t.type == INDIRECTION)
            return true;
    } else {
        data::setError("Invalid target register for instruction -> " + data::token_list.get().s_value);
    }
    return false;
}

int getMacroImmValue(void)
{
    Symbol sym;
    getSymbol(sym);
    if (data::state.error)
    {
        Token t = data::token_list.get();
        if (t.type == NUMBER)
        {
            data::state.error = 0;
            data::state.message = "";
            return t.value;
        }
        data::setError("Invalid immediate value");
        return -1;
    }

    switch (sym.type())
    {
    case CONST:
        return sym.value();
    default:
        data::setError("Invalid immediate value");
        return -1;
    }
}

void macroCall(void)
{
    Symbol lab, reg;
    getReg(reg);
    if (data::state.error)
        return;

    if (!checkComma())
        return;

    getLabel(lab);
    if (data::state.error)
        return;

    if (!checkForMore())
        return;

    std::string jmp = stutils::int_to_hex(INSTRUCTION_JAL_VALUE);
    jmp += stutils::int_to_hex(reg.location());
    jmp += stutils::int_to_hex((lab.location() >> 8) & 0xff);
    jmp += stutils::int_to_hex(lab.location() & 0xff);
    data::data.ins_list.push_back(jmp);
    data::data.insertLog(data::state.line_number, data::state.prog_count++, jmp, data::state.line);
}

void macroDec()
{
    Symbol dest;

    /*
    Get the symbols for the instructions
    */
    getReg(dest);
    if (data::state.error == 1)
        return;

    if (!checkForMore())
        return;

    std::string inst = stutils::int_to_hex(INSTRUCTION_ADD_VALUE);
    inst +=  stutils::int_to_hex(dest.location());
    inst +=  stutils::int_to_hex(dest.location());
    inst +=  "02";

    data::data.ins_list.push_back(inst);
    data::data.insertLog(data::state.line_number, data::state.prog_count++, inst, data::state.line);

}

void macroDjnz(void)
{
    Symbol dest, lab;

    getReg(dest);
    if (data::state.error)
        return;

    if (!checkComma())
        return;

    getLabel(lab);
    if (data::state.error)
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

    data::data.ins_list.push_back(add1);
    data::data.insertLog(data::state.line_number, data::state.prog_count++, add1, data::state.line);
    data::data.ins_list.push_back(jnz);
    data::data.insertLog(data::state.line_number, data::state.prog_count++, jnz, COMMAND_EXPANDER);
}

void macroInc()
{
    Symbol dest;

    /*
    Get the symbols for the instructions
    */
    getReg(dest);
    if (data::state.error == 1)
        return;

    if (!checkForMore())
        return;

    std::string inst = stutils::int_to_hex(INSTRUCTION_ADD_VALUE);
    inst +=  stutils::int_to_hex(dest.location());
    inst +=  stutils::int_to_hex(dest.location());
    inst +=  "01";

    data::data.ins_list.push_back(inst);
    data::data.insertLog(data::state.line_number, data::state.prog_count++, inst, data::state.line);

}

void macroJbs(int command)
{
    Symbol sym, rs1, lab;// rs2, ;
    getMacroRegister(sym, "1");
    if (data::state.error == 1)
        return;
        
    getALUReg(rs1, command);
    if (data::state.error == 1)
        return;

    if (!checkComma())
        return;

    int imm = getMacroImmValue();
    if (data::state.error)
        return;

    if (!checkComma())
        return;

    if (imm > 15)
    {
        data::setError( "immediate value out of range (0 - 15)  but is value -> " + std::to_string(imm));
        return;
    }

    getLabel(lab);
    if (data::state.error)
        return;

    if (!checkForMore())
        return;

    std::string inst1 = stutils::int_to_hex(INSTRUCTION_LDI_VALUE);
    inst1 += stutils::int_to_hex(sym.location());
    inst1 += stutils::int_to_hex((lab.location() >> 8) & 0xff);
    inst1 += stutils::int_to_hex(lab.location() & 0xff);

    std::string inst2 = stutils::int_to_hex(command);
    inst2 += stutils::int_to_hex(sym.location());
    inst2 += stutils::int_to_hex(imm);
    inst2 += stutils::int_to_hex(rs1.location());

    data::data.ins_list.push_back(inst1);
    data::data.insertLog(data::state.line_number, data::state.prog_count++, inst1, data::state.line);
    data::data.ins_list.push_back(inst2);
    data::data.insertLog(data::state.line_number, data::state.prog_count++, inst2, COMMAND_EXPANDER);
}

void macroJeq(int command)
{
    Symbol sym, rs1, rs2, lab;
    getMacroRegister(sym, "1");
    if (data::state.error == 1)
        return;
    getReg(rs1);
    if (data::state.error == 1)
        return;
    if (!checkComma())
        return;
    getReg(rs2);
    if (data::state.error)
        return;
    if (!checkComma())
        return;
    getLabel(lab);
    if (data::state.error)
        return;
    if (!checkForMore())
        return;

    std::string inst1 = stutils::int_to_hex(INSTRUCTION_LDI_VALUE);
    inst1 += stutils::int_to_hex(sym.location());
    inst1 += stutils::int_to_hex((lab.location() >> 8) & 0xff);
    inst1 += stutils::int_to_hex(lab.location() & 0xff);

    std::string inst2 = stutils::int_to_hex(command);
    inst2 += stutils::int_to_hex(sym.location());
    inst2 += stutils::int_to_hex(rs1.location());
    inst2 += stutils::int_to_hex(rs2.location());

    data::data.ins_list.push_back(inst1);
    data::data.insertLog(data::state.line_number, data::state.prog_count++, inst1, data::state.line);
    data::data.ins_list.push_back(inst2);
    data::data.insertLog(data::state.line_number, data::state.prog_count++, inst2, COMMAND_EXPANDER);
}

void macroJgtr(void)
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

    std::string inst = stutils::int_to_hex(INSTRUCTION_JLTR_VALUE);
    inst += stutils::int_to_hex(target.location());
    inst += stutils::int_to_hex(rs2.location());
    inst += stutils::int_to_hex(rs1.location());

    data::data.ins_list.push_back(inst);
    data::data.insertLog(data::state.line_number, data::state.prog_count++, inst, data::state.line);
}

void macroJle(int command)
{
    Symbol sym, rs1, rs2, lab;
    getMacroRegister(sym, "1");
    if (data::state.error == 1)
        return;
    getReg(rs1);
    if (data::state.error == 1)
        return;
    if (!checkComma())
        return;
    getReg(rs2);
    if (data::state.error)
        return;
    if (!checkComma())
        return;
    getLabel(lab);
    if (data::state.error)
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

    data::data.ins_list.push_back(inst1);
    data::data.insertLog(data::state.line_number, data::state.prog_count++, inst1, data::state.line);
    data::data.ins_list.push_back(inst2);
    data::data.insertLog(data::state.line_number, data::state.prog_count++, inst2, COMMAND_EXPANDER);
}

void macroJler()
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

    std::string inst = stutils::int_to_hex(INSTRUCTION_JGER_VALUE);
    inst += stutils::int_to_hex(target.location());
    inst += stutils::int_to_hex(rs2.location());
    inst += stutils::int_to_hex(rs1.location());

    data::data.ins_list.push_back(inst);
    data::data.insertLog(data::state.line_number, data::state.prog_count++, inst, data::state.line);
}

void macroJmp(void)
{
    Symbol lab;

    getLabel(lab);

    if (data::state.error)
        return;

    if (!checkForMore())
        return;

    std::string jmp = stutils::int_to_hex(INSTRUCTION_JAL_VALUE);
    jmp += "00"; // 0 in memory
    jmp += stutils::int_to_hex((lab.location() >> 8) & 0xff);
    jmp += stutils::int_to_hex(lab.location() & 0xff);
    data::data.ins_list.push_back(jmp);
    data::data.insertLog(data::state.line_number, data::state.prog_count++, jmp, data::state.line);
}

void macroSt(void)
{
    int command = 0x00;

    Symbol dest, reg;
    command |= 0x80;

    if (!getPossibleIndirectReg(dest))
    {
        data::setError("Destination register for st command must be an indirection.");
        return;
    }

    if (!checkComma())
        return;

    getReg(reg);
    if (data::state.error)
        return;

    std::string cmd = stutils::int_to_hex(command);
    cmd += stutils::int_to_hex(dest.location());
    cmd += "00";
    cmd += stutils::int_to_hex(reg.location());

    data::data.ins_list.push_back(cmd);
    data::data.insertLog(data::state.line_number, data::state.prog_count++, cmd, data::state.line);
}

void macroLd(void)
{
    int command = 0x00;

    command |= 0x40;

    Symbol dest, reg;

    getReg(dest);
    if (data::state.error)
        return;

    if (!checkComma())
        return;

    if (!getPossibleIndirectReg(reg))
    {
        data::setError("Value register for ld command must be an indirection.");
        return;
    }

    std::string cmd = stutils::int_to_hex(command);
    cmd += stutils::int_to_hex(dest.location());
    cmd += "00";
    cmd += stutils::int_to_hex(reg.location());

    data::data.ins_list.push_back(cmd);
    data::data.insertLog(data::state.line_number, data::state.prog_count++, cmd, data::state.line);
}

void macroMov(void)
{
    int command = 0x00;

    Symbol dest, reg;

    if (getPossibleIndirectReg(dest))
        command |= 0x80;

    if (!checkComma())
        return;

    if (getPossibleIndirectReg(reg))
        command |= 0x40;

    std::string cmd = stutils::int_to_hex(command);
    cmd += stutils::int_to_hex(dest.location());
    cmd += "00";
    cmd += stutils::int_to_hex(reg.location());

    data::data.ins_list.push_back(cmd);
    data::data.insertLog(data::state.line_number, data::state.prog_count++, cmd, data::state.line);
}

void macroRet(void)
{
    Symbol reg;
    getReg(reg);

    if (data::state.error)
        return;

    if (!checkForMore())
        return;

    std::string r = stutils::int_to_hex(INSTRUCTION_JALR_VALUE);
    r += stutils::int_to_hex(reg.location());
    r += stutils::int_to_hex(reg.location());
    r += "00";

    data::data.ins_list.push_back(r);
    data::data.insertLog(data::state.line_number, data::state.prog_count++, r, data::state.line);
}

void macroSll(void)
{
    Symbol dest, rs1;
    int command = INSTRUCTION_ADD_VALUE;

    if (getPossibleIndirectReg(dest))
        command |= 0x80;

    if (data::state.error)
        return;

    if (!checkComma())
        return;

    getReg(rs1);
    if (data::state.error)
        return;

    if (!checkForMore())
        return;

    std::string r = stutils::int_to_hex(command);
    r += stutils::int_to_hex(dest.location());
    r += stutils::int_to_hex(rs1.location());
    r += stutils::int_to_hex(rs1.location());

    data::data.ins_list.push_back(r);
    data::data.insertLog(data::state.line_number,data::state.prog_count++, r, data::state.line);

}

void macroSub(void)
{
    Symbol sym, dest, rs1, rs2;
    getMacroRegister(sym, "1");
    if (data::state.error == 1)
        return;

    int xor_command = INSTRUCTION_XOR_VALUE;
    int add_command = INSTRUCTION_ADD_VALUE;   
    /*
    Get the symbols for the instructions
    */
    if (getPossibleIndirectReg(dest))
        add_command |= 0x80;
        
    if (data::state.error == 1)
        return;

   if (!checkComma())
        return;

    getReg(rs1);
    if (data::state.error == 1)
        return;

    if (!checkComma())
        return;

    if(getPossibleIndirectReg(rs2))
        xor_command |= 0x40;
    if (data::state.error == 1)
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
    data::data.ins_list.push_back(xor_ins);
    data::data.insertLog(data::state.line_number, data::state.prog_count++, xor_ins, data::state.line);
    data::data.ins_list.push_back(add1);
    data::data.insertLog(data::state.line_number, data::state.prog_count++, add1, COMMAND_EXPANDER);
    data::data.ins_list.push_back(add2);
    data::data.insertLog(data::state.line_number, data::state.prog_count++, add2, COMMAND_EXPANDER);
}

} // namespace instructions