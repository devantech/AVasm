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

#include "string_utils.hpp"
#include "token.hpp"

/**
 * Init static values
 */


std::unordered_map<std::string, int> Token::instructions = [] {
    std::unordered_map<std::string, int> m;
    m.insert(std::pair<std::string, int>(INSTRUCTION_ADD,   INSTRUCTION_ADD_VALUE));
    m.insert(std::pair<std::string, int>(INSTRUCTION_OR,    INSTRUCTION_OR_VALUE));
    m.insert(std::pair<std::string, int>(INSTRUCTION_AND,   INSTRUCTION_AND_VALUE));
    m.insert(std::pair<std::string, int>(INSTRUCTION_XOR,   INSTRUCTION_XOR_VALUE));
    m.insert(std::pair<std::string, int>(INSTRUCTION_SRL,   INSTRUCTION_SRL_VALUE));
    m.insert(std::pair<std::string, int>(INSTRUCTION_LDI,   INSTRUCTION_LDI_VALUE));
    m.insert(std::pair<std::string, int>(INSTRUCTION_JAL,   INSTRUCTION_JAL_VALUE));
    m.insert(std::pair<std::string, int>(INSTRUCTION_JZ,    INSTRUCTION_JZ_VALUE));
    m.insert(std::pair<std::string, int>(INSTRUCTION_JNZ,   INSTRUCTION_JNZ_VALUE));
    m.insert(std::pair<std::string, int>(INSTRUCTION_JALR,  INSTRUCTION_JALR_VALUE));
    m.insert(std::pair<std::string, int>(INSTRUCTION_JEQR,   INSTRUCTION_JEQR_VALUE));
    m.insert(std::pair<std::string, int>(INSTRUCTION_JNER,   INSTRUCTION_JNER_VALUE));
    m.insert(std::pair<std::string, int>(INSTRUCTION_JLTR,   INSTRUCTION_JLTR_VALUE));
    m.insert(std::pair<std::string, int>(INSTRUCTION_JGER,   INSTRUCTION_JGER_VALUE));
    m.insert(std::pair<std::string, int>(INSTRUCTION_JBSR,   INSTRUCTION_JBSR_VALUE));
    m.insert(std::pair<std::string, int>(INSTRUCTION_JBCR,   INSTRUCTION_JBCR_VALUE));
    m.insert(std::pair<std::string, int>(INSTRUCTION_NOP,   INSTRUCTION_NOP_VALUE));
    return m;
}();

std::vector<std::string> Token::macros = [] {
    std::vector<std::string> m;
    m.push_back(MACRO_CALL);
    m.push_back(MACRO_SUB);
    m.push_back(MACRO_SLL);
    m.push_back(MACRO_DJNZ);
    m.push_back(MACRO_RETURN);
    m.push_back(MACRO_JMP);
    m.push_back(MACRO_JLER);
    m.push_back(MACRO_JGTR);
    m.push_back(MACRO_JGTR);
    m.push_back(MACRO_JEQ);
    m.push_back(MACRO_JNE);
    m.push_back(MACRO_JGT);
    m.push_back(MACRO_JLE);
    m.push_back(MACRO_JLT);
    m.push_back(MACRO_JGE);
    m.push_back(MACRO_JBS);
    m.push_back(MACRO_JBC);
    m.push_back(MACRO_INC);
    m.push_back(MACRO_DEC);
    m.push_back(MACRO_LD);
    m.push_back(MACRO_ST);
    m.push_back(MACRO_MOV);
    return m;
}();

void Token::createToken()
{
    s_value = stutils::cpy_trim(identifier);
    checkInstruction() || checkMacros() || checkNumber() || checkType() || checkProcess() || checkLabel() || checkString() || checkSize() || checkIndirection() || checkIdentifier() || checkOperator();
}

int Token::checkInstruction(void)
{
    auto found = instructions.find(s_value);

    if (found == instructions.end())
    {
        return 0;
    }

    // found, store type and return
    type = INSTRUCTION;
    value = instructions.at(s_value);
    return 1;
}

/**
 * Check to see if this is a macro command that we are dealing with
 */
int Token::checkMacros(void)
{
    if (std::find(macros.begin(), macros.end(), s_value) != macros.end())
    {
        type = MACRO;
        value = 0;
        return 1;
    }
    return 0;
}

int Token::checkNumber(void)
{
    std::size_t p;
    try
    {
        uint16_t v = std::stoi(s_value, &p, stutils::calculateBase(s_value));
        if (p != s_value.size())
        {
            return 0;
        }
        type = NUMBER;
        value = v;
        return 1;
    }
    catch (std::invalid_argument &e)
    {
        return 0;
    }
    catch (std::out_of_range &e)
    {
        return 0;
    }
}

int Token::checkType(void)
{
    if (s_value == REG)
    {
        type = REGISTER;
    }
    else if (s_value == CON)
    {
        type = CONST;
    }
    else if (s_value == DAT)
    {
        type = DATA;
    }
    else
    {
        return 0;
    }
    value = 0;
    return 1;
}

int Token::checkProcess(void)
{
    if (s_value == PROC)
    {
        type = PROCESS;
    }
    else if (s_value == EPROC)
    {
        type = ENDPROCESS;
    }
    else
    {
        return 0;
    }
    value = 0;
    return 1;
}

int Token::checkIdentifier(void)
{
    if (s_value == ",")
    {
        type = COMMA;
        return 1;
    }
    std::regex rex("(^[_a-zA-Z]+[_a-zA-Z0-9]*$)"); // regex to match an identifier
    if (std::regex_match(s_value, rex))
    {
        type = IDENTIFIER;
        value = 0;
        return 1;
    }
    return 0;
}

int Token::checkSize(void)
{
    std::regex rex("\\[([\\s\\S]*)\\]");
    std::smatch match;
    if (std::regex_search(s_value, match, rex))
    {
        type = SIZE;
        s_value = match.str(1);
            return 1;
    }
    return 0;
}

int Token::checkIndirection(void)
{
    std::regex rex("\\((\\w*?)\\)");
    std::smatch match;
    if (std::regex_search(s_value, match, rex))
    {
        std::string s = match.str(1);
        s_value = s;
        type = INDIRECTION;
        value = 0;
        return 1;
    }
    return 0;
}

int Token::checkString(void)
{
    std::regex rex("\"(.*?)\"");
    std::smatch match;
    if (std::regex_search(s_value, match, rex))
    {
        type = STRING;
        s_value = match.str(1); // Store the contents of the string
        value = s_value.length();
        return 1;
    }
    return 0;
}

int Token::checkLabel(void)
{
    std::regex reg("(\\w+\\s*):");
    std::smatch match;
    if (std::regex_match(s_value, match, reg))
    {
        type = LABEL;
        s_value = match.str(1);
        return 1;
    }
    return 0;
}

int Token::checkOperator(void)
{
    if (s_value == "*" || s_value == "/" || s_value == "+" || s_value == "-")
    {
        type = OPERATOR;
        return 1;
    }
    return 0;
}