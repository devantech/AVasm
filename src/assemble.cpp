
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
#include "assemble.hpp"
#include "string_utils.hpp"
#include "macro.hpp"
#include "options.hpp"
#include "data_type.hpp"
#include "process_map.hpp"
#include <iostream>

int Assemble::go()
{
    preprocess();
    if (data::state.error)
        return data::state.error;
    assemble();
    if (data::state.error)
        return data::state.error;
    if ((getLCMForSeqData() * p_list.size()) > 511)
    {
        data::setError("Sequence ram overflow (" + std::to_string(getLCMForSeqData() * p_list.size()) + "/511)");
    }
    return 0;
}

void Assemble::preprocess(void)
{
    while (std::getline(file, data::state.line))
    {
        data::state.line_number++;
        std::string commentless = stutils::stripComment(data::state.line);
        stutils::trim(commentless);
        if (commentless.empty())
        {
            data::data.log(data::state.line_number, -1, "", data::state.line);
            continue;
        }
        data::token_list.getAllTokens(commentless);
        preprocessLine();
        if (data::state.error)
        {
            return;
        }
    }

    if (p_list.size() < 7)
    {
        data::setError("There must be a minimum of 7 processes in a project, only " + std::to_string(data::data.process_count) + " found.");
        return;
    }

    file.clear();
    file.seekg(0, file.beg); // Inished reading the file in
                             // so return to the beginning of
                             // the file for the next pass
}

void Assemble::assemble(void)
{
    data::state.line_number = 0;
    data::state.prog_count = 0;
    while (std::getline(file, data::state.line))
    {
        data::state.line_number++;
        std::string commentless = stutils::stripComment(data::state.line);
        stutils::trim(commentless);
        if (commentless.empty())
        {
            continue;
        }
        data::token_list.getAllTokens(commentless);
        assembleLine();
        if (data::state.error)
        {
            return;
        }
    }

    file.seekg(0, file.beg); // Inished reading the file in
                             // so return to the beginning of
                             // the file for the next pass
}

void Assemble::preprocessLine(void)
{
    Token t = data::token_list.getNext();
    Token t2;
    switch (t.type)
    {
    case REGISTER:
        doSymbol(REGISTER);
        break;
    case CONST:
        doSymbol(CONST);
        break;
    case DATA:
        doSymbol(DATA);
        break;
    case PROCESS:
        data::state.in_process = true;
        t2 = data::token_list.expect(data::state.error, {IDENTIFIER, SPLIT_IDENTIFIER});
        try
        {
            addProcessData(t2.s_value, data::data.process_count++);
        }
        catch (std::invalid_argument &ex)
        {
            data::setError(t2.s_value + std::string(ex.what()));
        }
        data::state.process_name = t2.s_value + "_";
        if (data::state.error)
            return;
        break;
    case ENDPROCESS:
        doEndProcess();
        break;
    case INSTRUCTION:
        data::state.prog_count++;
        break;
    case LABEL:
        doLabel(t);
        if (data::token_list.hasNext())
            preprocessLine();
        break;
    case MACRO:
        data::state.prog_count += instructions::getLengthOfMacro(t.s_value);
        break;
    case NONE:
    default:
        data::setError("Unknown instruction --> " + t.s_value);
        return;
    }
}

void Assemble::assembleLine(void)
{
    Token t = data::token_list.getNext();
    Token i;
    switch (t.type)
    {
    case REGISTER:
    case CONST:
    case DATA:
        break;
    case LABEL:
        if (data::token_list.hasNext())
            assembleLine(); // A label may well have an instruction on the same line
        else                // So we call preprocessLine again so we can catch it
        {
            data::data.log(data::state.line_number, data::state.prog_count, "", data::state.line);
        }
        break;
    case PROCESS:
        doProcess();
        break;
    case ENDPROCESS:
        doEndProcess();
        break;
    case INSTRUCTION:
        doInstruction(t);
        break;
    case MACRO:
        doMacro(t);
        break;
    case NONE:
    default:
        data::setError("Unknown instruction --> " + t.s_value);
        return;
    }
}

void Assemble::doLabel(Token &label)
{
    if (!data::state.in_process)
    {
        data::setError("Label found outside of process");
        return;
    }
    std::string s = data::state.process_name + label.s_value;
    try
    {
        data::symbol_list.addSymbolToTable(data::state.error, label.type, data::state.prog_count, data::state.prog_count, data::state.prog_count, s);
    }
    catch (std::invalid_argument ex)
    {
        data::setError("Label " + std::string(ex.what() + label.s_value));
    }
}

void Assemble::doProcess(void)
{
    if (data::state.in_process)
    {
        data::setError(("Trying to declare process inside another process, did "
                        "you forget an endprocess?"));
        return;
    }

    Token t = data::token_list.expect(data::state.error, {IDENTIFIER, SPLIT_IDENTIFIER});
    if (data::state.error)
    {
        data::setError("Expected valid process name but found -> " + t.s_value);
        return;
    }
    data::state.in_process = true;
    data::state.process_name = t.s_value + "_";
    data::data.pc_list.push_back(stutils::int_to_hex((data::state.prog_count >> 8) & 0xff) + stutils::int_to_hex(data::state.prog_count & 0xff));

    if (data::token_list.hasNext())
    {
        data::setError("Unexpected instruction found after process name.");
        return;
    }
    data::data.log(data::state.line_number, data::state.prog_count, "", data::state.line);
}

