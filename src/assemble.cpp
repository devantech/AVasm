
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

int Assemble::go()
{
    preprocess();
    if (state.error)
        return state.error;
    assemble();
    if (state.error)
        return state.error;
    return 0;
}

void Assemble::preprocess(void)
{
    while (std::getline(file, state.line))
    {
        state.line_number++;
        std::string commentless = stutils::stripComment(state.line);
        stutils::trim(commentless);
        if (commentless.empty())
        {
            data.log(state.line_number, -1, "", state.line);
            continue;
        }
        token_list.getAllTokens(commentless);
        preprocessLine();
        if (state.error)
        {
            std::cout << "Error on line - " << state.line_number << std::endl;
            std::cout << ">>> " << state.line << std::endl;
            std::cout << ">>> " << state.message << std::endl;
            return;
        }
    }

    if (data.process_count < 7)
    {
        state.error = 1;
        state.message = "There must be a minimum of 7 processes in a project, only " + std::to_string(data.process_count) + " found.";
        return;
    }

    file.clear();
    file.seekg(0, file.beg); // Inished reading the file in
                             // so return to the beginning of
                             // the file for the next pass
}

void Assemble::assemble(void)
{
    state.line_number = 0;
    state.prog_count = 0;
    while (std::getline(file, state.line))
    {
        state.line_number++;
        std::string commentless = stutils::stripComment(state.line);
        stutils::trim(commentless);
        if (commentless.empty())
        {
            continue;
        }
        token_list.getAllTokens(commentless);
        assembleLine();
        if (state.error)
        {
            std::cout << "Error on line - " << state.line_number << std::endl;
            std::cout << ">>> " << state.line << std::endl;
            std::cout << ">>> " << state.message << std::endl;
            return;
        }
    }

    file.seekg(0, file.beg); // Inished reading the file in
                             // so return to the beginning of
                             // the file for the next pass
}

void Assemble::preprocessLine(void)
{
    Token t = token_list.getNext();
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
        state.in_process = true;
        data.process_count++;
        state.process_name = token_list.expect(state.error, {IDENTIFIER}).s_value + "_";
        if (state.error)
            return;
        break;
    case ENDPROCESS:
        doEndProcess();
        break;
    case INSTRUCTION:
        state.prog_count++;
        break;
    case LABEL:
        doLabel(t);
        if (token_list.hasNext())
            preprocessLine();
        break;
    case MACRO:
        state.prog_count += instructions::getLengthOfMacro(t.s_value);
        break;
    case NONE:
    default:
        state.message = "Unknown instruction --> " + t.s_value;
        state.error = 1;
        return;
    }
}

