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
} // namespace instructions

int instructions::getLengthOfMacro(std::string m)
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
    return 0; // Not a macro so no length
}

/***********************************************
 * 
 * MacroCommand defs
 * 
 ***********************************************/

void instructions::MacroCommand::getLabel(Symbol &sym)
{
    getSymbol(sym);
    if (state.error == 1)
        return;
    if (sym.type() != LABEL)
    {
        state.error = 1;
        state.message = token_list.get().s_value + " is not a valid label";
    }
}

void instructions::MacroCommand::getMacroRegister(Symbol &sym, std::string suffix)
{
    std::string data_reg_name = "___macro" + state.process_name + suffix + "__";

    try
    {
        sym = symbol_list.getSymbol(data_reg_name);
    }
    catch (std::invalid_argument &e)
    {
        Symbol s(REGISTER, 0, 1, state.register_count++);
        symbol_list.addSymbol(data_reg_name, s);
        data.reg_list.push_back("0000"); // The register has a value of 0
        try
        {
            sym = symbol_list.getSymbol(data_reg_name);
        }
        catch (std::invalid_argument &e)
        {
            state.error = 1;
            state.message = "Error processing sub command";
        }
    }
}

bool instructions::MacroCommand::getPossibleIndirectReg(Symbol & sym)
{
    getSymbol(sym);
    if (sym.type() == REGISTER)
    {
        Token t = token_list.get();
        if (state.error)
            return false;
        if (t.type == INDIRECTION)
            return true;
    } else {
        state.error = 1;
        state.message = "Invalid target register for instruction -> " + token_list.get().s_value;
    }
    return false;
}