void Assemble::doEndProcess(void)
{
    if (!data::state.in_process)
    {
        data::setError("endprocess found with no matching process before it.");
        return;
    }
    data::state.in_process = false;
    data::state.process_name = "";
    if (data::token_list.hasNext())
    {
        data::setError("Unexpected instruction found after endprocess.");
        return;
    }
    data::data.log(data::state.line_number, -1, "", data::state.line);
}

void Assemble::doSymbol(int type)
{
    switch (type)
    {
    case REGISTER:
        data_type::createReg();
        break;
    case DATA:
        data_type::createData();
        break;
    case CONST:
        data_type::createConst();
        break;
    }
}

void Assemble::doInstruction(Token &ins)
{
    if (ins.s_value == INSTRUCTION_NOP)
    {
        data::data.ins_list.push_back("00000000");
        data::data.log(data::state.line_number, data::state.prog_count++, "00000000", data::state.line);
        return;
    }

    switch (ins.value)
    {
    case INSTRUCTION_ADD_VALUE:
        instructions::createALUInstruction(INSTRUCTION_ADD_VALUE);
        break;
    case INSTRUCTION_OR_VALUE:
        instructions::createALUInstruction(INSTRUCTION_OR_VALUE);
        break;
    case INSTRUCTION_AND_VALUE:
        instructions::createALUInstruction(INSTRUCTION_AND_VALUE);
        break;
    case INSTRUCTION_XOR_VALUE:
        instructions::createALUInstruction(INSTRUCTION_XOR_VALUE);
        break;
    case INSTRUCTION_SRL_VALUE:
        instructions::createSRLInstruction(INSTRUCTION_SRL_VALUE);
        break;
    case INSTRUCTION_JAL_VALUE:
        instructions::createLDIInstruction(INSTRUCTION_JAL_VALUE);
        break;
    case INSTRUCTION_JNZ_VALUE:
        instructions::createLDIInstruction(INSTRUCTION_JNZ_VALUE);
        break;
    case INSTRUCTION_JZ_VALUE:
        instructions::createLDIInstruction(INSTRUCTION_JZ_VALUE);
        break;
    case INSTRUCTION_LDI_VALUE:
        instructions::createLDIInstruction(INSTRUCTION_LDI_VALUE);
        break;
    case INSTRUCTION_JALR_VALUE:
        instructions::createJALRInstruction(INSTRUCTION_JALR_VALUE);
        break;
    case INSTRUCTION_JEQR_VALUE:
        instructions::createJEQRInstruction(INSTRUCTION_JEQR_VALUE);
        break;
    case INSTRUCTION_JGER_VALUE:
        instructions::createJEQRInstruction(INSTRUCTION_JGER_VALUE);
        break;
    case INSTRUCTION_JLTR_VALUE:
        instructions::createJEQRInstruction(INSTRUCTION_JLTR_VALUE);
        break;
    case INSTRUCTION_JNER_VALUE:
        instructions::createJEQRInstruction(INSTRUCTION_JNER_VALUE);
        break;
    case INSTRUCTION_JBCR_VALUE:
        instructions::createJBSRInstruction(INSTRUCTION_JBCR_VALUE);
        break;
    case INSTRUCTION_JBSR_VALUE:
        instructions::createJBSRInstruction(INSTRUCTION_JBSR_VALUE);
        break;
    case INSTRUCTION_SETB_VALUE:
        instructions::createSetbClrbInstruction(INSTRUCTION_SETB_VALUE);
        break;
    case INSTRUCTION_CLRB_VALUE:
        instructions::createSetbClrbInstruction(INSTRUCTION_CLRB_VALUE);
        break;
    default:
        break;
    }
}

void Assemble::doMacro(Token &t)
{
    if (t.s_value == MACRO_CALL)
        instructions::macroCall();
    else if (t.s_value == MACRO_DJNZ)
        instructions::macroDjnz();
    else if (t.s_value == MACRO_JMP)
        instructions::macroJmp();
    else if (t.s_value == MACRO_RETURN)
        instructions::macroRet();
    else if (t.s_value == MACRO_SLL)
        instructions::macroSll();
    else if (t.s_value == MACRO_SUB)
        instructions::macroSub();
    else if (t.s_value == MACRO_JLER)
        instructions::macroJeq(INSTRUCTION_JLTR_VALUE);
    else if (t.s_value == MACRO_JGTR)
        instructions::macroJeq(INSTRUCTION_JGER_VALUE);
    else if (t.s_value == MACRO_INC)
        instructions::macroInc();
    else if (t.s_value == MACRO_DEC)
        instructions::macroDec();
    else if (t.s_value == MACRO_JEQ)
        instructions::macroJeq(INSTRUCTION_JEQR_VALUE);
    else if (t.s_value == MACRO_JNE)
        instructions::macroJeq(INSTRUCTION_JNER_VALUE);
    else if (t.s_value == MACRO_JGT)
        instructions::macroJle(INSTRUCTION_JLTR_VALUE);
    else if (t.s_value == MACRO_JLE)
        instructions::macroJle(INSTRUCTION_JGER_VALUE);
    else if (t.s_value == MACRO_JLT)
        instructions::macroJeq(INSTRUCTION_JLTR_VALUE);
    else if (t.s_value == MACRO_JGE)
        instructions::macroJeq(INSTRUCTION_JGER_VALUE);
    else if (t.s_value == MACRO_JBS)
        instructions::macroJbs(INSTRUCTION_JBSR_VALUE);
    else if (t.s_value == MACRO_JBC)
        instructions::macroJbs(INSTRUCTION_JBCR_VALUE);
    else if (t.s_value == MACRO_LD)
        instructions::macroLd();
    else if (t.s_value == MACRO_ST)
        instructions::macroSt();
    else if (t.s_value == MACRO_MOV)
        instructions::macroMov();
}