void Assemble::assembleLine(void)
{
    Token t = token_list.getNext();
    Token i;
    switch (t.type)
    {
    case REGISTER:
    case CONST:
    case DATA:
        break;
    case LABEL:
        if (token_list.hasNext())
            assembleLine(); // A label may well have an instruction on the same line
        else                // So we call preprocessLine again so we can catch it
        {
            data.log(state.line_number, state.prog_count, "", state.line);
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
        state.message = "Unknown instruction --> " + t.s_value;
        state.error = 1;
        return;
    }
}

void Assemble::doLabel(Token &label)
{
    if (!state.in_process)
    {
        state.error = 1;
        state.message = "Label found outside of process";
        return;
    }
    std::string s = state.process_name + label.s_value;
    symbol_list.addSymbolToTable(state.error, label.type, state.prog_count, state.prog_count, state.prog_count, s);
}

void Assemble::doProcess(void)
{
    if (state.in_process)
    {
        state.error = 1;
        state.message = ("Trying to declare process inside another process, did "
                         "you forget an endprocess?");
        return;
    }

    Token t = token_list.expect(state.error, {IDENTIFIER});
    if (state.error)
    {
        state.message = "Expected valid process name but found -> " + t.s_value;
        return;
    }
    state.in_process = true;
    state.process_name = t.s_value + "_";
    data.pc_list.push_back(stutils::int_to_hex((state.prog_count >> 8) & 0xff) + stutils::int_to_hex(state.prog_count & 0xff));
    if (token_list.hasNext())
    {
        state.error = 1;
        state.message = ("Unexpected instruction found after process name.");
        return;
    }
    data.log(state.line_number, state.prog_count, "", state.line);
}

void Assemble::doEndProcess(void)
{
    if (!state.in_process)
    {
        state.error = 1;
        state.message = ("endprocess found with no matching process before it.");
        return;
    }
    state.in_process = false;
    state.process_name = "";
    if (token_list.hasNext())
    {
        state.error = 1;
        state.message = ("Unexpected instruction found after endprocess.");
        return;
    }
    data.log(state.line_number, -1, "", state.line);
}

void Assemble::doSymbol(int type)
{
    switch (type)
    {
    case REGISTER:
        RegType(data, state, token_list, symbol_list).create();
        break;
    case DATA:
        DataType(data, state, token_list, symbol_list).create();
        break;
    case CONST:
        ConstType(data, state, token_list, symbol_list).create();
        break;
    }
}

void Assemble::doInstruction(Token &ins)
{
    if (ins.s_value == INSTRUCTION_NOP)
    {
        data.ins_list.push_back("00000000");
        data.log(state.line_number, state.prog_count++, "00000000", state.line);
        return;
    }

    switch (ins.value)
    {
    case INSTRUCTION_ADD_VALUE:
        instructions::ADD(data, state, token_list, symbol_list).build();
        break;
    case INSTRUCTION_AND_VALUE:
        instructions::AND(data, state, token_list, symbol_list).build();
        break;
    case INSTRUCTION_JAL_VALUE:
        instructions::JAL(data, state, token_list, symbol_list).build();
        break;
    case INSTRUCTION_JALR_VALUE:
        instructions::JALR(data, state, token_list, symbol_list).build();
        break;
    case INSTRUCTION_JBCR_VALUE:
        instructions::JBCR(data, state, token_list, symbol_list).build();
        break;
    case INSTRUCTION_JBSR_VALUE:
        instructions::JBSR(data, state, token_list, symbol_list).build();
        break;
    case INSTRUCTION_JEQR_VALUE:
        instructions::JEQR(data, state, token_list, symbol_list).build();
        break;
    case INSTRUCTION_JGER_VALUE:
        instructions::JGER(data, state, token_list, symbol_list).build();
        break;
    case INSTRUCTION_JLTR_VALUE:
        instructions::JLTR(data, state, token_list, symbol_list).build();
        break;
    case INSTRUCTION_JNER_VALUE:
        instructions::JNER(data, state, token_list, symbol_list).build();
        break;
    case INSTRUCTION_JNZ_VALUE:
        instructions::JNZ(data, state, token_list, symbol_list).build();
        break;
    case INSTRUCTION_JZ_VALUE:
        instructions::JZ(data, state, token_list, symbol_list).build();
        break;
    case INSTRUCTION_LDI_VALUE:
        instructions::LDI(data, state, token_list, symbol_list).build();
        break;
    case INSTRUCTION_OR_VALUE:
        instructions::OR(data, state, token_list, symbol_list).build();
        break;
    case INSTRUCTION_SRL_VALUE:
        instructions::SRL(data, state, token_list, symbol_list).build();
        break;
    case INSTRUCTION_XOR_VALUE:
        instructions::XOR(data, state, token_list, symbol_list).build();
        break;
    default:
        break;
    }
}

void Assemble::doMacro(Token &t)
{
    if (t.s_value == MACRO_CALL)
        instructions::Call(data, state, token_list, symbol_list).build();
    else if (t.s_value == MACRO_DJNZ)
        instructions::DJNZ(data, state, token_list, symbol_list).build();
    else if (t.s_value == MACRO_JMP)
        instructions::JMP(data, state, token_list, symbol_list).build();
    else if (t.s_value == MACRO_RETURN)
        instructions::RET(data, state, token_list, symbol_list).build();
    else if (t.s_value == MACRO_SLL)
        instructions::SLL(data, state, token_list, symbol_list).build();
    else if (t.s_value == MACRO_SUB)
        instructions::Sub(data, state, token_list, symbol_list).build();
    else if (t.s_value == MACRO_JLER)
        instructions::JLER(data, state, token_list, symbol_list).build();
    else if (t.s_value == MACRO_JGTR)
        instructions::JGTR(data, state, token_list, symbol_list).build();
    else if (t.s_value == MACRO_INC)
        instructions::INC(data, state, token_list, symbol_list).build();
    else if (t.s_value == MACRO_DEC)
        instructions::DEC(data, state, token_list, symbol_list).build();
    else if (t.s_value == MACRO_JEQ)
        instructions::JEQ(data, state, token_list, symbol_list).build();
    else if (t.s_value == MACRO_JNE)
        instructions::JNE(data, state, token_list, symbol_list).build();
    else if (t.s_value == MACRO_JGT)
        instructions::JGT(data, state, token_list, symbol_list).build();
    else if (t.s_value == MACRO_JLE)
        instructions::JLE(data, state, token_list, symbol_list).build();
    else if (t.s_value == MACRO_JLT)
        instructions::JLT(data, state, token_list, symbol_list).build();
    else if (t.s_value == MACRO_JGE)
        instructions::JGE(data, state, token_list, symbol_list).build();
    else if (t.s_value == MACRO_JBS)
        instructions::JBS(data, state, token_list, symbol_list).build();
    else if (t.s_value == MACRO_JBC)
        instructions::JBC(data, state, token_list, symbol_list).build();
    else if (t.s_value == MACRO_LD)
        instructions::LD(data, state, token_list, symbol_list).build();
    else if (t.s_value == MACRO_ST)
        instructions::ST(data, state, token_list, symbol_list).build();
    else if (t.s_value == MACRO_MOV)
        instructions::MOV(data, state, token_list, symbol_list).build();
}
